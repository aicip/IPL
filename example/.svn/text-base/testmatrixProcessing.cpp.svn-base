/**********************************************************
 * This is a test program for matrixProcessing routines
 * and some utility routines
 * 
 * Author: Hairong Qi
 * Date: 01/26/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

int main()
{
  Image img(4,4);      // create an 8x8 image
  int i, j;
  
  // mannually fill in pixel values
  for (j=0; j<4; j++) {
    img(0,j) = j+1;
    img(1,j) = 4-j;
    img(2,j) = j+3;
    img(3,j) = j+2;
  }
  img(2,2) = 2; 
  img(2,3) = 1;
  img(3,3) = 1;
  
  // output the image to screen
  cout << "The original image" << endl;    
  cout << img;

  // image transpose
  Image timg = transpose(img);
  cout << "\nThe transposed image" << endl;
  cout << timg;

  // image inverse
  Image iimg = inverse(img);
  cout << "\nThe inversed image" << endl;
  cout << iimg;
  
  // image cropping
  Image simg = subImage(img, 2, 2, 3, 3);
  cout << "\nThe lower-right quarter of the image" << endl;
  cout << simg;
  
  return 0;
}
