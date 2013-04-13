#include <opencv2/opencv.hpp>
#include <iostream>
#include "background.hpp"
#include "foreground.hpp"

int main(int argc, char **argv) {
  if(argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <path to video file>" << std::endl;
      exit(-1);
    }

  std::string filename(argv[1]);

  Background background(filename);
  background.computeBasicModel();

  Foreground foreground(background);
  foreground.computeBinary();
  foreground.computeProjections();

  return 0;
}
