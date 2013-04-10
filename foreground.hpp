#ifndef FOREGROUND_HPP
#define FOREGROUND_HPP

#include <opencv2/opencv.hpp>
#include <cassert>
#include <vector>
#include "background.hpp"
#include "gaussian.hpp"

class Foreground {
public:
  Background &background;
  Foreground(Background &back);
  ~Foreground();
  std::vector<cv::Mat> binary;
  void computeBinary();
};

#endif // FOREGROUND_HPP
