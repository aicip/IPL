/**********************************************************
 * This is a test program for edgeDetection routines
 * and lowpass routines 
 * 
 * Author: Hairong Qi
 * Date: 01/26/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

#define Usage "./testmedian\n"

int main()
{
  Image img;      

  img = readImage("../testimage/lena.pgm");
  
  // add SAP noise
  Image noisyImg1 = sapNoise(img, 0.1);
  Image noisyImg2 = sapNoise(img, 0.25);

  // test different functions
  Image tmp1 = median(noisyImg1, 5);
  writeImage(tmp1, "lena0.1med5.pgm");

  tmp1 = median(noisyImg1, 3);
  writeImage(tmp1, "lena0.1med3.pgm");

  tmp1 = median(noisyImg1, 7);
  writeImage(tmp1, "lena0.1med7.pgm");

  tmp1 = median(noisyImg2, 7);
  writeImage(tmp1, "lena0.25med7.pgm");

  tmp1 = median(noisyImg2, 5);
  writeImage(tmp1, "lena0.25med5.pgm");

  tmp1 = median(noisyImg2, 3);
  writeImage(tmp1, "lena0.25med3.pgm");

  tmp1 = amedian(noisyImg1, 7);
  writeImage(tmp1, "lena0.1amed7.pgm");

  tmp1 = amedian(noisyImg1, 5);
  writeImage(tmp1, "lena0.1amed5.pgm");

  tmp1 = amedian(noisyImg1, 3);
  writeImage(tmp1, "lena0.1amed3.pgm");

  tmp1 = amedian(noisyImg2, 7);
  writeImage(tmp1, "lena0.25amed7.pgm");

  tmp1 = amedian(noisyImg2, 5);
  writeImage(tmp1, "lena0.25amed5.pgm");

  tmp1 = amedian(noisyImg2, 3);
  writeImage(tmp1, "lena0.25amed3.pgm");

  writeImage(noisyImg1, "lena0.1.pgm");
  writeImage(noisyImg2, "lena0.25.pgm");

  return 0;
}
