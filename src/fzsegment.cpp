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
#include "image.h"
#include "misc.h"
//#include "pnmfile.h" // not needed as using opencv
#include "segment-image.h"

image<rgb> *opencv_to_imagefz(cv::Mat &im) {
    
    
}

int main(int argc, char **argv) {
  if (argc != 6) {
    std::cerr <<  "usage: " << argv[0] << " sigma k min input(ppm) output(ppm)\n";
    return 1;
  }
  
  float sigma = atof(argv[1]);
  float k = atof(argv[2]);
  int min_size = atoi(argv[3]);
	
  std::cout << "loading input image.\n";
  cv::Mat im = cv::imread(argv[4], CV_LOAD_IMAGE_COLOR);
  image<rgb> *input = opencv_to_imagefz(im);
  /*image<rgb> *input = loadPPM(argv[4]);
	
  std::cout << "processing\n";
  int num_ccs; 
  image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs); 
  savePPM(seg, argv[5]);

  std::cout << "got " << num_ccs << " components\n";
  std::cout << "done! uff...thats hard work.\n";*/

  return 0;
}
