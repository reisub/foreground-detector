#include "background.hpp"
#include <algorithm>

#define FRAMES 10

Background::Background(std::string filename): file(filename) {
  std::cout << "Initializing stuff.." << std::endl;
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
  std::cout << "Computing basic background model.." << std::endl;
  assert(video.isOpened());

  cv::Mat frame, gray;
//  cv::namedWindow( "Frame", CV_WINDOW_AUTOSIZE );
//  cv::namedWindow( "Frame2", CV_WINDOW_AUTOSIZE );

  // read frame by frame and process
  while(video.read(frame)) {
      // convert to grayscale and apply Gaussian blur
      cv::cvtColor(frame, gray, CV_RGB2GRAY);
      grayscaleGaussianBlur(gray, gray, 5);

      for (int row = 0; row < gray.rows; ++row) {
          for (int col = 0; col < gray.cols; ++col)	{
              pixels[row * width + col].push_back((int)gray.at<uchar>(row, col));
            }
        }

      frames.push_back(gray.clone());


//      cv::imshow( "Frame", gray );
//      cv::imshow( "Frame2", frames.back() );
//      cv::waitKey(5);
    }

  // compute medians and get background model
  for (int row = 0; row < backgroundModel.rows; ++row) {
      for (int col = 0; col < backgroundModel.cols; ++col)	{
          backgroundModel.at<uchar>(row, col) = getMean(pixels[row * width + col]);
        }
    }

  // izračunati koliko se maksimalno frame razlikuje od modela pozadine
  // postaviti granicu eksperimantalno
  video.release();
  video.open(file);

  double difference_score = 0.0;
  int cnt = 0;

  while(video.read(frame)) {
      cnt++;
      if(cnt % FRAMES != 0) {
          continue;
        }

      cv::cvtColor(frame, gray, CV_RGB2GRAY);
      grayscaleGaussianBlur(gray, gray, 5);

      double difference = 0.0;

      for (int row = 0; row < gray.rows; ++row) {
          for (int col = 0; col < gray.cols; ++col) {
              // using Mean Squared Error
              difference += pow(abs((int)gray.at<uchar>(row, col)-(int)backgroundModel.at<uchar>(row, col))/10, 2);
            }
        }

      if(difference > difference_score) {
          difference_score = difference;
        }
    }

  std::cout << "Difference score: " << difference_score << std::endl;

  cv::namedWindow( "Background", CV_WINDOW_AUTOSIZE );
  cv::imshow( "Background", backgroundModel );
  cv::waitKey(0);
  exit(0);
}

Background::~Background() {
  video.release();
  delete [] pixels;
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

unsigned char Background::getMedian(std::vector<unsigned char> &v) {
  size_t n = v.size() / 2;
  nth_element(v.begin(), v.begin()+n, v.end());

  return v[n];
}
