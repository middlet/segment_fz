/*
  compare.cpp
  
  compare two images to see if they are identical
*/

// c++
#include <iostream>
// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  
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
}
