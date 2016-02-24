/**********************************************************
 * canny.cpp - Implement the Canny edge detector
 *
 *   - canny: the edge detector
 *   - nonmax: nonmax suppression 
 *   - hthreshold: hysteresis thresholding
 *   - estThreshold: estimate thresholds
 *   - DoGX: DoG kernel in the horizontal direction
 *   - DoGY: DoG kernel in the vertical direction
 *   - gaussianKernel: the Gaussian smooth kernel
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   02/02/06 - nonmax suppression (the assignment of a for
 *              edge direction between 90 and 135 degree 
 *              is wrong) (problem found by Lidan Miao)
 *   02/02/06 - hysteresis thresholding (the original design
 *              included too many extra edges; currently, a
 *              pixel is set to edge pixel only if its 
 *              intensity is higher than the low threshold 
 *              and it's adjacent to a connected edge.
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>

using namespace std;
 
 
/** 
 * Canny edge detector.
 * @param inimg The input image
 * @param sigma The standard deviation
 * @return The edge image by Canny filter
 */
Image canny(Image &inimg, float sigma) {
  Image G, DoGx, DoGy, I, Ix, Iy, thinEdge;

  // Create Gaussian kernel G, and seperate DoG 
  G = gaussianKernel(sigma);
  DoGx = DoGX(sigma);
  DoGy = DoGY(sigma);

  // first smooth the input image with a Gaussian kernel
  I = conv(inimg, G);

  // then take the derivative
  Ix = conv(I, DoGx);
  Iy = conv(I, DoGy);
  
  // nonmaximum suppression (generate 1-pixel edge)
  thinEdge = nonmax(Ix, Iy);

  // hysteresis thresholding (double threshold)
  thinEdge = hThreshold(thinEdge);

  return thinEdge;
}


/** 
 * Suppress thicker edge to make it 1-pixel wide
 * The magnitude of the gradient at an edge pixel should be
 * greater than the magnitude of teh gradient of the pixels on each side
 * of the edge. 
 * @param imgx Edge image in the horizontal direction
 * @param imgy Edge image in the vertical direction
 * @return Edge suppressed image.
 */
Image nonmax(Image &imgx, Image &imgy) {
  float ratio, x, y, g, o1, o2, a1, a2;
  int nr, nc, i, j;
  Image edge;

  nr = imgx.getRow();
  nc = imgx.getCol();
  edge.createImage(nr, nc);

  for (i=1; i<nr-1; i++)
    for (j=1; j<nc-1; j++) {
      x = imgx(i,j);
      y = imgy(i,j);
      g = norm(x, y);        

      // gradient direction is roughly vertical or edge direction is horizontal 
      if (fabs(y) > fabs(x)) {
        ratio = fabs(x) / fabs(y);
        // the two pixels "across" edge direction
        o1 = norm(imgx(i-1,j), imgy(i-1,j));
        o2 = norm(imgx(i+1,j), imgy(i+1,j));
        // when the edge is not strictly horizontal, but tilted with an angle
        // we also find those neighbors along the crossing direction
        if (x*y > 0.0) {   // edge with a positive slope
          a1 = norm(imgx(i-1,j-1), imgy(i-1,j-1));
          a2 = norm(imgx(i+1,j+1), imgy(i+1,j+1));
        } else {           // edge with a negative slope
          a1 = norm(imgx(i-1,j+1), imgy(i-1,j+1));
          a2 = norm(imgx(i+1,j-1), imgy(i+1,j-1));
        }
      } else {        // edge direction is basically vertical
        ratio = fabs(y) / fabs(x);
        o1 = norm(imgx(i,j-1), imgy(i,j-1));
        o2 = norm(imgx(i,j+1), imgy(i,j+1));
        if (x*y > 0.0) {
          a1 = norm(imgx(i-1,j-1), imgy(i-1,j-1));
          a2 = norm(imgx(i+1,j+1), imgy(i+1,j+1));
        } else {
          a1 = norm(imgx(i+1,j-1), imgy(i+1,j-1));   // modified on 02/02/06
          a2 = norm(imgx(i-1,j+1), imgy(i-1,j+1));   // modified on 02/02/06
        }
      }

      // compare the current pixel mag with interpolated two neighbor mag
      // use interpolation to more accurately estimate the edge direction
      if ((g > (ratio*o1+(1-ratio)*a1)) && (g > (ratio*o2+(1-ratio)*a2))) {
        if (g > L)
          edge(i,j) = L;
        else       
          edge(i,j) = g; 
      }
      else {
        edge(i,j) = 0.0;
      }
    }

  return(edge);
}


/** 
 * Hysteresis thresholding. Fine tune the edge image. 
 * For each edge with a magnitude above the high threshold, 
 * begin tracing edge pixels that are above the low threshold. 
 * @param inimg The input image.
 * @return Fined tuned edge image.
 */
Image hThreshold(Image &inimg) {
  int high, low, i, j, m, n;
  int nr, nc;
  Image edge;
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  edge.createImage(nr, nc);
  
  estThreshold(inimg, &high, &low);
  
  // modified on 02/02/06
  // original design: when current pixel is greater than the higher threshold,
  // set it to white and if its 8 neighbors are greater than the lower 
  // threshold, set them to white as well; if the current pixel is greater than
  // the lower threshold, set its preceding neighbors to white if they are
  // greater than the lower threshold.
  // well... I don't remember why I programmed like that, that's not really
  // hysteresis thresholding. Following is the corrected one.
  for (i=1; i<nr-1; i++)
    for (j=1; j<nc-1; j++) { 
      if (inimg(i,j) >= high) {        // if greater than "high"
	    edge(i,j) = L;                 // keep this pixel in the edge image	          
      } else if (inimg(i,j)<=low) {    // if less than "low", discard
	    edge(i,j) = 0.0;
      } else {                         // if between "low" and "high"
      	for (m=-1; m<=0; m++)          // look for backward neighbors
      	  for (n=-1; n<=1; n++)
      	    if (edge(i+m,j+n) == L) {  // if any of them are edge pixel 
      	      edge(i,j) = L;           // set the current one as edge
      	      break;
      	    }
      }                               
    }

  return(edge);
}


/** 
 * Estimate the appropriate high and low thresholds. 
 * @param inimg The input image.
 * @param high The pointer to the high threshold
 * @param low The pointer to the low threshold
 */
void estThreshold(Image &inimg, int *high, int *low) {
  int i, j, *hist, count, pixels;
  int nr, nc;

  nr = inimg.getRow();
  nc = inimg.getCol();
  
  hist = (int *) new int [(int)L+1];
  for (i=0; i<=L; i++)
    hist[i] = 0;
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      hist[(int)(inimg(i,j))]++;

  pixels = nr*nc-hist[0]-hist[(int)L];

  // High threshold is determined. 70% of the total pixels.
  i = (int)L-1;
  count = hist[i];
  while (count < 0.7*pixels) {
    i--;
    if (i < 1)
      break;
    count += hist[i];
  }
  *high = i;
  *low = (int)(*high / 2.0);
}


/**
 * DoGX - Difference of Gaussian in the horizontal direction
 * @param sigma Standard deviation of Gaussian
 * @return The DoG kernel in the horizontal direction
 */
Image DoGX(float sigma) {
  Image kernel;
  int i, j;
  int radius;

  // radius of the kernel in the vertical and the horizontal direction
  radius = (int)(3 * sqrt(2.0) * sigma);  // row = 2*radius+1
  kernel.createImage(2*radius+1, 2*radius+1);

  // calculate first derivative of Gaussian
  for (i=-radius; i<=radius; i++) {
    for (j=-radius; j<radius; j++) {
      kernel(i+radius,j+radius) = 
          -j/(2*PI*pow((double)sigma,4.0)) * exp(-(i*i+j*j)/(2*sigma*sigma));
    }
  }

  return kernel;
}


/**
 * DoGY - Difference of Gaussian in the vertical direction
 * @param sigma Standard deviation of Gaussian
 * @return The DoG kernel in the vertical direction
 */
Image DoGY(float sigma) {
  Image kernel;
  int i, j;
  int radius;

  // radius of the kernel in the vertical and the horizontal direction
  radius = (int)(3 * sqrt(2.0) * sigma);  // row = 2*radius+1
  kernel.createImage(2*radius+1, 2*radius+1);

  // calculate first derivative of Gaussian
  for (i=-radius; i<=radius; i++) {
    for (j=-radius; j<radius; j++) {
      kernel(i+radius,j+radius) = 
          -i/(2*PI*pow((double)sigma,4.0)) * exp(-(i*i+j*j)/(2*sigma*sigma));
    }
  }

  return kernel;
}


/**
 * Generate the Gaussian smooth kernel
 * @param Standard deviation of Gaussian
 * @return The Gaussian kernel
 */
Image gaussianKernel(float sigma) {
  Image kernel;
  int i, j;
  int radius;

  radius = (int)(3.0 * sqrt(2.0) * sigma);
  kernel.createImage(2*radius+1, 2*radius+1);

  for (i=-radius; i<=radius; i++) {
    for (j=-radius; j<=radius; j++) {
      kernel(i+radius,j+radius) = 
          1/(2*PI*sigma*sigma) * exp(-(i*i+j*j)/(2*sigma*sigma));
    }
  }

  return kernel;
}
