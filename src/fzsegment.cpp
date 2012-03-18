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
#include "pnmfile.h" // not needed as using opencv
#include "segment-image.h"

image<rgb> *opencv_to_imagefz(const cv::Mat &im) {
    image<rgb> *input = new image<rgb>(im.size().width, im.size().height);
    cv::MatConstIterator_<cv::Vec3b> it = im.begin<cv::Vec3b>();
    cv::MatConstIterator_<cv::Vec3b> it_end = im.end<cv::Vec3b>();
    rgb *imptr = input->data;
    for(; it != it_end; it++) {
        cv::Vec3b pixel = *it;
        imptr->r = pixel[0];
        imptr->g = pixel[1];
        imptr->b = pixel[2];
        imptr++;
    } // it
        
    return input;
}

cv::Mat *imagefz_to_opencv(image<rgb> *im) {
    int width = im->width();
    int height = im->height();
    cv::Mat *imout = new cv::Mat(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            cv::Vec3b pixel = imout->at<cv::Vec3b>(x, y);
            pixel[0] = imRef(im, x, y).r;
            pixel[1] = imRef(im, x, y).g;
            pixel[2] = imRef(im, x, y).b;
            imout->at<cv::Vec3b>(0,0) = pixel;
        } // x
    } // y
    
    return imout;
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
  cv::Mat im = cv::imread(argv[4]);
  if (!im.data)
  {
      std::cout << "failed to load : " << argv[4] << std::endl;
      return -1;
  }
  image<rgb> *input = opencv_to_imagefz(im);
  std::cout << "processing\n";
  int num_ccs; 
  image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs); 
  cv::Mat *segim = imagefz_to_opencv(seg);
  cv::imwrite(argv[5], *segim);
  std::cout << "got " << num_ccs << " components\n";
  std::cout << "done! uff...thats hard work.\n";

  delete input;
  delete segim;

  return 0;
}
