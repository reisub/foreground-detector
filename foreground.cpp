#include "foreground.hpp"

#define HPROJ_THRESH 0.10
#define VPROJ_THRESH 0.05

Foreground::Foreground(Background &back): background(back) {}

Foreground::~Foreground() {
  return;
}

void Foreground::computeBinary() {
  std::cout << "Computing binary images and projections.." << std::endl;

  cv::Mat difference, binarized;

  cv::namedWindow( "Person", CV_WINDOW_AUTOSIZE );

  for (unsigned int i = 0; i < background.frames.size(); ++i) {
      difference = cv::abs(background.frames[i] - background.backgroundModel);

      // Reduce noise with Gaussian Blur
      grayscaleGaussianBlur(difference, difference, 5);
      cv::threshold(difference, binarized, 40, 255, cv::THRESH_BINARY);
      binary.push_back(binarized.clone());

      cv::Mat bounded = computeProjection(binarized);

      cv::imshow("Person", bounded);

      cv::waitKey(0);
  }
}

cv::Mat Foreground::computeProjection(cv::Mat &binary) {
  cv::Mat vertical = cv::Mat(background.height, background.width, CV_8UC1);
  cv::Mat horizontal = cv::Mat(background.height, background.width, CV_8UC1);
  std::vector<unsigned char> hproj, vproj;
  int hdim[2], vdim[2];
  cv::Mat bounded = binary.clone();

  getProjection(binary, vproj, VERTICAL);
  drawProjection(vertical, vproj, VERTICAL);
  getDimension(vproj, VERTICAL, vdim);
  getProjection(binary, hproj, HORIZONTAL);
  drawProjection(horizontal, hproj, HORIZONTAL);
  getDimension(hproj, HORIZONTAL, hdim);

  cv::cvtColor(bounded, bounded, CV_GRAY2RGB);

  if(hdim[0] != -1 && hdim[1] != -1 && vdim[0] != -1 && vdim[1] != -1) {
      cv::rectangle(bounded, cv::Point(vdim[0], hdim[0]), cv::Point(vdim[1], hdim[1]), cv::Scalar(0, 0, 255), 1);
    }

  return bounded;

}

void Foreground::getProjection(cv::Mat &binary, std::vector<unsigned char> &projection, Foreground::ProjectionType type) {
  assert(binary.type() == CV_8UC1);
  projection.clear();
  int sum;

  if(type == HORIZONTAL) {
      // parallelize here
      for (int i = 0; i < binary.rows; ++i) {
          sum = 0;
          for (int j = 0; j < binary.cols; ++j) {
              if( (int)binary.at<uchar>(i, j) != 0 ) {
                  sum++;
                }
            }
          projection.push_back((char)sum);
        }
    }
  else {
      // parallelize here
      for (int i = 0; i < binary.cols; ++i) {
          sum = 0;
          for (int j = 0; j < binary.rows; ++j) {
              if( (int)binary.at<uchar>(j, i) != 0 ) {
                  sum++;
                }
            }
          projection.push_back((char)sum);
        }
  }
}

void Foreground::drawProjection(cv::Mat &graph, std::vector<unsigned char> &projection, Foreground::ProjectionType type) {
  assert(graph.type() == CV_8UC1);
  graph.setTo(cv::Scalar::all(0));

  if(type == HORIZONTAL) {
      for (unsigned int i = 0; i < projection.size(); ++i) {
          assert(projection.at(i) <= graph.cols);
          if((int)projection.at(i) != 0) {
              cv::line(graph, cv::Point(0, i), cv::Point(projection.at(i), i), cv::Scalar(255));
            }
        }
    }
  else {
      for (unsigned int i = 0; i < projection.size(); ++i) {
          assert(projection.at(i) <= graph.rows);
          if((int)projection.at(i) != 0) {
              cv::line(graph, cv::Point(i, graph.rows - 1), cv::Point(i, graph.rows - 1 - projection.at(i)), cv::Scalar(255));
            }
        }
    }
}

void Foreground::getDimension(std::vector<unsigned char> &projection, Foreground::ProjectionType type, int *result) {
  result[1] = result[0] = -1;
  int max = 0, maxIndex = -1;
  float threshold = (type == HORIZONTAL) ? HPROJ_THRESH : VPROJ_THRESH ;

  for (unsigned int var = 0; var < projection.size(); ++var) {
      if(projection[var] > max) {
          max = projection[var];
          maxIndex = var;
        }
    }

  if(max == 0) {
      return ;
    }

  result[1] = result[0] = maxIndex;

  // go from max left and right to borders and return borders
  int var;
  for (var = maxIndex; var >= 0; --var) {
      if(projection[var] < threshold*max) {
          result[0] = var;
          break;
        }
    }
  if(var <= 0) {
      result[0] = 0;
    }

  for (var = maxIndex; (unsigned)var < projection.size(); ++var) {
      if(projection[var] < threshold*max) {
          result[1] = var;
          break;
        }
    }
  if((unsigned)var >= projection.size() - 1) {
      result[1] = projection.size()-1;
    }


  return ;
}
