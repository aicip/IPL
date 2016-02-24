/****************************************************************
 * hough.cpp - Hough transform to detect lines, circles, and
 *   parabolae
 *
 *   - houghLine
 *   - houghParabola
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 03/14/06
 *
 * Modified:
 *  - 
 ****************************************************************/
#include <iostream>
#include "Image.h"
#include "Dip.h"

using namespace std;

/**
 * Hough transform to detect line segments
 * @param inimg The input binary image
 * @return The hough map
 */
Image houghLine(Image &inimg) {
  Image A;                       // the accumulator
  int i, j, nc, nr, nchan;
  int maxrou, maxtheta, rou, theta;
  int maxitheta, maxirou, y;
  float maxi, angle, bx, by, slope;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
  	cout << "houghLine: This function does not process color images.\n";
  	exit(3);
  }

  // calculate the maximum size of the A image using polar coordinate
  maxrou = (int)sqrt((float)(nc*nc + nr*nr));
  maxtheta = 360;   
  A.createImage(maxtheta, maxrou);
  
  // accumulating
  for (i=0; i<nr; i++) {
    for (j=0; j<nc; j++) {
      if (inimg(i,j)) {               // if the pixel is foreground
	for (theta=0; theta<maxtheta; theta++) {
	  angle = theta * 2 * PI / 360;
	  rou = (int)((nr-i)*cos(angle) + j*sin(angle));
	  if (rou <= maxrou && rou >= 0) 
	    A(theta,rou)++;
	}
      }
    }
  }
	
  // find the highest intensity to locate intensity and its location
  maxi = 0;
  for (i=0; i<maxtheta; i++) {
    for (j=0; j<=maxrou; j++) {
      if (A(i,j) > maxi) {
	maxi = A(i,j);
    	maxitheta = i;
	maxirou = j;
      }
    }
  }

  cout << "maxirou = " << maxirou << ",   maxitheta = " << maxitheta << endl;
  
  // find the intersect of this line with the x and y axes
  angle = maxitheta*2*PI/360;
  by = (int)(maxirou/sin(angle));      // the intercept with the y axis
  bx = (int)(maxirou/cos(angle));      // the intercept with the x axis
  slope = -by/bx;
  
  cout << "slope = " << slope << endl;

  // overlay on the input image the detected line segment
  for (i=0; i<nc; i++) {
    y = (int)(slope*i+by);
    if ((nr-y) < nr && (nr-y) >= 0) {
      inimg(nr-y,i) = 255;
    }
  }
  
  writeImage(inimg, "testLine2.pgm");

  return A;
} 


/**
 * Hough transform to detect parabola
 * @param inimg The input image is gray scale
 * @return The hough map
 */
void houghPara(Image &inimg) {
  Image *A, mag, gradient;
  int i, j, theta, nc, nr, nchan;
  int x0, y0;
  float sin2t, cos2t;
  const int maxtheta = 360;   
  int res = 1;                   // resolution of theta
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
  	cout << "houghPara: This function does not process color images.\n";
  	exit(3);
  }

  mag.createImage(nr, nc);
  gradient.createImage(nr, nc);
  
  // detect gradient (direction of the edge) and magnitude of the edge 
  sobel(inimg, mag, gradient);
          
  // allocate memory for the A accumulating array (3-D) or pointer to
  // an array of gray-level images
  // "res" implies how many degrees do you take one sample. The lowest is 1
  A = (Image *) new Image [maxtheta/res];
  for (theta=0; theta<maxtheta; theta+=res)
  	A[theta/res].createImage(nr, nc);
  
  // for each foreground pixel in the input image, find possible places
  // in the parametric space    
  for (i=0; i<nr; i++) {
  	for (j=0; j<nc; j++) {
      if ((int)mag(i,j)) {
      	for (theta=0; theta<maxtheta; theta+=res) {
          for (x0=0; x0<nc; x0++) {  // follow the reference SPIE paper
          	sin2t = sin((float)theta*360/PI);
          	cos2t = cos((float)theta*360/PI);
          	y0 = (int)((j-x0) * (3*gradient(i,j)-gradient(i,j)*cos2t-sin2t) / 
          	                   (cos2t-gradient(i,j)*sin2t+2));
          	if (y0<nr && y0>=0)
          	  A[theta/res](y0,x0)++; 
          }
      	}
      }
    }
  }

  // find local maximum
  int ix[(int)maxtheta/res], iy[(int)maxtheta/res], index[(int)maxtheta/res];
  float maxi[(int)maxtheta/res];
  
  // first figure out all the maximum intensities at different theta
  for (theta=0; theta<maxtheta; theta+=res)  
    A[theta/res].getMaximum(maxi[theta/res], ix[theta/res], iy[theta/res]);
    
  // bubblesort with index
  bubblesort(maxi, maxtheta/res, index);
  
  // the (x0,y0) 
  cout << "(x0, y0)=" << "(" << iy[index[(int)maxtheta/res]] << ","
       << ix[index[(int)maxtheta/res]] << ")" << endl;

  // probably need other testing  
}
