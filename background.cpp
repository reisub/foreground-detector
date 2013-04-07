#include "background.hpp"

Background::Background(std::string filename): file(filename) {
	std::cout << "Initializing stuff.." << std::endl;
	video.open(file);
	if (!video.isOpened()) {
		std::cerr << "Could not open video file \"" << file << "\". Aborting." << std::endl;
		exit(-1);
	}

	width = (int)video.get(CV_CAP_PROP_FRAME_WIDTH);
	height = (int)video.get(CV_CAP_PROP_FRAME_HEIGHT);
	backgroundModel = cv::Mat(cv::Size(width, height), CV_8UC1, cv::Scalar::all(0));
	std::cout << "Video size is " << width << "x" << height << "." << std::endl;
	pixels = new std::vector<char>[width*height];
}

void Background::computeBasicModel() {
	std::cout << "Computing basic background model.." << std::endl;
	assert(video.isOpened());

	cv::Mat frame, gray;
    cv::namedWindow( "Frame", CV_WINDOW_AUTOSIZE );

	// read frame by frame and process
	while(video.read(frame)) {
		// convert to grayscale and apply Gaussian blur
		cvtColor(frame, gray, CV_RGB2GRAY);
		grayscaleGaussianBlur(gray, gray, 5);

		for (int row = 0; row < gray.rows; ++row) {
			for (int col = 0; col < gray.cols; ++col)	{
				pixels[row * width + col].push_back((int)gray.at<uchar>(row, col));
			}
		}

		cv::imshow( "Frame", gray );
	    cv::waitKey(5);
	}

	unsigned char value;

	// compute medians and get background model
	for (int row = 0; row < backgroundModel.rows; ++row) {
		for (int col = 0; col < backgroundModel.cols; ++col)	{
			backgroundModel.at<uchar>(row, col) = getMedian(pixels[row * width + col]);
		}
	}

    cv::namedWindow( "Background", CV_WINDOW_AUTOSIZE );
    cv::imshow( "Background", backgroundModel );
    cv::waitKey(0);
}

Background::~Background() {
	video.release();
	delete [] pixels;
}

char Background::getMedian(std::vector<char> &v) {
	size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin()+n, v.end());

    return v[n];
}