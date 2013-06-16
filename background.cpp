#include "background.hpp"
#include <algorithm>

#define FRAMES 20
#define DIFF_THRESH 40
#define MARGIN_SIDE 0.1
#define MARGIN_BOTTOM 0.15
#define HISTOGRAM_MOST 0.9

Background::Background(std::string filename): file(filename) {
//  std::cout << "Initializing stuff.." << std::endl;
  video.open(file);
  if (!video.isOpened()) {
      std::cerr << "Could not open video file \"" << file << "\". Aborting." << std::endl;
      exit(-1);
    }

  width = (int)video.get(CV_CAP_PROP_FRAME_WIDTH);
  height = (int)video.get(CV_CAP_PROP_FRAME_HEIGHT);
  backgroundModel = cv::Mat(cv::Size(width, height), CV_8UC1, cv::Scalar::all(0));
//   std::cout << "Video size is " << width << "x" << height << "." << std::endl;
  pixels = new std::vector<unsigned char>[width*height];
}

void Background::computeBasicModel() {
//  std::cout << "Computing basic background model.." << std::endl;
  assert(video.isOpened());

  cv::Mat frame, gray;

  // read frame by frame and process
  while(video.read(frame)) {
      // convert to grayscale and apply Gaussian blur
      cv::cvtColor(frame, gray, CV_RGB2GRAY);
      grayscaleGaussianBlur(gray, gray, 5);

      for (int row = 0; row < gray.rows; ++row) {
#pragma omp parallel for
          for (int col = 0; col < gray.cols; ++col) {
              pixels[row * width + col].push_back((int)gray.at<uchar>(row, col));
            }
        }

      frames.push_back(gray.clone());

    }

  // compute medians and get background model
  for (int row = 0; row < backgroundModel.rows; ++row) {
#pragma omp parallel for
      for (int col = 0; col < backgroundModel.cols; ++col) {
          backgroundModel.at<uchar>(row, col) = getMean(pixels[row * width + col]);
        }
    }

  cv::Mat hist;
  cv::Mat centralBgModel = cv::Mat(backgroundModel, cv::Range(0, backgroundModel.rows*(1.0 - MARGIN_BOTTOM)), cv::Range(backgroundModel.cols * MARGIN_SIDE, backgroundModel.cols*(1.0 - MARGIN_SIDE)));
  bgModelHist = computeHistogram(centralBgModel);
  drawHistogram(bgModelHist, hist);

//  cv::namedWindow( "Background", CV_WINDOW_AUTOSIZE );
//  cv::imshow( "Background", backgroundModel );
//  cv::namedWindow( "Histogram", CV_WINDOW_AUTOSIZE );
//  cv::imshow( "Histogram", hist );

  adjustBackgroundModel();
  drawHistogram(bgModelHist, hist);

//  cv::namedWindow( "Adjusted background", CV_WINDOW_AUTOSIZE );
//  cv::imshow( "Adjusted background", backgroundModel );
//  cv::namedWindow( "Adjusted histogram", CV_WINDOW_AUTOSIZE );
//  cv::imshow( "Adjusted histogram", hist );
//  cv::waitKey(0);

//  exit(0);
}

void Background::adjustBackgroundModel() {
  unsigned int totalPixels = 0;

  // get peak value
  unsigned int max = 0, maxPos = 0;
  for (int p = 0; p < 256; ++p) {
      totalPixels += bgModelHist[p];
      if(bgModelHist[p] > max) {
          max = bgModelHist[p];
          maxPos = p;
        }
    }

  unsigned int upperMost, lowerMost, upperAll, lowerAll;
  upperMost = lowerMost = upperAll = lowerAll = maxPos;

  unsigned int sum = bgModelHist[maxPos];
  bool directionUp = true, boundsMostFound = false, boundsAllFound = false;
  int counterUp = maxPos, counterDown = maxPos;
  while(sum < totalPixels) {
      if(directionUp) {
          counterUp++;
          if(counterUp < 255) {
              sum += bgModelHist[counterUp];
            }
          directionUp = false;
        }
      else {
          counterDown--;
          if(counterDown >= 0) {
              sum += bgModelHist[counterDown];
            }
          directionUp = true;
        }

      if(!boundsMostFound && sum >= HISTOGRAM_MOST*totalPixels) {
          upperMost = counterUp;
          lowerMost = counterDown;
          boundsMostFound = true;
        }
      if(!boundsAllFound && sum == totalPixels) {
          upperAll = counterUp;
          lowerAll = counterDown;
          boundsAllFound = true;
        }

    }

//  std::cout << "All pixels are in:(" << lowerAll << "," << upperAll << ")" << std::endl;
//  std::cout << "Most pixels are in:(" << lowerMost << "," << upperMost << ")" << std::endl;

  for (unsigned int i = 0; i < 256; ++i) {
      if(i < lowerMost || i > upperMost) {
          bgModelHist[i] = 0;
        }
    }

  for (int row = 0; row < backgroundModel.rows*(1.0 - MARGIN_BOTTOM); ++row) {
      for (int col = backgroundModel.cols * MARGIN_SIDE; col < backgroundModel.cols*(1.0 - MARGIN_SIDE); ++col) {
           if((unsigned int)backgroundModel.at<uchar>(row, col) < lowerMost || (unsigned int)backgroundModel.at<uchar>(row, col) > upperMost) {
               backgroundModel.at<uchar>(row, col) = (unsigned char)maxPos;
             }
        }
    }

  return;
}

Background::~Background() {
  video.release();
  delete [] pixels;
  delete [] bgModelHist;
}

unsigned char Background::getMean(std::vector<unsigned char> &v) {
  int hash[256] = { 0 };
  for (unsigned int var = 0; var < v.size(); ++var) {
      hash[v[var]]++;
    }
  int max = 0, maxIndex = 0;
  for (int var = 0; var < 256; ++var) {
      if(hash[var] > max) {
          max = hash[var];
          maxIndex = var;
        }
    }

  return maxIndex;
}

unsigned int *Background::computeHistogram(cv::Mat m) {
  unsigned int *hist = new unsigned int[256]();
  for (int row = 0; row < m.rows; ++row) {
      for (int col = 0; col < m.cols; ++col) {
          hist[(unsigned int)m.at<uchar>(row, col)]++;
        }
    }

  return hist;
}

unsigned char Background::getMedian(std::vector<unsigned char> &v) {
  size_t n = v.size() / 2;
  nth_element(v.begin(), v.begin()+n, v.end());

  return v[n];
}

void Background::drawHistogram(unsigned int *bins, cv::Mat &hist, unsigned int height, unsigned int binWidth, unsigned int binSpacing) {
  assert(height > 0);
  assert(binWidth > 0);

  unsigned int width = 256 * binWidth + 255 * binSpacing;
  hist = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(0));

  unsigned int max = 0;
  for (int bin = 0; bin < 256; ++bin) {
      if(bins[bin] > max) {
          max = bins[bin];
        }
    }

  unsigned int size;
  for (unsigned int bin = 0, position = 0; bin < 256; ++bin, position += binWidth + binSpacing) {
      size = (bins[bin]/(float)max) * height;
      cv::rectangle(hist, cv::Point(position, height - 1), cv::Point(position + binWidth - 1, height - 1 -size), cv::Scalar(255), CV_FILLED);
    }

}

















