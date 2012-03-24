/*
  compare.cpp
  
  compare two images to see if they are identical
*/

// c++
#include <iostream>
// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

// felzenswalb
#include "image.h"
#include "misc.h"
#include "filter.h"
#include "pnmfile.h"


using namespace std;
using namespace cv;

void compare_gaussians() {
  int width = 100, height = 100;
  // create felzenswalb image
  image<rgb> *imf = new image<rgb>(width,height);
  imRef(imf, 49,49).r = 255;
  imRef(imf, 49,49).g = 255;
  imRef(imf, 49,49).b = 255;
  // create opencv image
  cv::Mat *imo = new cv::Mat(height, width, CV_8UC3, cv::Scalar(0,0,0));
  cv::Vec3b pixel = imo->at<cv::Vec3b>(49, 49);
  pixel[0] = 255;
  pixel[1] = 255;
  pixel[2] = 255;
  imo->at<cv::Vec3b>(49, 49) = pixel;
  // save initial images
  savePPM(imf, "./inputf.ppm");
  imwrite("./inputo.png", *imo);
  // blur
  float sigma = 0.5;
  // felzenswalb
  image<float> *r = new image<float>(width, height);
  image<float> *g = new image<float>(width, height);
  image<float> *b = new image<float>(width, height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(r, x, y) = imRef(imf, x, y).r;
      imRef(g, x, y) = imRef(imf, x, y).g;
      imRef(b, x, y) = imRef(imf, x, y).b;
    }
  }
  image<float> *smooth_r = smooth(r, sigma);
  image<float> *smooth_g = smooth(g, sigma);
  image<float> *smooth_b = smooth(b, sigma);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(imf, x, y).r = imRef(r, x, y);
      imRef(imf, x, y).g = imRef(g, x, y);
      imRef(imf, x, y).b = imRef(b, x, y);
    }
  }
  // opencv
  cv::Mat *sm = new cv::Mat(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
  cv::GaussianBlur(*imo, *sm, cv::Size(0,0), sigma, 0);
  // save results
  savePPM(imf, "./outputf.ppm");
  imwrite("./outputo.png", *sm);
  // clean up
  delete imf;
  delete imo;
  delete sm;
  
}


int main(int argc, char **argv) {
 compare_gaussians();
 
 
 /* 
  if (argc != 3) {
    cerr <<  "usage: " << argv[0] << " image1 image2\n";
    return 1;
  }
  
  // load image1
  Mat im1 = imread(argv[1]);
  // load image2
  Mat im2 = imread(argv[2]);
  
  // check the dimensions are equal
  if (im1.size().width!=im2.size().width) {
    cout << "widths do not match : "<< im1.size().width << " " 
      << im2.size().width << endl;
    return 1;
  }
  if (im1.size().height!=im2.size().height) {
    cout << "widths do not match : "<< im1.size().height << " " 
      << im2.size().height << endl;
    return 1;
  }
  
  // check the pixels are equal
  int width = im1.size().width, height = im1.size().height;
  for (int y=0; y<height; y++) {
    for (int x=0; x<width; x++) {
      Vec3b p1 = im1.at<Vec3b>(y, x);
      Vec3b p2 = im2.at<Vec3b>(y, x);
      //cout << (int)p1[0] << "," << (int)p1[1] << "," << (int)p1[2] << "\t" 
      //  << (int)p2[0] << "," << (int)p2[1] << "," << (int)p2[2] << endl;
      if (p1[0]!=p2[0]) {
        cout << "pixels dont match (red) : " << x << " " << y << endl;
        return 1;
      }
      if (p1[1]!=p2[1]) {
        cout << "pixels dont match (green) : " << x << " " << y << endl;
        return 1;
      }
      if (p1[2]!=p2[2]) {
        cout << "pixels dont match (blue) : " << x << " " << y << endl;
        return 1;
      }
    } // x
  } // y
  cout << "images are the same" << endl;
  */
}
