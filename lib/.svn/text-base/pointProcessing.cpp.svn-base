/***************************************************************
 * pointProcessing.cpp - contains point-based image enhancement
 *                    algorithms
 *
 *   - negative: image negative
 *   - cs: contrast stretching
 *   - logtran: log transformation
 *   - powerlaw: power-law transformation
 *   - threshold: thresholding an image using either binary or 
 *                gray-level thresholding
 *   - bitplane: bitplane slicing
 *   - sampling: downsample the image
 *   - quantization: quantize the image
 *   - histeq: histogram equalization
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   11/07/11 - modify histeq() for better performance efficiency
 *   07/31/09 - move rescale() to imageIO.cpp
 *   02/06/06 - Chris reported problem with gcc4 when using
 *              new (float *). Removed ().
 ***************************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cmath>

using namespace std;

/**
 * Image negative.  s = L - r where
 * L: the largest intensity in the image,
 * s: the enhanced pixel intensity, and
 * r: the original pixel intensity.
 * @para inimg Input image
 * @return Negative of the input image
 */
Image negative(Image &inimg) {
  Image outimg;
  int i, j, k;
  int nc, nr, ntype, nchan;
  Image maxi;

  nc = inimg.getCol();
  nr = inimg.getRow();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  // allocate memory for the negative image
  outimg.createImage(nr, nc, ntype);

  // find the negative
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        outimg(i,j,k) = L - inimg(i,j,k);

  return outimg;
}


/**
 * Contrast stretching. s = m*r + b where
 * s: enhanced pixel intensity
 * r: original pixel intensity
 * @param inimg Input image
 * @param m Slope
 * @param b Intercept
 * @return Contrast stretched image.
 */
Image cs(Image &inimg, float m, float b) {
  Image outimg;
  int i, j, k;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);

  // perform contrast stretching
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        outimg(i,j,k) = m * inimg(i,j,k) + b;

  return outimg;
}


/**
 * Log transformation for dynamic range compression. s = log(1+|r|)
 * @param inimg Input image
 * @return Image with dynamic range compressed
 */
Image logtran(Image &inimg) {
  Image outimg;
  int i, j, k;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        outimg(i,j,k) = log(1.0+fabs(inimg(i,j,k)));

  return outimg;
}


/**
 * Power-law transformation. s = r^gamma
 * @param inimg Input image
 * @param gamma When gamma<1, compress the high intensity content; When gamm>1,
 * compress the low intensity content.
 * @return Image corrected by power-law transformation
 */
Image powerlaw(Image &inimg, float gamma) {
  Image outimg;
  int i, j, k;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        outimg(i,j,k) = pow(inimg(i,j,k), gamma);

  return outimg;
}


/**
 * Thresholding image to a gray-level or a binary image
 * @param inimg The input image.
 * @param thresh The threshold.
 * @param nt The type of the resulting image. When nt==BINARY, using binary
 * thresholding; when nt==GRAY, using gray-level thresholding.
 * @return The thresholded image.
 */
Image threshold(Image &inimg, float thresh, int nt) {
  Image outimg;
  int i, j, k;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);

  if (nt != GRAY && nt != BINARY) 
    nt = BINARY;

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        if (inimg(i,j,k) >= thresh)
          outimg(i,j,k) = (nt == GRAY) ? inimg(i,j,k) : L;
        else
          outimg(i,j,k) = 0;

  return outimg;
}

/**
 * Thresholding image to a gray-level or a binary image between certain range
 * @param inimg The input image.
 * @param lthresh The lower threshold.
 * @param uthresh The upper threshold.
 * @param nt The type of the resulting image. When nt==BINARY, using binary
 * thresholding; when nt==GRAY, using gray-level thresholding.
 * @return The thresholded image.
 */
Image threshold(Image &inimg, float lthresh, float uthresh, int nt) {
  Image outimg;
  int i, j;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "threshold: can only handle single-channel image.\n";
    exit(3);
  }
  outimg.createImage(nr, nc, ntype);

  if (nt != GRAY && nt != BINARY) 
    nt = BINARY;

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      if (inimg(i,j) >= lthresh && inimg(i,j) <= uthresh)
          outimg(i,j) = (nt == GRAY) ? inimg(i,j) : L;
        else
          outimg(i,j) = 0;

  return outimg;
}


/**
 * Automatic thresholding using Otsu's method
 * Qi (10/06/07) revision:
 *   - couldn't remember who wrote the first version of this code
 *   - instead of returning the thresholded image, it returns the optimal threshold
 * @param inimg The input image.
 * @return The thresholded image and the threshold automatically determined.
 */
Image autoThreshold(Image &inimg) {
  Image temp;
  float maxi, mini;
  float *hist, *chist, *ihist;  // histogram, cumulative histogram, and
                                // intensity cumulative histogram
  int i, j;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  if (nchan > 1) {
    cout << "autoThreshold: Can only process gray-scale images.\n";
    exit(3);
  }

  // check the range of the pixel intensity. rescale if out-of-range
  maxi = inimg.getMaximum();    // wonder if we should just make
  mini = inimg.getMinimum();    // getMaximum() return a scalar
  if (mini<0 || maxi>L) {
    cout << "autoThreshold: "
         << "The intensity value is outside [0, " << L << "], rescale.\n";
    temp = rescale(inimg);
  }
  else
    temp = inimg;
         
  // initialize the histogram and the cumulative histogram arrays
  hist = (float *) new float [(int)L+1];
  chist = (float *) new float [(int)L+1];
  ihist = (float *) new float [(int)L+1];
  for (i=0; i<=L; i++) {
    hist[i] = 0;
    chist[i] = 0;
    ihist[i] = 0;
  }

  // get the histogram from the scaled inimg
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) 
      hist[(int)temp(i,j)]++;

  // calculate the cumulative histogram
  chist[0] = hist[0];
  for (i=1; i<=L; i++) {
    chist[i] += chist[i-1] + hist[i];
    ihist[i] += ihist[i-1] + hist[i] * i;
  }
  
  // the Otsu's automatic thresholding method
  // find the average of intensity
  float avg, avgL, avgR, avgNew = 0;
  
  avg = chist[(int)L] / ihist[(int)L];
  i = 0;
  while (avg != avgNew) {
    avg = avgNew;
    avgL = ihist[(int)avg] / chist[(int)avg];
    avgR = (ihist[(int)L]-ihist[(int)avg]) / (chist[(int)L]-chist[(int)avg]);
    avgNew = (avgL + avgR) / 2;
    i++;
  }
  cout << "i=" << i << endl;
  cout << "avg=" << avg << endl;
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      if (temp(i,j) >= avg)
	temp(i,j) = 255.0;
      else
	temp(i,j) = 0;  

  return temp;
}


/**
 * Bitplane slicing. It takes an input image and creates 8 binary images
 * with each image storing information of a certain bit. 
 * The function assumes an 8-bit b/w input image.
 * @param inimg Input 8-bit b/w image.
 * @param outimg An array of 8 binary images.
 */
void bitplane(Image &inimg, Image outimg[NBIT]) {
  int nr, nc, nchan, nt;
  int i, j, k;

  // get the dimension
  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan == 3) {
    cout << "bitplane: " 
         << "This program only handles 8-bit gray-scale images.\n";
    exit(3);
  }
      
  // go through the whole image and process each pixel
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<NBIT; k++)    // for each pixel, process each bit
        outimg[k](i,j) = ((int)inimg(i,j) & (1 << k)) ? L : 0;
}


/**
 * Downsample the image by s without changing the image size.
 * @param inimg The input image.
 * @param s The downsample ratio (s > 1 and s is integer)
 * @return The downsampled image.
 */
Image sampling(Image &inimg, int s) {
  Image outimg;
  int i, j, k;
  int nr, nc, ntype, nchan;

  // check the range of s
  if (s < 1) {
  	cout << "sampling: Cannot process downsample rate less than 1\n.";
  	exit(3);
  }
  
  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        outimg(i,j,k) = inimg(s*(i/s),s*(j/s),k);

  return outimg;
}


/**
 * Quantize the original L-level image to a q-level image.
 * @param inimg The input image.
 * @param q The number of quantization levels
 * @return The quantized image
 */
Image quantization(Image &inimg, int q) {
  Image outimg;
  int i, j, k;
  int nr, nc, ntype, nchan;

  // allocate memory
  nr = inimg.getRow();
  nc = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        outimg(i,j,k) = (int) (inimg(i,j,k) / ((L+1)/q));

  return outimg;
}


/**
 * Histogram equalization
 * @param inimg The input image 
 * @return The histogram equalized image. 
 */
Image histeq(Image &inimg) {
  Image outimg, temp;
  int nr, nc, nt, nchan;
  int i, j;
  float maxi, mini;
  float *hist;           // histogram of the original image
    //        *chist;          // accumulated histogram 
  
  // read in original image
  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "histeq: can only handle gray-scale images\n";
    exit(3);
  }

  // create histogram equalized image
  outimg.createImage(nr, nc, nt);

  // check the range of the pixel intensity. rescale if out-of-range
  maxi = inimg.getMaximum();
  mini = inimg.getMinimum();
  if (mini<0 || maxi>L) {
    cout << "histeq: "
         << "The intensity value is outside [0, " << L << "], rescale.\n";
    temp = rescale(inimg);
  }
  else
    temp = inimg;
         
  // initialize the histogram arrays
  hist = (float *) new float [(int)L+1];    // Chris reported problem with gcc4
  //  chist = (float *) new float [(int)L+1];   // when have the () outside float *
  for (i=0; i<=L; i++) {
    hist[i] = 0;
    //    chist[i] = 0;
  }

  // get the histogram from inimg
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      hist[(int)temp(i,j)]++;

  // get the accumulated histogram
  //  chist[0] = hist[0] / (nr*nc);
  for (i=1; i<=L; i++)
    //    chist[i] = chist[i-1] + hist[i]/(nr*nc); 
    hist[i] = hist[i-1] + hist[i]/(nr*nc); 
  
  // equalize the histogram of the original image
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      //      outimg(i,j) = chist[(int)temp(i,j)];
      outimg(i,j) = hist[(int)temp(i,j)] * L;

  return outimg;
}


