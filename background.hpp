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
  unsigned int *bgModelHist;
  std::vector<unsigned char> *pixels;
  std::vector<cv::Mat> frames;
  Background(std::string filename);
  ~Background();
  void computeBasicModel();
  void adjustBackgroundModel();
  unsigned char getMean(std::vector<unsigned char> &v);
  static unsigned int *computeHistogram(cv::Mat m);
  static unsigned char getMedian(std::vector<unsigned char> &v);
  static void drawHistogram(unsigned int *bins, cv::Mat &hist, unsigned int height = 512, unsigned int binWidth = 2, unsigned int binSpacing = 1);
};

#endif // BACKGROUND_HPP
