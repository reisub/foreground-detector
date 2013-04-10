#include "foreground.hpp"

Foreground::Foreground(Background &back): background(back) {}

Foreground::~Foreground() {
  return;
}

void Foreground::computeBinary() {
  cv::Mat difference, binarized, sobel, x_grad, y_grad;
  cv::Mat x_grad_u8 = cv::Mat(background.backgroundModel.size(), CV_8UC1);
  cv::Mat y_grad_u8 = cv::Mat(background.backgroundModel.size(), CV_8UC1);

  cv::namedWindow( "Frame", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Difference", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Binary", CV_WINDOW_AUTOSIZE );

  for (unsigned int i = 0; i < background.frames.size(); ++i) {
      std::cout << "Frame #" << i << std::endl;
      difference = cv::abs(background.frames[i] - background.backgroundModel);

      // Reduce noise with Gaussian Blur
      grayscaleGaussianBlur(difference, difference, 11);

      cv::imshow( "Difference", difference );
      cv::threshold(difference, binarized, 30, 255, cv::THRESH_BINARY);
      binary.push_back(binarized);
      cv::imshow( "Frame", background.frames[i] );
      cv::imshow( "Binary", binarized );
//      cv::imshow( "Sobel", sobel );
      cv::waitKey(0);
    }
}
