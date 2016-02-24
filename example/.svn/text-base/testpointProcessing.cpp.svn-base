/**********************************************************
 * This is a test program for pointProcessing routines 
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
  Image img(4,5);      
  int i, j, k;
  
  // mannually fill in pixel values
  for (i=0; i<4; i++)
    for (j=0; j<5; j++)
        img(i,j) = i+j*2;
  img(0,4) = 200;
  
  // output the image to screen
  cout << "The original image" << endl;    
  cout << img;

  // test different functions
  Image tmp1 = negative(img);
  cout << "\nAfter negative:" << endl << tmp1;
  
  Image tmp2 = logtran(img);
  cout << "\nAfter log transformation:" << endl << tmp2;

  Image tmp3 = cs(img, 2, 5);
  cout << "\nAfter contrast stretching (slope=2,intercept=5):" << endl << tmp3;
  
  Image tmp4 = powerlaw(img, 0.1);
  cout << "\nAfter power-law transformation (gamma=0.1):" << endl << tmp4;
  
  Image tmp5 = threshold(img, 2.0);
  Image tmp6 = threshold(img, 2.0, GRAY);
  Image tmp61 = autoThreshold(img);
  cout << "\nAfter binary thresholding (threshold=2.0):" << endl << tmp5;
  cout << "\nAfter gray-level thresholding:" << endl << tmp6;
  cout << "\nUsing autothresholding:" << endl << tmp61;
    
  Image tmp7[8];
  for (int i=0; i<8; i++)
    tmp7[i].createImage(4,5);
  bitplane(img, tmp7);
  cout << "\nAfter bitplane slicing (1st plane):" << endl << tmp7[0];
  
  Image tmp8 = sampling(img, 2);
  cout << "\nAfter sampling (ratio=2):" << endl << tmp8;
  
  Image tmp9 = quantization(img, 2);
  cout << "\nAfter quantization (level=2):" << endl << tmp9;  

  Image tmp10 = rescale(img);
  cout << "\nAfter rescale between 0 and 255:" << endl << tmp10;
  
  img(0,4) = 300;          // test if rescale
  Image tmp11 = histeq(img);
  cout << "\nAfter histogram equalization:" << endl << tmp11;
  
  return 0;
}
