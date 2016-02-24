// Test code for the FFT and IFFT functions using a simple numerical example
// and how padding can be done during the implementation of convolution
// in the frequency domain

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
using namespace std;

#define Usage "./testfft_num \n"

int main(int argc, char **argv)
{
  Image inimg, mag, phase, outimg, img2, img3;
  int imgsize = 4;
  int i, j;

  if (argc < 1) {
    cout << Usage;
    exit(3);
  }

  // create the image 
  inimg.createImage(imgsize, imgsize);
  inimg(0,0) = inimg(1,1) = inimg(2,2) = inimg(3,3) = 255.0;
  cout << "Input image:\n" << inimg << endl;
  
  // for padding
  img2.createImage(2*imgsize, 2*imgsize);

  for (i=0; i<imgsize; i++)
    for (j=0; j<imgsize; j++)
      img2(i,j) = inimg(i,j);

  // perform FFT and IFFT
  mag.createImage(2*imgsize, 2*imgsize);
  phase.createImage(2*imgsize, 2*imgsize);
  img3.createImage(2*imgsize, 2*imgsize);

  fft(img2, mag, phase);
  cout << "Magnitude after padding:\n" << mag << endl;
  ifft(img3, mag, phase);
  cout << "IFFT image with padding:\n" << img3 << endl;

  // you need to apply subImage to cut the upper-left corner
  // to obtain the resulting image of the right size


  return 0;
}

