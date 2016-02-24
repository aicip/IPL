// Test code to show the effect of padding, FFT, IFFT, autocorr.
//
// The purpose of this code includes:
//
//    1. show how to perform padding and convolution in the frequency domain
//
//    2. benefit of padding
//
// Copyright: hqi@utk.edu
//

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
using namespace std;

#define Usage "./testconv inimg outsp outfreq\n"

int main(int argc, char **argv)
{
  Image img, imgpad, mag, phase, imgsp, imgfreq;
  int i, j;
  int nr, nc;
  
  if (argc < 4) {
    cout << Usage;
    exit(3);
  }

  // read in image
  img = readImage(argv[1]);
  nr = img.getRow();
  nc = img.getCol();

  // perform spatial-domain autocorrelation
  imgsp = conv(img, img);
  
  // pad the mask to be of the same size as the image
  imgpad.createImage(2*nr, 2*nc);
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      imgpad(i,j) = img(i,j);
  mag.createImage(2*nr, 2*nc);
  phase.createImage(2*nr, 2*nc);
  fft(imgpad, mag, phase);

  mag = mag * mag;
  phase = phase - phase;

  imgfreq.createImage(2*nr, 2*nc);
  ifft(imgfreq, mag, phase);

  Image imgauto = subImage(imgfreq, 0, 0, nr-1, nc-1);
 
  writeImage(imgsp, argv[2], 1);
  writeImage(imgauto, argv[3], 1);

  return 0;
}
