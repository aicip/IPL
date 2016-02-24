/**********************************************************
 * This is a test program for addNoise routines and PSNR
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
  Image img(3,8,PPMRAW);      // create a 3x8 color image of 3 channels
  int i, j, k;
  
  // mannually fill in pixel values
  for (i=0; i<3; i++)
    for (j=0; j<8; j++)
      for (k=0; k<3; k++)
        img(i,j,k) = i+j*2+k;
  
  // output the image to screen
  cout << "The original 3-channel color image" << endl;    
  cout << img;

  // add Gaussian noise
  Image noise1;

  noise1 = gaussianNoise(img, 0, 2.0);
  cout << "\nThe original image corrupted by Gaussian noise of (0,2.0)\n";
  cout << noise1;
  
  // add SAP noise
  Image noise2;
  
  noise2 = sapNoise(img, 0.3);
  cout << "\nThe original image corrupted by SAP noise\n";
  cout << noise2;  
  
  // psnr
  Image img0 = img.getImage(0);
  Image noise10 = noise1.getImage(0);
  Image noise20 = noise2.getImage(0);
  
  cout << "The PSNR of the Gaussian noise corrupted image is: " 
       << psnr(img0, noise10) << endl
       << "The PSNR of the SAP noise corrupted image is: "
       << psnr(img0, noise20) << endl;

  return 0;
}
