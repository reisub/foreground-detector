#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "gaussian.hpp"

int main( int argc, char** argv ) {
    if( argc != 3) {
        std::cout <<" Usage: " << argv[0] << " ImageToBlur BlurRadius" << std::endl;
        return -1;
    }

    unsigned int radius = atoi(argv[2]);
    cv::Mat image, blurred;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    // create empty result matrix for grayscale image - 8-bit, unsigned, 1-channel
    blurred = cv::Mat(image.size(), CV_8UC1, cv::Scalar::all(0));

    if(! image.data ) {                              // Check for invalid input
        std::cout <<  "Could not open or find the image" << std::endl;
        return -1;
    }

    grayscaleGaussianBlur(image, blurred, 2*radius+1);

    cv::namedWindow( "Original image", CV_WINDOW_AUTOSIZE );// Create a window for display.
    cv::namedWindow( "Gaussian blur", CV_WINDOW_AUTOSIZE );// Create a window for display.

    cv::imshow( "Original image", image );                   // Show our image inside it.
    cv::imshow( "Gaussian blur", blurred );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
