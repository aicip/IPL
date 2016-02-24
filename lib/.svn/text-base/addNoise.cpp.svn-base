/**********************************************************
 * addNoise.cpp - add noise of different distributions
 *                to the image
 *
 *   - gaussianNoise: add Gaussian random noise
 *   - sapNoise: add salt and pepper noise
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   - 02/12/06: gaussianNoise - move "mean" to the end of
 *               the parameter list to take default value.
 *   - 02/12/06: each pixel is added with a random number
 *               instead of a fixed number all across the
 *               image plane (bug found by Tom Karnowski)            
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>


/**
 * Add Gaussian noise to the image
 * @param inimg The input image
 * @param mean The mean value of the noise
 * @param std The standard deviation of the noise
 * @return Image corrupted by Gaussian noise
 */
Image gaussianNoise(Image &inimg, float std, float mean) {
  Image outimg;
  int nr, nc, nt, nchan;
  int i, j, k;

  // get the dimension
  nc = inimg.getCol();
  nr = inimg.getRow();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  outimg.createImage(nr, nc, nt);
  
  srand(time(NULL));         // randomize the seed

  // modified on 02/12/06 
  // the original implementation added the same value
  // to all pixels in the image
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++) 
        outimg(i,j,k) = inimg(i,j,k) + mean + std * gaussrand();
  
  return outimg;
}


/**
 * Add salt-and-pepper noise to an image
 * @param inimg The input image.
 * @param q The probability. 0<q<1. 
 * For each pixel in the image, generate a random number, say r. 
 *   If r<q, change the pixel's intensity to zero. 
 *   If r>1-q, change the pixel's intensity to L
 * The higher the q, the worse the noise
 * @return Image corrupted by salt and pepper noise.
 */
Image sapNoise(Image &inimg, float q) {
  Image outimg;
  int i, j, k;
  int nc, nr, ntype, nchan;
  float r;

  // get the dimension
  nc = inimg.getCol();
  nr = inimg.getRow();
  ntype = inimg.getType();
  nchan = inimg.getChannel();
    
  // allocate memory for the negative image
  outimg.createImage(nr, nc, ntype);

  // add SAP noise
  srand(time(0));         // so that a different seed nr is generated
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) 
      for (k=0; k<nchan; k++) {
        r = rand()/(RAND_MAX+1.0);
        outimg(i,j,k) = inimg(i,j,k);
        if (r < q)
          outimg(i,j,k) = 0;
        if (r > 1-q)
          outimg(i,j,k) = L;
      }

  return outimg;
}
