/**********************************************************
 * lowpassFilter.cpp - perform low-pass filtering 
 *
 *   - average: the average filter
 *   - median: the median filter
 *   - contrah: the contraharmonic filter
 *   - gmean: geometric mean
 *   - amedian: adaptive median
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   - 11/04/08: add gmean(), amedian()
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

/**
 * Geometric mean filter.
 * @param inimg The input image.
 * @param size The size of the neighborhood. Assume odd for the simplicity.
 * @return The smoothed image using geometric mean filter.
 */
Image gmean(Image &inimg, int size) {
  Image outimg;
  int nr, nc, nchan, i, j, m, n;
  double sum;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "gmean: Can only handle gray-scale images.\n";
    exit(3);
  }

  outimg.createImage(nr, nc);

  for (i=size/2; i<nr-size/2; i++)
    for (j=size/2; j<nc-size/2; j++) {
      sum = 1.0;
      for (m=-size/2; m<size/2; m++)
	for (n=-size/2; n<size/2; n++)
	  sum *= inimg(i+m,j+n);
      outimg(i,j) = pow(sum, 1.0/(size*size));
    }
  
  return outimg;
}


/**
 * Average lowpass filter.
 * @param inimg The input image.
 * @param size The size of the neighborhood.
 * @return The smoothed image using average filter.
 */
Image average(Image &inimg, int size) {
  Image outimg, mask;

  // create the average mask
  mask.createImage(size, size);
  mask.initImage(1);
  mask = mask / (size * size);

  // apply kernel operation
  outimg = conv(inimg, mask);
      
  return outimg;
}
 

/**
 * Gaussian lowpass filter. Kernel = [1 2 1; 2 4 2; 1 2 1]/16;
 * @param inimg The input image
 * @return The smoothed image.
 */
Image gaussianSmooth(Image &inimg) {
  Image outimg, mask;

  // create the two masks and initialize to zero  
  mask.createImage(3,3);
  mask(0,0) = mask(0,2) = mask(2,0) = mask(2,2) = 1;
  mask(0,1) = mask(1,0) = mask(1,2) = mask(2,1) = 2;
  mask(1,1) = 4;
  mask = mask / 16.0;
  
  // apply kernel operation
  outimg = conv(inimg, mask); 
    
  return outimg;
}

 
/**
 * Median filter
 * @param inimg The input image
 * @param masksize The size (or radius) of the neighborhood 
 * @return Image smoothed by median filter
 */
Image median(Image &inimg, int masksize) {
  Image outimg;
  int nr, nc, nchan, radius1, radius2;
  int i, j, m, n, l;
  float *p;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "Median: Can only handle gray-scale images.\n";
    exit(3);
  }

  outimg.createImage(nr, nc);
  
  // handle odd and even mask size
  if ((float)masksize/2.0 > masksize/2) {
    radius1 = radius2 = masksize/2;
  }
  else {
    radius1 = masksize/2;
    radius2 = radius1 - 1;
  }

  p = (float *) new float [masksize*masksize];

  for (i=radius1; i<nr-radius2; i++)
    for (j=radius1; j<nc-radius2; j++) {
      l = 0;
      for (m=-radius1; m<=radius2; m++)
	for (n=-radius1; n<=radius2; n++)
	  p[l++] = inimg(i+m,j+n);
      bubblesort(p, masksize*masksize);
      outimg(i,j) = p[(int)masksize*masksize/2];
    }

  return outimg;
}

 
/**
 * Adaptive median filter. Handles SAP noise better with prob. larger than 0.2
 * @param inimg The input image
 * @param maxmask The maximum mask size. Assume starts from 3. 
 * @return Image smoothed by the adaptive median filter
 */
Image amedian(Image &inimg, int maxmask) {
  Image outimg;
  int nr, nc, nchan, radius1, radius2, masksize, nsize;
  int i, j, m, n, l, flag;
  float *p, zmin, zmax, zmed, zxy;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "amedian: Can only handle gray-scale images.\n";
    exit(3);
  }

  outimg.createImage(nr, nc);
  
  flag = 1;
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      // for each pixel, start with a masksize equal to 3
      masksize = 3;
      flag = 1;
      while (masksize <= maxmask && flag) {
	// handle odd and even mask size
	if ((float)masksize/2.0 > masksize/2) {
	  radius1 = radius2 = masksize/2;
	}
	else {
	  radius1 = masksize/2;
	  radius2 = radius1 - 1;
	}

	// calculate neighborhood size
	nsize = 0;
	for (m=-radius1; m<=radius2; m++) 
	  for (n=-radius1; n<=radius2; n++) 
	    if (i+m>=0 && j+n>=0 && i+m<nr && j+n<nc) 
	      nsize++;
	p = (float *) new float [nsize];

	// sort the neighbors
	l = 0;
	for (m=-radius1; m<=radius2; m++)
	  for (n=-radius1; n<=radius2; n++)
	    if (i+m>=0 && j+n>=0 && i+m<nr && j+n<nc)
	      p[l++] = inimg(i+m,j+n);
	bubblesort(p, nsize);

	// assign zmed, zmax, and zmin
	zmin = p[0];
	zmax = p[nsize-1];
	zmed = p[(int)nsize/2];
	zxy = inimg(i,j);
	
	// stage A
	if (zmed-zmin>0 && zmed-zmax<0) { // the medium is not impulse
	  // stage B
	  if (zxy-zmin>0 && zxy-zmax<0) { // the current pixel is not impulse
	    outimg(i,j) = zxy;
	    flag = 0;
	  }
	  else {
	    outimg(i,j) = zmed;
	    flag = 0;
	  }
	}
	else {
	  masksize++;
	  delete p;
	}
      }
      if (flag) 
	outimg(i,j) = zmed;
    }

  return outimg;
}
 

/**
 * Adaptive local noise reduction filter.
 * @param inimg The input image
 * @param masksize The radius of the mask. Assume odd for simplicity.
 * @return Image smoothed by the adaptive median filter
 */
/*
Image alnr(Image &inimg, int masksize) {
  Image outimg;
  int nr, nc, nchan;
  int i, j, m, n, l;
  float *p;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "alnr: Can only handle gray-scale images.\n";
    exit(3);
  }

  outimg.createImage(nr, nc);
  
  // calculate the variance of the image

  for (i=masksize/2; i<nr-masksize/2; i++)
    for (j=masksize/2; j<nc-masksize/2; j++) {
      // for each pixel, start with a masksize equal to 3
      masksize = 3;
      while (masksize <= maxmask) {
	// handle odd and even mask size
	if ((float)masksize/2.0 > masksize/2) {
	  radius1 = radius2 = masksize/2;
	}
	else {
	  radius1 = masksize/2;
	  radius2 = radius1 - 1;
	}

	// calculate neighborhood size
	nsize = 0;
	for (m=-radius1; m<=radius2; m++) 
	  for (n=-radius1; n<=radius2; n++) 
	    if (i+m>=0 && j+n>=0 && i+m<nr && j+n<nc) 
	      nsize++;
	p = (float *) new float [nsize];

	// sort the neighbors
	l = 0;
	for (m=-radius1; m<=radius2; m++)
	  for (n=-radius1; n<=radius2; n++)
	    if (i+m>=0 && j+n>=0 && i+m<nr && j+n<nc)
	      p[l++] = inimg(i+m,j+n);
	bubblesort(p, nsize);

	// assign zmed, zmax, and zmin
	zmin = p[0];
	zmax = p[nsize-1];
	zmed = p[(int)nsize/2];
	zxy = inimg(i,j);
	
	// stage A
	if (zmed-zmin>0 && zmed-zmax<0) { // the medium is not impulse
	  // stage B
	  if (zxy-zmin>0 && zxy-zmax<0) { // the current pixel is not impulse
	    outimg(i,j) = zxy;
	  }
	  else
	    outimg(i,j) = zmed;
	}
	else {
	  masksize++;
	  delete p;
      }
    }

  return outimg;
}
*/

/**
 * Contraharmonic mean filter. Well suited for eliminating sap noise.
 * @param inimg The input image.
 * @param Q The order of the filter
 *        when Q=0, it becomes arithmetic mean filter
 *        when Q=-1, it becomes harmonic mean filter
 *        when Q is positive, it erases pepper noise
 *        when Q is negative, it erases salt noise.
 * @param masksize The size of the neighborhood.
 * @return Restored image.
 */
Image contrah(Image &inimg, float Q, int masksize) {
  Image outimg;
  int i, j, m, n;
  int nc, nr, nchan;
  float sumn, sumd;

  // get the dimension
  nc = inimg.getCol();
  nr = inimg.getRow();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "ContraH: Can only handle gray-scale images.\n";
    exit(3);
  }

  outimg.createImage(nr, nc);

  // apply the contraharmonic filter
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      sumn = 0;
      sumd = 0;    
      for (m=-masksize; m<=masksize; m++)
	for (n=-masksize; n<=masksize; n++)
	  if (i+m>=0 && i+m<nr && j+n>=0 && j+n<nc &&
	      masksize+m>=0 && masksize+m<nr && 
	      masksize+n>=0 && masksize+n<nc) {
	    sumn += pow((float)inimg(i+m,j+n), Q+1);
	    sumd += pow((float)inimg(i+m,j+n), Q);
	  }
      outimg(i,j) = sumn / sumd;
    }
    
  return outimg;
}




