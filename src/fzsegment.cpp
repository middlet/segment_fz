/*
    fzsegment.cpp
    
    port of the felzenswalb segmentation code to opencv
*/

// c++
#include <iostream>
#include <cstdio>
#include <cstdlib>
// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// local includes
#include "fz-segment-image.h"

int main(int argc, char **argv) {
  if (argc != 6) {
    std::cerr <<  "usage: " << argv[0] << " sigma k min input(ppm) output(ppm)\n";
    return 1;
  }
  
  float sigma = atof(argv[1]);
  float k = atof(argv[2]);
  int min_size = atoi(argv[3]);
	
  std::cout << "loading input image.\n";
  cv::Mat im = cv::imread(argv[4]);
  if (!im.data)
  {
      std::cout << "failed to load : " << argv[4] << std::endl;
      return -1;
  }
  std::cout << "processing\n";
  int num_ccs; 
  cv::Mat *seg = segment_image(&im, sigma, k, min_size, &num_ccs);
  cv::imwrite(argv[5], *seg);
  std::cout << "got " << num_ccs << " components\n";
  std::cout << "done! uff...thats hard work.\n";

  delete seg;

  return 0;
}
