/*********************************************************************
 * wt.cpp - perform 2-D wavelet transform
 *
 *   - daub4: Daubechies 4-coefficient wavelet filter
 *   - wt1d: 1-D wavelet transform
 *   - wt2d: 2-D wavelet transform
 * 
 * Contributors: 
 *   - wt1d: by Xiaoling Wang based on Numerical Recipe in C
 *   - wt2d: based on Tom Karnowski's project of Class 2005 
 * 
 * Last Modified:
 *   - 12/01/07: instead of membership functions, written as standard
 *               function by hqi@utk.edu
 *               wt2d forward and inverse is handled in one function
 ********************************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cmath>

using namespace std;

/**
 * Daubechies 4-coefficient wavelet filter. 
 * Reference: Numerical Recipe in C
 * @param inimg The input vector, whose length should be power of 2.
 * @param n The length of the low-passed signal, corresponding to the WT level
 * @param isign Apply wavelet filter when isign=1
 *              Apply the inverse when isign=-1
 * @return The wavelet coefficients at a certain level, saved in inimg
 **/

void daub4(Image &inimg, int n, int isign)
{
  // coefficients for D4 wavelet transform
  const double C0 = 0.4829629131445341;
  const double C1 = 0.8365163037378079;
  const double C2 = 0.2241438680420134;
  const double C3 = -0.1294095225512604;

  Image wksp;
  int nh, nh1, i, j;

  wksp.createImage(1,n);
  nh1 = (nh=n>>1) + 1;

  if (isign>=0) {
    for (i=0,j=0; j<n-3; j+=2,i++) {
      // the 1st half is the smoothed version of the input vector,
      // obtained by applying the low-pass filter
      wksp(0,i) = C0*inimg(0,j)+C1*inimg(0,j+1)+C2*inimg(0,j+2)+ 
	C3*inimg(0,j+3);           
      // the 2nd half is the detail obtained by applying the high-pass filter
      wksp(0,i+nh) = C3*inimg(0,j)-C2*inimg(0,j+1)+C1*inimg(0,j+2)- 
	C0*inimg(0,j+3);           
    }
    // taking care of the last coefficient from wrap-around values
    wksp(0,i) = C0*inimg(0,n-2)+C1*inimg(0,n-1)+C2*inimg(0,0)+C3*inimg(0,1);
    wksp(0,i+nh) = C3*inimg(0,n-2)-C2*inimg(0,n-1)+C1*inimg(0,0)-C0*inimg(0,1);
  }
  else {
    wksp(0,0) = C2*inimg(0,nh-1)+C1*inimg(0,n-1)+C0*inimg(0,0)+
      C3*inimg(0,nh1-1);
    wksp(0,1) = C3*inimg(0,nh-1)-C0*inimg(0,n-1)+C1*inimg(0,0)- 
      C2*inimg(0,nh1-1);
    for (i=0,j=2; i<nh-1; i++) {
      wksp(0,j++) = C2*inimg(0,i)+C1*inimg(0,i+nh)+C0*inimg(0,i+1)+ 
	C3*inimg(0,i+nh1);
      wksp(0,j++) = C3*inimg(0,i)-C0*inimg(0,i+nh)+C1*inimg(0,i+1)- 
	C2*inimg(0,i+nh1);
    }
  }

  for (i=0; i<n; i++)
    inimg(0,i) = wksp(0,i);
}


/**
 * The 1-D wavelet transform. Reference: Numerical Recipe in C
 * @inimg The input vector.
 * @isign DWT when isign=1, IDWT isign=-1
 * @return 1-D DWT or IDWT.
 **/
Image wt1d(Image &inimg, int isign)
{
  int nn, i, j, nr, nc, len, count;
  Image temp;

  nr = inimg.getRow();
  nc = inimg.getCol();
  if (nr > 1) {
    cout << "WT1D: Can only return wavelet transform of 1-D row vector.\n";
    exit(3);
  }

  // check if the length of input image is an integer power of 2
  // if not, then cut the rest of the image
  len = floorPower2(nc);
  if (len < 4) {
    cout << "WT1D: The length of the 1-D signal is less than 4.\n";
    exit(1);
  }
  if (len != nc) {
    cout << "WT1D: The signal has been cut to make the length power of 2.\n";
    temp = subImage(inimg, 0, 0, 0, len-1);
  }
  else
    temp = inimg;

  // perform 1-D wt using Daubechies mother wavelet with 4 coefficients
  if (isign>=0) {
    for (nn=len; nn>=4; nn>>=1) 
      daub4(temp, nn, isign);
  }
  else {
    for (nn=4; nn<=len; nn<<=1)
      daub4(temp, nn, isign);
  }

  return temp;
}


/**
 * The 2-D wavelet transform. 
 * @inimg The input image.
 * @level The level of decomposition. 
 * @isign DWT when isign=1, IDWT isign=-1.
 * @return 2-D DWT (IDWT) with the following arrangement:
 *         UL quadrant - blurred version of the image.
 *         UR quadrant - vertical edges (details in the horizontal direction)
 *         LL quadrant - horizontal edges (details in the vertical direction)
 *         LR quadrant - corners (details in both directions)
 **/
Image wt2d(Image &inimg, int level, int isign)
{
  int i, j, nn1, nn2, nr, nc, nr2, nc2;
  Image temp, inimg2;

  nr = inimg.getRow();
  nc = inimg.getCol();

  // check if the size of the input image is integer power of 2
  // if not, then cut the rest of the image
  nr2 = floorPower2(nr);
  nc2 = floorPower2(nc);
  if (nc2 < 4 || nr2 < 4) {
    cout << "WT2D: The size of the input image is less than 4.\n";
    exit(1);
  }
  if (nc2 != nc || nr2 != nr) {
    cout << "WT2D: The image has been cut to make size power of 2.\n";
    inimg2 = subImage(inimg, 0, 0, nr2-1, nc2-1);
  }
  else
    inimg2 = inimg;

  // perform 2-D wt using Daubechies mother wavelet with 4 coefficients
  // somehow, when nn1 or nn2 is less than 4, still continue. need fix
  if (isign >= 0) {
    for (nn1=nc2,nn2=nr2; nn1>=(int)nc2/pow(2.0,(double)level) &&
	   nn2>=(int)nr2/pow(2.0,(double)level); 
	 nn1>>=1,nn2>>=1) {
      // WT in the horizontal direction
      for (i=0; i<nr2; i++) {
	temp = subImage(inimg2, i, 0, i, nc2-1);
	daub4(temp, nn1, isign);
	for (j=0; j<nc2; j++)
	  inimg2(i,j) = temp(0,j);
      }
      // WT in the vertical direction
      for (j=0; j<nc2; j++) {
	temp = subImage(inimg2, 0, j, nr2-1, j);
	temp = transpose(temp);
	daub4(temp, nn2, isign);
	for (i=0; i<nr2; i++)
	  inimg2(i,j) = temp(0,i);
      }
    }
  }
  else {
    for (nn1=(int)nc2/pow(2.0,(double)level),
	   nn2=(int)nr2/pow(2.0,(double)level); nn1<=nc2 && nn2<=nr2;
	 nn1<<=1,nn2<<=1) {
      // WT in the horizontal direction
      for (i=0; i<nr2; i++) {
	temp = subImage(inimg2, i, 0, i, nc2-1);
	daub4(temp, nn1, isign);
	for (j=0; j<nc2; j++)
	  inimg2(i,j) = temp(0,j);
      }
      // WT in the vertical direction
      for (j=0; j<nc2; j++) {
	temp = subImage(inimg2, 0, j, nr2-1, j);
	temp = transpose(temp);
	daub4(temp, nn2, isign);
	for (i=0; i<nr2; i++)
	  inimg2(i,j) = temp(0,i);
      }
    }
  }

  return inimg2;
}



