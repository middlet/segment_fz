/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#ifndef SEGMENT_IMAGE
#define SEGMENT_IMAGE

#include <cstdlib>
#include <vector>
//#include <image.h>
//#include <misc.h>
//#include <filter.h>
#include "segment-graph.h"
#include "opencv2/imgproc/imgproc.hpp"

template <class T>
inline T square(const T &x) { return x*x; };

cv::Vec3b random_rgb(){ 
  cv::Vec3b c;
  
  c[0] = (uchar)random();
  c[1] = (uchar)random();
  c[2] = (uchar)random();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(cv::Mat *im, int x1, int y1, int x2, int y2) {
  cv::Vec3b p1 = im->at<cv::Vec3b>(y1, x1);
  cv::Vec3b p2 = im->at<cv::Vec3b>(y2, x2);  
  return sqrt(square(p1[0]-p2[0]) + square(p1[1]-p2[1]) + square(p1[2]-p2[2]));
}

// image convolution
void convolve_image(cv::Mat *src, cv::Mat *dst, std::vector<float> &mask) {
  int width = src->size().width, height = src->size().height;
  int len = mask.size();
  for (int y=0; y<height; y++) {
    for (int x=0; x<width; x++) {
      cv::Vec3b pixel = src->at<cv::Vec3b>(y, x);
      float sumr = mask[0]*pixel[0], sumg = mask[0]*pixel[1], 
        sumb = mask[0]*pixel[2];
      for (int i=1; i<len; i++) {
        cv::Vec3b p1 = src->at<cv::Vec3b>(y, std::max(x-i, 0));
        cv::Vec3b p2 = src->at<cv::Vec3b>(y, std::min(x+i, width-1));
        sumr += mask[i]*(p1[0]+p2[1]);
        sumg += mask[i]*(p1[1]+p2[1]);
        sumb += mask[i]*(p1[2]+p2[2]);
      }
      pixel[0] = sumr;
      pixel[1] = sumg; 
      pixel[2] = sumb;
      dst->at<cv::Vec3b>(x, y) = pixel;
    }
  }
  
}

// felzenswalb isnt using a standard gaussian filter
cv::Mat *smooth_image(cv::Mat *src, float sigma) {
  int width = src->size().width, height = src->size().height;
  // create mask
  sigma = std::max(sigma,0.01F);
  int WIDTH = 1;
  int len = (int)ceil(sigma*WIDTH) + 1;
  std::vector<float> mask(len);
  for (int i=0; i<len; i++) {
    mask[i] = exp(-0.5*square(i/sigma));
  }
  // normalise mask so it integrates to 1
  float sum = 0.0;
  for (int i=1; i<len; i++)
    sum += fabs(mask[i]);
  sum = 2*sum + fabs(mask[0]);
  for (int i=0; i<len; i++)
    mask[i] /= sum;
  // perform filtering
  cv::Mat *tmp = new cv::Mat(width, height, CV_8UC3, cv::Scalar(0,0,0));
  cv::Mat *dst = new cv::Mat(height, width, CV_8UC3, cv::Scalar(0,0,0));
  convolve_image(src, tmp, mask);
  convolve_image(tmp, dst, mask);
  // return result
  delete tmp;
  return dst;
}

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for threshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
cv::Mat *segment_image(cv::Mat *im, float sigma, float c, int min_size,
			  int *num_ccs) {
  int width = im->size().width;
  int height = im->size().height;

  // produce a smoothed version of the original image
  cv::Mat *smooth = smooth_image(im, sigma);
 
  // build graph
  edge *edges = new edge[width*height*4];
  int num = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (x < width-1) {
        edges[num].a = y * width + x;
        edges[num].b = y * width + (x+1);
        edges[num].w = diff(smooth, x, y, x+1, y);
        num++;
      }

      if (y < height-1) {
        edges[num].a = y * width + x;
        edges[num].b = (y+1) * width + x;
        edges[num].w = diff(smooth, x, y, x, y+1);
        num++;
      }

      if ((x < width-1) && (y < height-1)) {
        edges[num].a = y * width + x;
        edges[num].b = (y+1) * width + (x+1);
        edges[num].w = diff(smooth, x, y, x+1, y+1);
        num++;
      }

      if ((x < width-1) && (y > 0)) {
        edges[num].a = y * width + x;
        edges[num].b = (y-1) * width + (x+1);
        edges[num].w = diff(smooth, x, y, x+1, y-1);
        num++;
      }
    }
  }
  delete smooth;

  // segment
  universe *u = segment_graph(width*height, num, edges, c);
  
  // post process small components
  for (int i = 0; i < num; i++) {
    int a = u->find(edges[i].a);
    int b = u->find(edges[i].b);
    if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
      u->join(a, b);
  }
  delete [] edges;
  *num_ccs = u->num_sets();

  cv::Mat *output = new cv::Mat(height, width, CV_8UC3, cv::Scalar(0,0,0));
  // pick random colors for each component
  cv::Vec3b *colors = new cv::Vec3b[width*height];
  for (int i = 0; i < width*height; i++)
    colors[i] = random_rgb();
  
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int comp = u->find(y * width + x);
      cv::Vec3b pixel = output->at<cv::Vec3b>(y, x);
      pixel[0] = colors[comp][0];
      pixel[1] = colors[comp][1];
      pixel[2] = colors[comp][2];
      output->at<cv::Vec3b>(y, x) = pixel;
    }
  }  

  delete [] colors;  
  delete u;

  return output;
}

#endif
