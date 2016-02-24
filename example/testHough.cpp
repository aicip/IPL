/**********************************************************
 * This is a test program for Hough transform routines
 * 
 * Author: Hairong Qi
 * Date: 03/14/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
  Image img, himg;

/*    
  // create a testing image for detecting line segments
  img.createImage(128,128);
  
  for (int i=40; i<80; i++) {
	img(i,10) = 255.0;
	img(i,30) = 255.0;
  }
  for (int i=100; i<120; i++) {
	img(i,10) = 255.0;
	img(i,30) = 255.0;
  }
  writeImage(img, "test1.pgm");
  
  himg = houghLine(img);

  writeImage(img, "test2.pgm");
  writeImage(himg, "test3.pgm", 1);
*/
/*
  // draw a parabola for testing
  int x0, y0, i, j;
  float p, theta;
  Image img2;
  
  x0 = 60;
  y0 = 80;
  p = 0.05;
  theta = 30;
  
  img.createImage(128,128);
  for (j=0; j<128; j++) {
    i = (int)(-p*(float)((j-x0)*(j-x0))+(float)y0);
    if (i<128 && i>=0)
      img(i,j) = 255;
  }
  img2 = rotate(img, theta);
  
  writeImage(img, "testPara.pgm");
  writeImage(img2, "testPara2.pgm");
  
  houghPara(img);
//  houghPara(img2);
*/
  
  img = readImage(argv[1]);
  himg = houghLine(img);

  writeImage(img, argv[2]);

  return 0;
}
