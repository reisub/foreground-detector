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
  cv::namedWindow( "Binary", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Sobel", CV_WINDOW_AUTOSIZE );

  for (unsigned int i = 0; i < background.frames.size(); ++i) {
      std::cout << "Frame #" << i << std::endl;
      difference = cv::abs(background.frames[i] - background.backgroundModel);
      cv::threshold(difference, binarized, 20, 255, cv::THRESH_BINARY);
      binary.push_back(binarized);
      Sobel (binarized, x_grad, CV_16S, 1, 0, 3, 1, 0);
      Sobel (binarized, y_grad, CV_16S, 0, 1, 3, 1, 0);
      cv::convertScaleAbs(x_grad, x_grad_u8);
      cv::convertScaleAbs(y_grad, y_grad_u8);
      cv::addWeighted( x_grad_u8, 0.5, y_grad_u8, 0.5, 0, sobel );
      cv::imshow( "Frame", background.frames[i] );
      cv::imshow( "Binary", binarized );
      cv::imshow( "Sobel", sobel );
      cv::waitKey(20);
    }
}


