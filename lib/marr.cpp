/**********************************************************
 * marr.cpp - Implement the Marr-Hildreth edge detector
 *
 *   - marr: the detector
 *   - LoG: generate the Laplacian of Gaussian kernel
 *   - zeroCrossing: find the zero crossing from the image
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>

using namespace std;
 
/** 
 * Marr-Hildreth edge detector
 * @param inimg The input image.
 * @param std The standard deviation of Gaussian
 * @return The edge image.
 */
Image marr(Image &inimg, float std) {
  Image edge, LoG1, LoG2, conv1, conv2, zc1, zc2;
  int i, j, k;
  int nr, nc, nchan, nt;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();
  edge.createImage(nr, nc, nt);

  // create two kernels: one with std-0.8, another with std+0.8
  // for reliable edge detection
  LoG1 = LoG(std-0.8);
  LoG2 = LoG(std+0.8);

  // convolve LoG with the original image
  conv1 = conv(inimg, LoG1);
  conv2 = conv(inimg, LoG2); 

  // zero crossing
  zc1 = zeroCrossing(conv1);
  zc2 = zeroCrossing(conv2);

  // double check zero crossings
  for (k=0; k<nchan; k++)
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++) {
        if (zc1(i,j,k) && zc2(i,j,k))
	      edge(i,j,k) = L;
        else
	      edge(i,j,k) = 0.0;
      }

  return edge;
}


/**
 * Generate the LoG kernel (Laplacian of Gaussian)
 * @param std The standard deviation of Gaussian
 * @return The LoG kernel.
 */
Image LoG(float std) {
  int size, i, j;
  Image logkernel;
  float tmp;

  // kernel size is 2*size+1
  size = (int)(3 * sqrt(2.0) * std);    

  // create the kernel as an image
  logkernel.createImage(2*size+1, 2*size+1);

  for (i=-size; i<=size; i++) {
    for (j=-size; j<=size; j++) {
      tmp = (i*i + j*j) / (2.0*std*std);
      logkernel(i+size,j+size) = (1/(PI*pow((double)std,4.0))) * 
                                 (tmp-1) * exp(-tmp);
    }
  }

  return logkernel;
}


/**
 * Find zero crossings in the image.
 * @param inimg The input image.
 * @return A binary image with zero crossings as white (or foreground).
 */
Image zeroCrossing(Image &inimg) {
  int i, j, k;
  int nr, nc, nchan, nt;
  Image zc;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();
  zc.createImage(nr, nc, nt);

  for (k=0; k<nchan; k++)
    for (i=1; i<nr-1; i++) {
      for (j=1; j<nc-1; j++) {
        if ((inimg(i,j-1,k)*inimg(i,j+1,k)) < 0 || 
	        (inimg(i-1,j,k)*inimg(i+1,j,k)) < 0 ||
	        (inimg(i-1,j-1,k)*inimg(i+1,j+1,k)) < 0 ||
	        (inimg(i-1,j+1,k)*inimg(i+1,j-1,k)) < 0)
	      zc(i,j,k) = L;
        else
	      zc(i,j,k) = 0.0;
      }
    }

  return zc;
}
