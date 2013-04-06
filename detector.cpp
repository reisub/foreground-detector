#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "gaussian.hpp"

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

	cv::Mat frame, gray;
    cv::namedWindow( "Frame", CV_WINDOW_AUTOSIZE );

	// read frame by frame and process
	while(video.read(frame)) {
		// convert to grayscale and apply Gaussian blur
		cvtColor(frame, gray, CV_RGB2GRAY);
		grayscaleGaussianBlur(gray, gray, 5);

		cv::imshow( "Frame", gray );
	    cv::waitKey(25);
	}

	video.release();

	return 0;
}