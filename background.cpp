#include "background.hpp"

void process(cv::VideoCapture &video) {
	assert(video.isOpened());

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
}