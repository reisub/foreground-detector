#include "foreground.hpp"

#define HPROJ_THRESH 0.10
#define VPROJ_THRESH 0.10

Foreground::Foreground(Background &back): background(back) {}

Foreground::~Foreground() {
  return;
}

void Foreground::computeBinary() {
  std::cout << "Computing binary images.." << std::endl;

  cv::Mat difference, binarized, sobel, x_grad, y_grad;

//  cv::namedWindow( "Frame", CV_WINDOW_AUTOSIZE );
//  cv::namedWindow( "Difference", CV_WINDOW_AUTOSIZE );
//  cv::namedWindow( "Binary", CV_WINDOW_AUTOSIZE );

  for (unsigned int i = 0; i < background.frames.size(); ++i) {
      difference = cv::abs(background.frames[i] - background.backgroundModel);

      // Reduce noise with Gaussian Blur
      grayscaleGaussianBlur(difference, difference, 5);
      cv::threshold(difference, binarized, 40, 255, cv::THRESH_BINARY);
      binary.push_back(binarized.clone());

//      cv::imshow( "Difference", difference );
//      cv::threshold(difference, binarized, 30, 255, cv::THRESH_BINARY);
//      cv::imshow( "Frame", background.frames[i] );
//      cv::imshow( "Binary", binarized );
//      cv::waitKey(10);
  }
}

void Foreground::computeProjections() {
  std::cout << "Computing projections.." << std::endl;

  cv::namedWindow( "Binary", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Proj_V", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Proj_H", CV_WINDOW_AUTOSIZE );

  cv::Mat vertical = cv::Mat(background.height, background.width, CV_8UC1);
  cv::Mat horizontal = cv::Mat(background.height, background.width, CV_8UC1);
  std::vector<unsigned char> values;

  for (unsigned int i = 0; i < binary.size(); ++i) {
      getProjection(binary.at(i), values, VERTICAL);
      drawProjection(vertical, values, VERTICAL);
      getProjection(binary.at(i), values, HORIZONTAL);
      drawProjection(horizontal, values, HORIZONTAL);
      cv::imshow( "Binary", binary.at(i) );
      cv::imshow( "Proj_V", vertical );
      cv::imshow( "Proj_H", horizontal );
      cv::waitKey(0);
    }
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

int* getHDimension(std::vector<unsigned char> &vprojection) {
  int result[2] = { -1, -1 };
  int max = 0, maxIndex = -1;
  for (int var = 0; var < vprojection.size(); ++var) {
      if(vprojection[var] > max) {
          max = vprojection[var];
          maxIndex = var;
        }
    }

  if(maxIndex = -1) {
      return result;
    }

  // go from max left and right to borders and return borders
  for (int var = maxIndex; var >= 0; --var) {
      if(vprojection[var] < VPROJ_THRESH*max) {
          result[0] = var;
          break;
        }
    }
  for (int var = maxIndex; var < vprojection.size(); ++var) {
      if(vprojection[var] < VPROJ_THRESH*max) {
          result[1] = var;
          break;
        }
    }

  result[0] = (result[0] == -1) ? 0 : result[0];
  result[1] = (result[1] == -1) ? vprojection.size()-1 : result[1];

  return result;
}

int* getVDimension(std::vector<unsigned char> &hprojection) {
  int result[2] = { -1, -1 };
  int max = 0, maxIndex = -1;
  for (int var = 0; var < hprojection.size(); ++var) {
      if(hprojection[var] > max) {
          max = hprojection[var];
          maxIndex = var;
        }
    }

  if(maxIndex = -1) {
      return result;
    }

  // go from max up and down to borders and return borders
  for (int var = maxIndex; var >= 0; --var) {
      if(hprojection[var] < HPROJ_THRESH*max) {
          result[0] = var;
          break;
        }
    }
  for (int var = maxIndex; var < hprojection.size(); ++var) {
      if(hprojection[var] < HPROJ_THRESH*max) {
          result[1] = var;
          break;
        }
    }

  result[0] = (result[0] == -1) ? 0 : result[0];
  result[1] = (result[1] == -1) ? hprojection.size()-1 : result[1];

  return result;
}
