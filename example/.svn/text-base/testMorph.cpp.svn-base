/**********************************************************
 * This is a test program for morph routines
 * 
 * Author: Hairong Qi
 * Date: 02/06/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

int main()
{
  Image img, se;      
  int i, j;

/*  
  // test binary morphology
  // create the image
  img.createImage(4,5);
  img(0,3) = img(1,1) = img(2,1) = img(2,2) = 1.0;
    
  // the s.e. is a 3x3 kernel with all pixels as foreground 
  se.createImage(3,3);
  se(1,1) = se(2,1) = se(2,2) = 1.0;
  
  // test different functions
  Image tmp1 = dilate(img, se, 1, 1);
  cout << tmp1 << endl;
    
  Image tmp2 = erode(img, se, 1, 1);
  cout << tmp2 << endl;

  Image tmp3 = open(img, se, 1, 1);
  cout << tmp3 << endl;
  
  Image tmp4 = close(img, se, 1, 1);
  cout << tmp4 << endl;
*/
  
  // test gray-scale morphology
  img = readImage("cameraman.pgm");

  se.createImage(5,5);
  for (i=0; i<5; i++)
    for (j=0; j<5; j++)
      se(i,j) = 10/sqrt((i-2)*(i-2)+(j-2)*(j-2)+1.0); 

  Image tmp1 = dilate(img, se, 2, 2, GRAY);
  writeImage(tmp1, "cameraman_dilate.pgm");
  
  Image tmp2 = erode(img, se, 2, 2, GRAY);
  writeImage(tmp2, "cameraman_erode.pgm");      
        
  return 0;
}
