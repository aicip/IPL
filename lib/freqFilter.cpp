/***********************************************************
 * freqFilter.cpp - perform frequency domain filtering
 *
 *   - ideal: the ideal highpass/lowpass filter
 *   - butterworth: the butterworth highpass/lowpass filter
 *   - gaussianFreq: the Gaussian highpass/lowpass filter
 *   - inverseFilter: the inverse filter
 *   - wiener: the Wiener filter
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 ***********************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cmath>

using namespace std;


/**
 * Ideal highpass/lowpass filter.
 * @param inimg The input image.
 * @param flag The flag that controls either lowpass or highpass operation.
 * @see HIGH.
 * @see LOW.
 * @param radius The radius of the filter. 
 * @return The filtered image.
 */
Image ideal(Image &inimg, int flag, float radius) {
  Image outimg, filter, mag, phase;
  int nr, nc;
  double dist;

  nr = inimg.getRow();
  nc = inimg.getCol();

  // generate the ideal filter
  filter.createImage(nr, nc);

  for (int i=0; i<nr; i++)
    for (int j=0; j<nc; j++) {
      dist = sqrt((float)(i-nr/2)*(i-nr/2)+(j-nc/2)*(j-nc/2));
      if (flag == HIGH)                             // highpass ideal
	filter(i,j) = ( dist > radius ) ? 1 : 0;
      else                                          // lowpass ideal
	filter(i,j) = ( dist <= radius ) ? 1 : 0;
    }

  // FFT of the input image
  mag.createImage(nr, nc);
  phase.createImage(nr, nc);
  fft(inimg, mag, phase);

  // apply the ideal filter
  mag = mag * filter;

  // inverse FFT
  outimg.createImage(nr, nc);
  ifft(outimg, mag, phase);
  
  return outimg;
}


/**
 * Butterworth highpass/lowpass filter.
 * @param inimg The input image.
 * @param flag The flag that controls either lowpass or highpass operation.
 * @see HIGH.
 * @see LOW.
 * @param radius The radius of the filter. 
 * @param n The exponent that controls the sharpness of the filter.
 * @return The filtered image.
 */
Image butterworth(Image &inimg, int flag, float radius, int n) {
  Image outimg, filter, mag, phase;
  int nr, nc;
  double dist;

  nr = inimg.getRow();
  nc = inimg.getCol();

  // generate the butterworth filter
  filter.createImage(nr, nc);

  for (int i=0; i<nr; i++)
    for (int j=0; j<nc; j++) {
      dist = sqrt((float)(i-nr/2)*(i-nr/2)+(j-nc/2)*(j-nc/2));
      if (flag == HIGH)                             // highpass butterworth
	filter(i,j) = 1 / (1 + pow(radius/dist, (double)2*n));
      else                                          // lowpass butterworth
	filter(i,j) = 1 / (1 + pow(dist/radius, (double)2*n));
    }

  // FFT of the input image
  mag.createImage(nr, nc);
  phase.createImage(nr, nc);
  fft(inimg, mag, phase);

  // apply the ideal filter
  mag = mag * filter;

  // inverse FFT
  outimg.createImage(nr, nc);
  ifft(outimg, mag, phase);
  
  return outimg;
}


/**
 * Gaussian highpass/lowpass filter.
 * @param inimg The input image.
 * @param flag The flag that controls either lowpass or highpass operation.
 * @see HIGH.
 * @see LOW.
 * @param radius The radius of the filter. 
 * @return The filtered image.
 */
Image gaussianFreq(Image &inimg, int flag, float std) {
  Image outimg, filter, mag, phase;
  int nr, nc;
  double dist;

  nr = inimg.getRow();
  nc = inimg.getCol();

  // generate the Gaussian filter
  filter.createImage(nr, nc);

  for (int i=0; i<nr; i++)
    for (int j=0; j<nc; j++) {
      dist = sqrt((float)(i-nr/2)*(i-nr/2)+(j-nc/2)*(j-nc/2));
      if (flag == HIGH)                             // highpass butterworth
	filter(i,j) = 1 - exp(-dist*dist/(2*std*std));
      else                                          // lowpass butterworth
	filter(i,j) = exp(-dist*dist/(2*std*std));
    }

  // FFT of the input image
  mag.createImage(nr, nc);
  phase.createImage(nr, nc);
  fft(inimg, mag, phase);

  // apply the ideal filter
  mag = mag * filter;

  // inverse FFT
  outimg.createImage(nr, nc);
  ifft(outimg, mag, phase);
  
  return outimg;
}


/**
 * Wiener filter for deblurring assuming Gaussian blur
 * @param inimg The input image.
 * @param K The ratio between the power of noise and power of signal.
 * @param std The standard deviation of the Gaussian blur kernel.
 * @return The filtered image.
 */
Image wiener(Image &inimg, float K, float std) {
  Image outimg, filter, mag, phase;
  int nr, nc;
  double dist, blur, blur2;

  nr = inimg.getRow();
  nc = inimg.getCol();

  // generate the Wiener filter
  filter.createImage(nr, nc);

  for (int i=0; i<nr; i++)
    for (int j=0; j<nc; j++) {
      dist = (float)((i-nr/2)*(i-nr/2)+(j-nc/2)*(j-nc/2));
      blur = exp(-dist/(2*std*std));
      blur2 = blur * blur;
      filter(i,j) = (1.0/blur) * (blur2/(blur2+K));
    }

  // FFT of the input image
  mag.createImage(nr, nc);
  phase.createImage(nr, nc);
  fft(inimg, mag, phase);

  // apply the Wiener filter
  mag = mag * filter;

  // inverse FFT
  outimg.createImage(nr, nc);
  ifft(outimg, mag, phase);
  
  return outimg;
}


/**
 * The inverse filter for deblurring assuming Gaussian blur
 * @param inimg The input image.
 * @return The filtered image.
 */
Image inverseFilter(Image &inimg, float std) {
  Image outimg, filter, mag, phase;
  int nr, nc;
  double dist, blur;

  nr = inimg.getRow();
  nc = inimg.getCol();

  // generate the Wiener filter
  filter.createImage(nr, nc);

  for (int i=0; i<nr; i++)
    for (int j=0; j<nc; j++) {
      dist = (float)((i-nr/2)*(i-nr/2)+(j-nc/2)*(j-nc/2));
      blur = exp(-dist/(2*std*std));
      filter(i,j) = 1.0/blur;
    }

  // FFT of the input image
  mag.createImage(nr, nc);
  phase.createImage(nr, nc);
  fft(inimg, mag, phase);

  // apply the Wiener filter
  mag = mag * filter;

  // inverse FFT
  outimg.createImage(nr, nc);
  ifft(outimg, mag, phase);
  
  return outimg;
}
