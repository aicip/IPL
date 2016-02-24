/**********************************************************
 * colorProcessing.cpp - color processing routings
 *
 *   - RGB2HSI: convert from RBG color model to HSI
 *   - HSI2RGB: convert from HSI color model to RGB
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/31/06
 *
 * Modified:
 *   - Mitch Horton (Class 2008) fixed two bugs in 
 *     RGB2HSI and HSI2RGB
 **********************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;


/**
 * Convert from the RGB color model to the HSI color model
 * @param inimg The input image in RGB model
 * @return The color image represented using hte HSI model
 */
Image RGB2HSI(Image &inimg) {
  Image temp;        // a 3-channel image holds H, S, and I components
  float r, g, b, theta, mini;
  int i, j;
  int nr, nc, nchan, nt;
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();  
  if (nchan != 3) {
    cout << "RGB2HSI: This is not a color image\n";
    exit(3);
  }
  temp.createImage(nr, nc, nt);

  // model conversion
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      r = inimg(i,j,0);           // the red channel
      g = inimg(i,j,1);           // the green channel
      b = inimg(i,j,2);         // the blue channel
      mini = r;
      if (mini > g) 
        mini = g;
      if (mini > b)
        mini = b;
      if ((r==g) && (g==b)) {
	theta = PI/2.0;
      } else {
	theta = acos((0.5*((r-g)+(r-b))) / sqrt((r-g)*(r-g)+(r-b)*(g-b)));
      }
      temp(i,j,0) = (g >= b) ? theta : (2*PI-theta);        // H component
      temp(i,j,2) = (r + g + b) / 3.0;                      // I component
      if (temp(i,j,2)==0)
        temp(i,j,1) = 1;
      else 
        temp(i,j,1) = 1 - mini / temp(i,j,2);               // S component
    }

  return temp;
}


/**
 * Convert the current color image from HSI to RGB model.
 * @param inimg The input color image in HSI model.
 * @return The color image in RGB model.
 */
Image HSI2RGB(Image &inimg) {
  Image temp;        // a 3-channel image holds R, G, B components
  float hue, saturation, intensity, theta;
  int i, j;
  int nr, nc, nt, nchan;
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();  
  if (nchan != 3) {
    cout << "HSI2RGB: This is not a color image\n";
    exit(3);
  }
  temp.createImage(nr, nc, nt);

  // color model conversion
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      hue = inimg(i,j,0);
      saturation = inimg(i,j,1);
      intensity = inimg(i,j,2);
      if (hue < 2*PI/3.0) {
        temp(i,j,0) = intensity * 
          (1 + saturation * cos(hue)/cos(PI/3.0-hue));
        temp(i,j,2) = intensity * (1-saturation);
        temp(i,j,1) = 3.0 * intensity - temp(i,j,0) - temp(i,j,2);
      }
      else if (hue < 4*PI/3.0) {
        hue -= 2*PI/3.0;
        temp(i,j,1) = intensity *
          (1 + saturation * cos(hue)/cos(PI/3.0-hue));
        temp(i,j,0) = intensity * (1 - saturation);
        temp(i,j,2) = 3.0 * intensity - temp(i,j,0) - temp(i,j,1);
      }
      else {
        hue -= 4.0*PI/3.0;
        temp(i,j,2) = intensity * 
          (1 + saturation * cos(hue) / cos(PI/3.0 - hue));
        temp(i,j,1) = intensity * (1 - saturation);
        temp(i,j,0) = 3.0 * intensity - temp(i,j,1) - temp(i,j,2);
      }
    }

  return temp;
}
