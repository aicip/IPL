/**
 * Image convolution with "mask" - a linear operation.
 * @param inimg The input image.
 * @param mask The convolution kernel as an image. 
 * Cannot have more than 1 channel.
 * @return The image after the kernel operation
 */
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

Image conv(Image &inimg, Image &mask)
{
  Image outimg;
  int i, j, k, m, n;
  int nc1, nr1, nc2, nr2, ntype1, nchan1, nchan2;
  float sum, radiusC, radiusR;

  // get the dimension of the image
  nc1 = inimg.getCol();
  nr1 = inimg.getRow();
  ntype1 = inimg.getType();
  nchan1 = inimg.getChannel();
  
  // get the dimension of the kernel
  nc2 = mask.getCol();
  nr2 = mask.getRow();
  nchan2 = mask.getChannel();
  if (nchan2 > 1) {
    cout << "convolution: The mask cannot have more than 1 channel.\n";
    exit(3);
  }
  
  // handle odd and even mask size
  radiusC = (float)nc2/2;
  radiusR = (float)nr2/2;
  
  // allocate memory for the output image
  outimg.createImage(nr1, nc1, ntype1);

  // perform the convolution (or kernel operation)
  for (k=0; k<nchan1; k++) {
    for (i=0; i<nr1; i++)
      for (j=0; j<nc1; j++) {
        sum = 0;    
        for (m=(int)-radiusR; m<=(int)radiusR; m++)
          for (n=(int)-radiusC; n<=(int)radiusC; n++)
            if (i+m>=0 && i+m<nr1 && j+n>=0 && j+n<nc1 &&
                radiusR+m>=0 && radiusR+m<nr2 && radiusC+n>=0 && radiusC+n<nc2)
              sum += inimg(i+m,j+n,k) * mask((int)radiusR+m,(int)radiusC+n);
        outimg(i,j,k) = sum;
      }
  }

  return outimg;
}
