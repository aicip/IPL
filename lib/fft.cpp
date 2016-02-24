/**********************************************************
 * fft.cpp - fast Fourier transform
 *           (can only handle image size of power of 2
 *
 *   - fft: forward FFT
 *   - ifft: inverse FFT
 *   - fftifft: fast Fourier transform
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 10/21/07
 *
 * Modified:
 *   - 10/04/08: fix problems when phase is outside the range
 *               of [-PI, PI], by Timothy Ragland, Fall 2008
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


/**
 * Forward FFT
 * @param inimg The input image to be transformed.
 * @mag The magnitude image from the FFT.
 * @phase The phase image from the FFT.
 */
void fft(Image &inimg, Image &mag, Image &phase)
{
  fftifft(inimg, mag, phase, 1);
}


/**
 * Inverse FFT
 * @param outimg The output image from FFT.
 * @mag The magnitude image.
 * @phase The phase image.
 */
void ifft(Image &outimg, Image &mag, Image &phase)
{
  fftifft(outimg, mag, phase, -1);
}


/**
 * Fast Fourier transform (FFT)
 * @param inimg The output image from FFT.
 * @mag The magnitude image.
 * @phase The phase image.
 * @scale 1 for forward transform and -1 for inverse transform
 */
void fftifft(Image &inimg, Image &mag, Image &phase, int scale)
{
  int N, M, N2;
  int nr, nc;
  int i, j, k, m, n, p, q, sign, index;
  float *realv, *imagv;
  float *real, *imag;            // the real and imaginary part of the fft
  float temp1, temp2, theta;
  int *loc;                      // the reordered index

  nr = inimg.getRow();
  nc = inimg.getCol();

  // allocate memory for real and imag
  real = (float *) new float [nr * nc];
  if (!real) {
    cout << "FFTIFFT: Out of memory.\n";
    exit(1);
  }
  imag = (float *) new float [nr * nc];
  if (!imag) { 
    cout << "FFTIFFT: Out of memory.\n";
    exit(1);
  }
  if (nr != nc) {
    cout << "FFTIFFT: The image has to be square and dimension power of 2. " 
         << "The current image is not square.\n";
    exit(3);
  }
  
  if (pow(2.0, log((double)nr)/log(2.0)) != (double)nc) {
    cout << "FFTIFFt: The image dimension is not power of 2.\n";
    exit(3);
  }

  N = nr; 
  M = N/2;
  N2 = (int)ceil(log((double)N)/log(2.0));
  realv = (float *) new float [N];
  if (!realv) {
    cout << "FFTIFFT: Out of memory.\n";
    exit(1);
  }
  imagv = (float *) new float [N];
  if (!imagv) {
    cout << "FFTIFFT: Out of memory.\n";
    exit(1);
  }
  loc = (int *) new int [N];
  if (!loc) {
    cout << "FFTIFFT: Out of memory.\n";
    exit(1);
  }

  // ordering the 1D vector for FFT
  loc[0] = 0;
  k = 1;
  m = 1;
  n = M;
  for (i=0; i<N2; i++) {
    for (j=0; j<pow(2.0,(double)i); j++) {
      loc[k] = loc[k-m]+n;
      k++;
    }
    m <<= 1;
    n >>= 1;
  }

  // 1D FFT - 2D row transform
  for (i=0; i<nr; i++) {
    for (j=0; j<nc; j++) {
      sign = (int)pow(-1.0, (double)loc[j]);
      if (scale == 1) {
        realv[j] = sign * inimg(i,loc[j]);
        imagv[j] = sign * 0;
      }
      else {
        temp1 = mag(i, loc[j]);
	// to fix problems when phase is outside the range of [-PI, PI]
	// this would occur when phases are accumulated in certain operators
	// like convolution
	// the following bug is found by Timothy Ragland, class of Fall 2008
	// start fixing ...............
	if (phase(i, loc[j]) > PI)
	  phase(i, loc[j]) -= 2*PI;
	if (phase(i, loc[j]) < -PI)
	  phase(i, loc[j]) += 2*PI;
	// end fixing .................
        temp2 = tan(phase(i, loc[j]));
        realv[j] = sqrt(temp1 * temp1 / (1+temp2*temp2));
        if (phase(i, loc[j]) > PI/2 || phase(i, loc[j]) < -PI/2)
          realv[j] = -realv[j];
        imagv[j] = realv[j] * temp2;
      }
    }
    for (p=0; p<N2; p++) {      // each level
      n = (int)pow(2.0,(double)p);
      for (q=0; q<n; q++) {
        theta = 2 * PI * q / (float)(2*n);
        for (k=q; k<N; k+=2*n) {
          temp1 = realv[k+n] * cos(theta) + imagv[k+n] * sin(theta) * scale;
          temp2 = -realv[k+n] * sin(theta) * scale + imagv[k+n] * cos(theta);
          realv[k+n] = realv[k] - temp1;
          realv[k] = realv[k] + temp1;
          imagv[k+n] = imagv[k] - temp2;
          imagv[k] = imagv[k] + temp2;
        }
      }
    }
    for (j=0; j<nc; j++) {
      index = i*nc + j;
      real[index] = realv[j];
      imag[index] = imagv[j];
    }    
  }

  // 1D FFT - 2D column transform
  for (j=0; j<nc; j++) {
    for (i=0; i<nr; i++) {
      sign = (int)pow(-1.0, (double)loc[i]);  // translate image to the center
      if (scale == 1) {
        realv[i] = sign * real[loc[i]*nc+j];   // reorder
        imagv[i] = sign * imag[loc[i]*nc+j];
      }
      else {
        realv[i] = real[loc[i]*nc+j];
        imagv[i] = imag[loc[i]*nc+j];
      }
    }
    for (p=0; p<N2; p++) {    
      n = (int)pow(2.0,(double)p);
      for (q=0; q<n; q++) {
        theta = 2 * PI * q / (2*n);
        for (k=q; k<N; k+=2*n) {
          temp1 = realv[k+n] * cos(theta) + imagv[k+n] * sin(theta) * scale;
          temp2 = -realv[k+n] * sin(theta) * scale + imagv[k+n] * cos(theta);
          realv[k+n] = realv[k] - temp1;        // F(u+M)
          realv[k] = realv[k] + temp1;          // F(u)
          imagv[k+n] = imagv[k] - temp2;
          imagv[k] = imagv[k] + temp2;
        }
      }
    }
    for (i=0; i<nr; i++) {
      index = i*nc + j;
      real[index] = realv[i];
      imag[index] = imagv[i];
    }
  }

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      index = i*nc + j;
      if (scale == 1) {
        mag(i, j) = sqrt(real[index]*real[index] + imag[index]*imag[index]);
        phase(i, j) = atan2(imag[index], real[index]);
      }
      else {
        inimg(i,j) = sqrt(real[index]*real[index] + 
                imag[index]*imag[index])/(N*N);
      }
    }

  delete [] realv;
  delete [] imagv;
  delete [] real;
  delete [] imag;
  delete [] loc;
}
