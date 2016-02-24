/**********************************************************
 * This is a test program for the Image class
 * 
 * Author: Hairong Qi
 * Date: 01/22/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

int main()
{
  Image img(2,2,PPMRAW);      // create a 2x2 color image of 3 channels
  int i, j, k;
  
  // mannually fill in pixel values
  for (i=0; i<4; i++)
    for (j=0; j<3; j++)
      for (k=0; k<3; k++)
        img(i,j,k) = i+j*2+k;
  
  // output the image to screen
  cout << "The original 3-channel color image" << endl;    
  cout << img;
  
  // create a grayscale image of the same size
  Image imggray(2,2);
  
  // assign the 3rd channel (k=2) of the color image to the grayscale image
  // or use img.getBlue();
  imggray = img.getImage(2);
  cout << "\nThe 3rd channel of the color image is: " << endl;
  cout << imggray;
  
  // set the 2nd channel (k=1) of the color image using the grayscale image
  // or use img.setGreen(imggray);
  img.setImage(imggray, 1);
  cout << "\nThe color image after setting the 2nd channel using the 3rd" 
       << endl;
  cout << img;
  
  // output the maximum and minimum values of the color image
  // the maximum and minimum are vectors
  float maxi = img.getMaximum();
  float mini = img.getMinimum();
  cout << "\nMaximum and minimum value of the image" << endl;
  cout << maxi << ' ' << mini << endl;
  
  // operator overloadings
  Image add;
  
  add = img / 2;
  cout << "\nImage divided by 2" << endl;
  cout << add;
  
  add = img * 2;
  cout << "\nImage multiplied by 2" << endl;
  cout << add;
  
  add = img + add;
  cout << "\nOriginal image add the time 2 version " << endl;
  cout << add;

  // test the overloading of () with one and two arguments 
  cout << "\nTest the overloading function of pixel access" << endl; 
  cout << add(1,1) << ' ' << maxi << endl;
  
  // test friend overloading functions
  Image tmp;
  
  tmp = img / 2;
  cout << "\nOriginal divided by 2" << endl;
  cout << tmp;
  
  tmp = tmp + 30;
  cout << "\nThen add 30" << endl;
  cout << tmp;
  
  return 0;
}
