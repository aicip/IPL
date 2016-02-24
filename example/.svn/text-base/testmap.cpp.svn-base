/**********************************************************
 * This is a test program for mapmfa
 * 
 * Author: Hairong Qi
 * Date: 02/12/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include "Map.h"
#include <iostream>

using namespace std;

int main()
{
  Image gimg, fimg;      
  Map par;                   // an object that holds all the parameters
  int i, j;
  
  /*
  // generate the corrupted checkerboard image
  gimg = readImage("checkerboard_perfect.pgm");
  gimg = average(gimg, 3);                  // blur the image first
  gimg = gaussianNoise(gimg, 3.0);          // add Gaussian noise  
  writeImage(gimg, "checkerboard_corrupt.pgm");
  */

  // readin the corrupted image, 
  // blurred by an average filter and added by N(0,3.0) noise  
  gimg = readImage("checkerboard_corrupt.pgm");

  // set the initial temperature to be sqrt of the average intensity
  par.setTi(sqrt(sum(gimg) / (gimg.getRow()*gimg.getCol())));
  par.setTf(0.1);
  par.setTd(0.75);
  par.setBeta(1.0);
  par.setRelax(0.005);
  par.setSigma(3.0);
  par.setErr(0.5);
  par.setDiv(5);
  
  fimg = mapmfa(gimg, par);
  
  return 0;
}
