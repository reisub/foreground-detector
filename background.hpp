#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <opencv2/opencv.hpp>
#include <cassert>
#include <vector>
#include "gaussian.hpp"

class Background {
public:
  std::string file;
  cv::VideoCapture video;
  cv::Mat backgroundModel;
  int width, height;
  std::vector<unsigned char> *pixels;
  std::vector<cv::Mat> frames;
  Background(std::string filename);
  ~Background();
  void computeBasicModel();
  static unsigned char getMedian(std::vector<unsigned char> &v);
};

#endif // BACKGROUND_HPP
