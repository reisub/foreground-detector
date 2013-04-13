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
  enum ProjectionType { HORIZONTAL, VERTICAL };
  void computeBinary();
  void computeProjections();
  void getProjection(cv::Mat &binary, std::vector<unsigned char> &projection, Foreground::ProjectionType type);
  void drawProjection(cv::Mat &graph, std::vector<unsigned char> &projection, Foreground::ProjectionType type);
};

#endif // FOREGROUND_HPP
