#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <opencv2/core/core.hpp>
#include <cassert>
#include <cmath>
#include <iostream>

void grayscaleGaussianBlur(cv::Mat &source, cv::Mat &destination, int sizeX, int sizeY = -1);

#endif // GAUSSIAN_H
