// Test code to show the effect of padding, FFT, IFFT, convolution, etc.
//
// The purpose of this code includes:
//
//    1. show how to perform padding and convolution in the frequency domain
//
//    2. benefit of padding
//
//       compare the spatial result and freq. result without padding
//             testavg.pgm vs. testavgFnopad.pgm and testdiff1.pgm
//       compare the spatial result and freq. result with padding
//             testavg.pgm vs. testavgFpad.pgm and testdiff2.pgm
//
// The mask is an average mask
//
//
// Copyright: hqi@utk.edu
//

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
using namespace std;

#define Usage "./testpadding inimg masksize\n"

int main(int argc, char **argv)
{
  Image img, imgMag, imgPhase,         // the original image 
    imgpad, imgpadMag, imgpadPhase,    // the padded original image
    imgavg,          // filtered original image in spatial domain
    imgavgFnopad,    // filtered original image using FT without padding
    imgavgFpad,      // filtered original image using FT with padding
    mask,            // mask 
    maskpad1, maskpad1Mag, maskpad1Phase,  
                // padded mask for conv in the freq. domain without padding
    maskpad2, maskpad2Mag, maskpad2Phase;  
                // padded mask for conv in the freq. domain with padding
  int i, j, m, n;
  int nr, nc;
  int masksize;
  
  if (argc < 3) {
    cout << Usage;
    exit(3);
  }

  // read in image
  img = readImage(argv[1]);
  nr = img.getRow();
  nc = img.getCol();

  // generate the mask
  masksize = atoi(argv[2]); // assuming average mask
  mask.createImage(masksize, masksize);
  for (i=0; i<masksize; i++)
    for (j=0; j<masksize; j++)
      mask(i,j) = 1.0/(masksize*masksize);

  /////////////////////////////////////////////////////////
  // spatial domain average filter
  // assume you already wrote a function that does avg
  //  imgavg = img.average(masksize);
  float lsum;
  imgavg.createImage(nr, nc);
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      lsum = 0.0;
      for (m=-(masksize-1)/2; m<=(masksize-1)/2; m++)
	for (n=-(masksize-1)/2; n<=(masksize-1)/2; n++)
	  if (i+m>=0 && i+m<nr && j+n>=0 && j+n<nc)
	      lsum += img(i+m, j+n);
      imgavg(i,j) = lsum/(masksize*masksize);
    }
  
  /////////////////////////////////////////////////////////////////////
  // average filter implemented in the frequency domain without padding
  imgMag.createImage(nr, nc);
  imgPhase.createImage(nr, nc);
  fft(img, imgMag, imgPhase);

  // pad the mask to be of the same size as the image
  maskpad1.createImage(nr, nc);
  for (i=0; i<masksize; i++)
    for (j=0; j<masksize; j++)
      maskpad1(i,j) = mask(i,j);
  maskpad1Mag.createImage(nr, nc);
  maskpad1Phase.createImage(nr, nc);
  fft(maskpad1, maskpad1Mag, maskpad1Phase);

  imgMag = imgMag * maskpad1Mag;
  imgPhase = imgPhase + maskpad1Phase;

  imgavgFnopad.createImage(nr, nc);
  ifft(imgavgFnopad, imgMag, imgPhase);

  /////////////////////////////////////////////////////////////////////
  // averaging filter implemented in the frequency domain with padding
 
  // pad the original image to be larger than nr+masksize-1, need to be power
  // or 2 as well
  int nrpad = nr*2, ncpad = nc*2;

  imgpad.createImage(nrpad, ncpad);
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      imgpad(i,j) = img(i,j);

  imgpadMag.createImage(nrpad, ncpad);
  imgpadPhase.createImage(nrpad, ncpad);
  fft(imgpad, imgpadMag, imgpadPhase);
  
  // pad the mask to the same size as imgpad
  maskpad2.createImage(nrpad, ncpad);
  for (i=0; i<masksize; i++)
    for (j=0; j<masksize; j++)
      maskpad2(i,j) = mask(i,j);

  maskpad2Mag.createImage(nrpad, ncpad);
  maskpad2Phase.createImage(nrpad, ncpad);
  fft(maskpad2, maskpad2Mag, maskpad2Phase);

  // convolution in the freq. domain
  imgpadMag = imgpadMag * maskpad2Mag;
  imgpadPhase = imgpadPhase + maskpad2Phase;

  imgavgFpad.createImage(nrpad, ncpad);
  ifft(imgavgFpad, imgpadMag, imgpadPhase);

  // cut the upper-left corner
  imgavgFpad = subImage(imgavgFpad, (masksize-1)/2, (masksize-1)/2, 
				   nr-1+(masksize-1)/2, nc-1+(masksize-1)/2);
 
  //////////////////////////////////////////////////////////
  // compare imgavg, imgavgFnopad, imgavgFpad

  Image diff1, diff2;

  diff1 = imgavg - imgavgFnopad;
  diff2 = imgavg - imgavgFpad;

  cout << "diff1\n" << diff1.getMaximum() << ' ' << diff1.getMinimum() << endl;
  cout << "diff2\n" << diff2.getMaximum() << ' ' << diff2.getMinimum() << endl;

  writeImage(imgavg, "testavg.pgm");             // result from spatial domain
  writeImage(imgavgFnopad, "testavgFnopad.pgm"); // in freq. domain no padding  
  writeImage(imgavgFpad, "testavgFpad.pgm");     // in freq. domain with padding 
  writeImage(diff1, "testdiff1.pgm", 1);
  writeImage(diff2, "testdiff2.pgm", 1);

  return 0;
}
