#ifndef BACKGROUND_H
#define BACKGROUND_H

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
	std::vector<char> *pixels;
	Background(std::string filename);
	~Background();
	void computeBasicModel();
	char getMedian(std::vector<char> &v);
};

#endif // BACKGROUND_H