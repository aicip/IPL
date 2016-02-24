/**********************************************************
 * utility.cpp - contains utility functions used by 
 *               various DIP algorithms
 *
 *   - bubblesort: a sorting algorithm
 *   - gaussrand: Gaussian random number generator
 *   - psnr: peak signal-to-noise ratio
 *   - rmse: root mean square error
 *   - power: power of an image
 *   - sqrt: square root of an image (pixel wise)
 *   - abs: absolute value of an image (pixel wise)
 *   - norm: sqrt of summation
 *   - sum: summation of pixel intensity
 *   - var: variance of image
 *   - outofMemory: print error message
 *   - floorPower2: the largest power of 2 that's <= the given value
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   - 02/12/06 - add abs()
 *   - 02/12/06 - add sum()
 *   - 11/04/08 - add var(), rmse()
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

 
/**
 * Find the largest power of 2 that's less than or equal to the given value.
 * @return The integer generated based on the criterion.
 */
int floorPower2(int x) {
  int count;

  count = 0;            
  while (x >= 2) {
    if (x%2 == 0) {
      x /= 2;
      count++;
    }
    else  
      x--;
  }

  return (int)pow(2.0,(double)count);
}


/**
 * Bubblesort algorithm. Sort an array in the ascending order.
 * @param p Pointer to the array to be sorted.
 * @param size Size of the array
 */
void bubblesort(float *p, int size) {
  int change, m, n;
  float swap;

  change = 1;

  for (m=0; m<size-1 && change; m++) {
    change = 0;       // detect any change orders in the sorting process
    for (n=0; n<size-1; n++)
      if (p[n] > p[n+1]) {
        swap = p[n];            // swap the value
        p[n] = p[n+1];
        p[n+1] = swap;
        change = 1;
      }
  }
}
 

/**
 * Bubblesort algorithm. Sort an array in the ascending order.
 * @param p Pointer to the array to be sorted.
 * @param pi Pointer to the index of the sorted array
 * @param size Size of the array
 */
void bubblesort(float *p, int size, int *pi) {
  int change, m, n;
  float swap;

  change = 1;

  for (m=0; m<size-1 && change; m++) {
    change = 0;       // detect any change orders in the sorting process
    for (n=0; n<size-1; n++)
      if (p[n] > p[n+1]) {
        swap = p[n];            // swap the value
        p[n] = p[n+1];
        p[n+1] = swap;
        swap = pi[n];         // swap the index
        pi[n] = pi[n+1];
        pi[n+1] = (int)swap;
        change = 1;
      }
  }
}


/**
 * Box-Muller method to generate a Gaussian random number
 * @return The random number between -3 and 3
 */
double gaussrand() {
  static double V1, V2, S;
  static int phase = 0;
  double X;

  if (phase == 0) {
    do {
      double U1 = (double)rand() / RAND_MAX;
      double U2 = (double)rand() / RAND_MAX;
      V1 = 2 * U1 - 1;
      V2 = 2 * U2 - 1;
      S = V1 * V1 + V2 * V2;
    } while(S >= 1 || S == 0);

    X = V1 * sqrt(-2 * log(S) / S);
  }
  else
    X = V2 * sqrt(-2 * log(S) / S);

  phase = 1 - phase;

  return X;
}


/**
 * Calculate peak signal to noise ratio (PSNR).  
 * psnr = 10log[(L-1)/RMSE] where L is the largest intensity represented
 * @param inimg1 The original image.
 * @param inimg2 The degraded image.
 * @return PSNR
 */
float psnr(Image &inimg1, Image &inimg2) {
  int i, j;
  int nc, nr, nchan;

  // get the dimension
  nc = inimg1.getCol();
  nr = inimg1.getRow();
  nchan = inimg1.getChannel();
  if (nchan == 3) {
    cout << "psnr: " 
         << "This program does not handle color image.\n";
    exit(3);
  }
  if (nr != inimg2.getRow() || nc != inimg2.getCol()) {
    cout << "psnr: "
	 << "The two input images do not have the same dimensions.\n";
    exit(3);
  }
    
  return 10 * log10((L-1)/rmse(inimg1, inimg2));
}


/**
 * Calculate root mean square error (RMSE).  
 * @param inimg1 The original image.
 * @param inimg2 The degraded image.
 * @return RMSE
 */
float rmse(Image &inimg1, Image &inimg2) {
  int i, j;
  int nc, nr, nchan;
  float sum;

  // get the dimension
  nc = inimg1.getCol();
  nr = inimg1.getRow();
  nchan = inimg1.getChannel();
  if (nchan > 1) {
    cout << "rmse: " 
         << "This program does not handle color image.\n";
    exit(3);
  }
  if (nr != inimg2.getRow() || nc != inimg2.getCol()) {
    cout << "rmse: "
	 << "The two input images do not have the same dimensions.\n";
    exit(3);
  }
    
  // calculate root mean square error (RMSE)
  sum = 0;
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) 
      sum += (inimg1(i,j) - inimg2(i,j)) * (inimg1(i,j) - inimg2(i,j));

  return sqrt(sum/(nr*nc));
}


/**
 * Power of an image.
 * @param inimg The input image.
 * @return The power of the image.
 */
float power(Image &inimg) {
  int i, j, k;
  float p = 0.0;

  for (i=0; i<inimg.getRow(); i++)
    for (j=0; j<inimg.getCol(); j++)
      for (k=0; k<inimg.getChannel(); k++)
	    p += inimg(i,j,k) * inimg(i,j,k);

  return p / inimg.getChannel();
}


/**
 * Square root of an image.
 * @param inimg The input image.
 * @return The square root of the image.
 */
Image sqrt(Image &inimg) {
  int i, j, k;
  Image outimg;
  
  outimg.createImage(inimg.getRow(), inimg.getCol(), inimg.getType());

  for (i=0; i<inimg.getRow(); i++)
    for (j=0; j<inimg.getCol(); j++)
      for (k=0; k<inimg.getChannel(); k++)
	     outimg(i,j,k) = sqrt(inimg(i,j,k));

  return outimg;
}


/**
 * 2-norm 
 * @param x
 * @param y
 * @return sqrt(x*x+y*y)
 */
float norm(float x, float y) {
  return (sqrt(x*x+y*y));
}


/**
 * Output out of memory error.
 */
void outofMemory() {
  cout << "outofMemory: Out of memory!\n";
  exit(1);
}


/**
 * Absolute value of an image.
 * @param inimg The input image.
 * @return The absolute of the image.
 */
Image abs(Image &inimg) {
  int i, j, k;
  Image outimg;
  
  outimg.createImage(inimg.getRow(), inimg.getCol(), inimg.getType());

  for (i=0; i<inimg.getRow(); i++)
    for (j=0; j<inimg.getCol(); j++)
      for (k=0; k<inimg.getChannel(); k++)
	     outimg(i,j,k) = fabs(inimg(i,j,k));

  return outimg;
}


/**
 * Summation of all pixel intensities in an image.
 * @param inimg The input image.
 * @return The summation of all pixel intensities.
 */
float sum(Image &inimg) {
  int i, j, k;
  float sum = 0.0;
  
  for (i=0; i<inimg.getRow(); i++)
    for (j=0; j<inimg.getCol(); j++)
      for (k=0; k<inimg.getChannel(); k++)
	     sum += fabs(inimg(i,j,k));

  return sum;
}


