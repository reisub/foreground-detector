#include <opencv2/opencv.hpp>
#include <iostream>
#include "background.hpp"

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <path to video file>" << std::endl;
		exit(-1);
	}

	cv::VideoCapture video(argv[1]);
	if (!video.isOpened()) {
		std::cerr << "Could not open video file \"" << argv[1] << "\". Aborting." << std::endl;
		exit(-1);
	}

	process(video);

	video.release();

	return 0;
}