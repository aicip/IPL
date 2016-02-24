/********************************************
 * geocorr.cpp: Geometric Correction
 *
 * Using 2nd degree polynomial Approximation
 *
 * Author: Mark Quayle
 * 
 * Created: 09/13/2007
 *
 * Modified:
 *  - 10/07/07 by H. Qi on formatting
 *******************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cmath>

using namespace std;

/*
 * Geometric Correction of an Image
 * A = pseudo_inverse(W)->*X
 * B = pseudo_inverse(W)->*Y
 * pseudo_inverse(W) = inverse((transpose(W)->*W))->*transpose(W)
 * W = [1  , u_0  , v_0  , u_0^2  , u_0*v_0    , v_0^2  ;
        1  , u_1  , v_1  , u_1^2  , u_1*v_1    , v_1^2  ;
        ..., ...  , ...  , ...    , ...        , ...    ;
	1  , u_m-1, v_m-1, u_m-1^2, u_m-1*v_m-1, v_m-1^2]
 * X = x coordinates of tiepoints of distorted image
 * Y = y coordinates of tiepoints of distorted image
 * U = x coordinates of tiepoints of the corrected image
 * V = y coordinates of tiepoints of the corrected image
 * X' = X->*A - x coordinates of corrected image
 * Y' = Y->*B - y coordinates of corrected image
 * @param inimg Input image
 * @return outimg Corrected image
 */

Image geocorr(Image &inimg){
  Image outimg, X, Y, U, V, A, B;
  Image W, W_transpose, W_temp, W_pseudo, W_temp_inv;
  int i, x, y, k, u, v;
  int nr, nc, ntype, nchan;

  //allocate memory
  nr    = inimg.getRow();
  nc    = inimg.getCol();
  ntype = inimg.getType();
  nchan = inimg.getChannel();

  outimg.createImage(nr, nc, ntype);  

  //Create Ideal Image Coordinates Matrices
  //Using 15 tiepoints

  V.createImage(15, 1, PGMRAW);   
  V(0,0)  = 0;
  V(1,0)  = 207;
  V(2,0)  = 414;
  V(3,0)  = 621;
  V(4,0)  = 827;
  V(5,0)  = 0;
  V(6,0)  = 207;
  V(7,0)  = 414;
  V(8,0)  = 621;
  V(9,0)  = 827;
  V(10,0) = 0;
  V(11,0) = 207;
  V(12,0) = 414;
  V(13,0) = 621;
  V(14,0) = 827;

  U.createImage(15, 1, PGMRAW);
  U(0,0)  = 0;
  U(1,0)  = 0;
  U(2,0)  = 0;
  U(3,0)  = 0;
  U(4,0)  = 0;
  U(5,0)  = 56;
  U(6,0)  = 56;
  U(7,0)  = 56;
  U(8,0)  = 56;
  U(9,0)  = 56;
  U(10,0) = 257;
  U(11,0) = 257;
  U(12,0) = 257;
  U(13,0) = 257;
  U(14,0) = 257;

  //Create Distorted Image Coordinates Matrices
  //Using 15 tiepoints

  Y.createImage(15, 1, PGMRAW);
  Y(0,0)  = 0;
  Y(1,0)  = 207;
  Y(2,0)  = 414;
  Y(3,0)  = 621;
  Y(4,0)  = 827;
  Y(5,0)  = 0;
  Y(6,0)  = 207;
  Y(7,0)  = 414;
  Y(8,0)  = 621;
  Y(9,0)  = 827;
  Y(10,0) = 0;
  Y(11,0) = 207;
  Y(12,0) = 414;
  Y(13,0) = 621;
  Y(14,0) = 827;

  X.createImage(15, 1, PGMRAW);
  X(0,0)  = 0;
  X(1,0)  = 0;
  X(2,0)  = 0;
  X(3,0)  = 0;
  X(4,0)  = 50;
  X(5,0)  = 105;
  X(6,0)  = 122;
  X(7,0)  = 110;
  X(8,0)  = 86;
  X(9,0)  = 56;
  X(10,0) = 257;
  X(11,0) = 257;
  X(12,0) = 257;
  X(13,0) = 257;
  X(14,0) = 257;

  //Create W matrix - 2nd degree polynomial
  
  W.createImage(15, 6, PGMRAW);

  for (i=0; i<15; i++) {  
    W(i,0) = 1; 
    W(i,1) = U(i,0); 
    W(i,2) = V(i,0); 
    W(i,3) = U(i,0)*U(i,0); 
    W(i,4) = U(i,0)*V(i,0); 
    W(i,5) = V(i,0)*V(i,0);
  }

  //Calculate the pseudo-inverse of W

  W_transpose = transpose(W);
  W_temp = W_transpose->*W;
  W_temp_inv = inverse(W_temp);
  W_pseudo = W_temp_inv->*W_transpose;
  
  //Calculate the Coefficient Matrices

  A = W_pseudo->*X;
  B = W_pseudo->*Y;

  //Perform Geometric Correction Using Coefficients

  for (u=0; u<nr; u++) {
    for (v=0; v<nc; v++) {
      for (k=0; k<nchan; k++) {
	x = A(0,0) + A(1,0)*u + A(2,0)*v + A(3,0)*u*u + A(4,0)*u*v + A(5,0)*v*v;
	y = B(0,0) + B(1,0)*u + B(2,0)*v + B(3,0)*u*u + B(4,0)*u*v + B(5,0)*v*v;	
	if ((0<=x && x<nr) && (0<=y && y<nc))
	  outimg(u,v,k) = inimg(x,y,k);
      }
    }
  }
  
  return outimg;
}
