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
  cv::Mat computeProjection(cv::Mat &binary, cv::Mat &frame, unsigned int frameN);
  void getProjection(cv::Mat &binary, std::vector<unsigned char> &projection, Foreground::ProjectionType type);
  void drawProjection(cv::Mat &graph, std::vector<unsigned char> &projection, Foreground::ProjectionType type);
  void getDimension(std::vector<unsigned char> &projection, Foreground::ProjectionType type, int *result);
};

#endif // FOREGROUND_HPP
