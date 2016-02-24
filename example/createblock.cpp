/*************************************************************************
 * This is a routine to create a synthetic image for segmentation purpose
 * 
 * Author: Hairong Qi
 * Date: 12/14/15
 ************************************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

int main()
{
  const int dim = 128;
  Image img(dim,dim,PGMRAW);      // create a 3x8 color image of 3 channels
  int i, j;
  
  // mannually fill in pixel values
  for (i=0; i<dim; i++)
    for (j=0; j<dim; j++)
      if ((i+j) >= dim-1) {
	if (i<100 && i>70 && j<100 && j>70)
	  img(i,j) = 50;
	else
	  img(i,j) = 200;
      }
      else {
	if (((i-32)*(i-32)+(j-32)*(j-32)) <= 15*15)
	  img(i,j) = 100;
	else
	  img(i,j) = 10;
      }
  
  writeImage(img, "seg_synthetic_pure.pgm");

  // add Gaussian noise
  img = gaussianNoise(img, 0, 2.0);
  
  writeImage(img, "seg_synthetic.pgm");

  return 0;
}
