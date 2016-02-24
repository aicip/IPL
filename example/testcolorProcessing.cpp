/**********************************************************
 * This is a test program for colorProcessing routines 
 * 
 * Author: Hairong Qi
 * Date: 01/31/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

int main()
{
  Image inimg, hsi, rgb, hue, sat, intensity;
  
  inimg = readImage("peppers.ppm");             // read a color image
  
  hsi = RGB2HSI(inimg);
  hue = hsi.getImage(0);
  sat = hsi.getImage(1);
  intensity = hsi.getImage(2);
  
  writeImage(hue, "peppershue.pgm", 1);         // write hue
  writeImage(sat, "pepperssat.pgm", 1);         // write satuation
  writeImage(intensity, "peppersint.pgm", 1);   // write intensity
  
  rgb = HSI2RGB(hsi);
  
  writeImage(rgb, "pepperstest.ppm");           // write the rgb back
  
  return 0;
}
