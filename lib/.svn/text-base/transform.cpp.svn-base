/**********************************************************
 * transform.cpp - Implement various affine transforms
 *                 as well as the perspective transform
 *
 *   - rotate: counter-clockwise rotation
 *   - scale: 
 *   - translate:
 *   - shear:
 *   - perspective
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   - 07/30/09: correct perspective transformation
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>

using namespace std;


/**
 * Affine transform - rotation. The homogeneous rotation matrix is
 * [cos(theta) -sin(theta) 0; sin(theta) cos(theta) 0; 0 0 1]
 * @param inimg The input image.
 * @param theta The rotation angle.
 * @return The rotated image.
 */
Image rotate(Image &inimg, float theta) {
  Image outimg;
  Image R(3,3), IR;   // the rotation matrix
  int i, j, k;
  int nr, nc, nt, nchan;
  int x, y;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  outimg.createImage(nr, nc, nt);
  
  // convert from degree to radian
  theta = theta * PI / 180.0;
  
  R(0,0) = cos(theta);
  R(0,1) = -sin(theta);
  R(0,2) = 0;
  R(1,0) = sin(theta);
  R(1,1) = cos(theta);
  R(1,2) = 0;
  R(2,0) = 0;
  R(2,1) = 0;
  R(2,2) = 1;
  
  IR = inverse(R);          // calculate the inverse transform

  // for each (i,j) in the transformed image, find the corresponding point
  // from the original image (inverse transform)
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      x = (int)(i*IR(0,0) + j*IR(0,1) + IR(0,2)*1);
      y = (int)(i*IR(1,0) + j*IR(1,1) + IR(1,2)*1);
      if (x < nr && y < nc && x > 0 && y > 0)
        for (k=0; k<nchan; k++)
          outimg(i,j,k) = inimg(x,y,k);
    }

  return outimg;
}


/**
 * Affine transform - scale. The homogeneous scale matrix is
 * [sx 0 0; 0 sy 0; 0 0 1]
 * @param inimg The input image.
 * @param sx The scaling factor in the horizontal direction
 * @param sy The scaling factor in the vertical direction
 * @return The scaled image.
 */
Image scale(Image &inimg, float sx, float sy) {
  Image outimg;
  Image S(3,3), IS;   // the translation matrix
  int i, j, k;
  int nr, nc, nt, nchan;
  int x, y;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  outimg.createImage(nr, nc, nt);

  S(0,0) = sx;
  S(0,1) = 0;
  S(0,2) = 0;
  S(1,0) = 0;
  S(1,1) = sy;
  S(1,2) = 0;
  S(2,0) = 0;
  S(2,1) = 0;
  S(2,2) = 1;
  
  IS = inverse(S);          // calculate the inverse transform

  // for each (i,j) in the transformed image, find the corresponding point
  // from the original image (inverse transform)
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      x = (int)(i*IS(0,0) + j*IS(0,1) + IS(0,2)*1);
      y = (int)(i*IS(1,0) + j*IS(1,1) + IS(1,2)*1);
      if (x < nr && y < nc && x > 0 && y > 0)
        for (k=0; k<nchan; k++)
          outimg(i,j,k) = inimg(x,y,k);
    }

  return outimg;
}


/**
 * Affine transform - translation. The homogeneous translation matrix is
 * [1 0 tx; 0 1 ty; 0 0 1]
 * @param inimg The input image.
 * @param tx The translation in the horizontal direction.
 * @param ty The translation in the vertical direction.
 * @return The translated image.
 */
Image translate(Image &inimg, float tx, float ty) {
  Image outimg;
  Image T(3,3), IT;   // the translation matrix
  int i, j, k;
  int nr, nc, nt, nchan;
  int x, y;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  outimg.createImage(nr, nc, nt);

  T(0,0) = 1;
  T(0,1) = 0;
  T(0,2) = tx;
  T(1,0) = 0;
  T(1,1) = 1;
  T(1,2) = ty;
  T(2,0) = 0;
  T(2,1) = 0;
  T(2,2) = 1;
  
  IT = inverse(T);          // calculate the inverse transform

  // for each (i,j) in the transformed image, find the corresponding point
  // from the original image (inverse transform)
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      x = (int)(i*IT(0,0) + j*IT(0,1) + IT(0,2)*1);
      y = (int)(i*IT(1,0) + j*IT(1,1) + IT(1,2)*1);
      if (x < nr && y < nc && x > 0 && y > 0)
        for (k=0; k<nchan; k++)
          outimg(i,j,k) = inimg(x,y,k);
    }

  return outimg;
}


/**
 * Affine transform - shear. The homogeneous shear matrix is
 * [1 hx 0; hy 1 0; 0 0 1]
 * @param inimg The input image.
 * @param hx The shear factor in the horizontal direction.
 * @param hy The shear factor in the vertical direction.
 * @return The sheared image.
 */
Image shear(Image &inimg, float hx, float hy) {
  Image outimg;
  Image H(3,3), IH;   // the translation matrix
  int i, j, k;
  int nr, nc, nt, nchan;
  int x, y;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  outimg.createImage(nr, nc, nt);

  H(0,0) = 1;
  H(0,1) = hx;             // shear along the vertical direction
  H(0,2) = 0;
  H(1,0) = hy;
  H(1,1) = 1;
  H(1,2) = 0;
  H(2,0) = 0;
  H(2,1) = 0;
  H(2,2) = 1;
  
  IH = inverse(H);          // calculate the inverse transform

  // for each (i,j) in the transformed image, find the corresponding point
  // from the original image (inverse transform)
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      x = (int)(i*IH(0,0) + j*IH(0,1) + IH(0,2)*1);
      y = (int)(i*IH(1,0) + j*IH(1,1) + IH(1,2)*1);
      if (x < nr && y < nc && x > 0 && y > 0)
        for (k=0; k<nchan; k++)
          outimg(i,j,k) = inimg(x,y,k);
    }

  return outimg;
}


/**
 * Perspective transformation. The homogeneous perspective transformation 
 * matrix is [a_11 a_12 a_13; a_21 a_22 a_23; a_31 a_32 1]
 * @param inimg The input image.
 * @param tiepoints A 4x4 matrix of coordinates of the four tie points
 *
 *       (x, y) ===== (u, v)
 *       x1, y1       u1, v1, etc.
 * 
 * @return The image after perspective transformation.
 */
Image perspective(Image &inimg, Image &tiepoints) {
  Image outimg;
  Image P(3,3), IP;   // the translation matrix
  int i, j, k;
  int nr, nc, nt, nchan;
  int x, y;
  float xprime, yprime, wprime;
  Image A, C, B, IA;  // used to calculate the P matrix using LS approach
  int tnr;            // nr of tie points

  tnr = tiepoints.getRow();
  if (tnr < 4) {
    cout << "Perspective: not enough tie points.\n";
    exit(3);
  }
  A.createImage(tnr*2, 8);        // AC=B where C is the P coefficients       
  C.createImage(8, 1);
  B.createImage(tnr*2, 1);

  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();
  outimg.createImage(nr, nc, nt);
   
  // find the 8 coefficients of projection matrix P, by solving AC=B
  // where C is a lexicographical representation of P
  for (i=0; i<tnr; i++) {
    A(2*i,0) = tiepoints(i,0);
    A(2*i,1) = tiepoints(i,1);
    A(2*i,2) = 1;
    A(2*i,3) = 0;
    A(2*i,4) = 0;
    A(2*i,5) = 0;
    A(2*i,6) = -tiepoints(i,0) * tiepoints(i,2);
    A(2*i,7) = -tiepoints(i,1) * tiepoints(i,2);
    A(2*i+1,0) = 0;
    A(2*i+1,1) = 0;
    A(2*i+1,2) = 0;
    A(2*i+1,3) = tiepoints(i,0);
    A(2*i+1,4) = tiepoints(i,1);
    A(2*i+1,5) = 1;
    A(2*i+1,6) = -tiepoints(i,0) * tiepoints(i,3);
    A(2*i+1,7) = -tiepoints(i,1) * tiepoints(i,3);
  }
  for (i=0; i<tnr; i++) {
    B(2*i,0) = tiepoints(i,2);
    B(2*i+1,0) = tiepoints(i,3);
  }
  
  if (IA.getRow() == IA.getCol())
    IA = inverse(A);
  else
    IA = pinv(A);         // pseudo inverse
  
  C = IA ->* B;
  
  P(0,0) = C(0,0);
  P(0,1) = C(1,0);
  P(0,2) = C(2,0);
  P(1,0) = C(3,0);
  P(1,1) = C(4,0);
  P(1,2) = C(5,0);
  P(2,0) = C(6,0);
  P(2,1) = C(7,0);
  P(2,2) = 1;

  IP = inverse(P);          // calculate the inverse transform

  // for each (i,j) in the transformed image, find the corresponding point
  // from the original image (inverse transform)
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      xprime = i*IP(0,0) + j*IP(0,1) + IP(0,2)*1;
      yprime = i*IP(1,0) + j*IP(1,1) + IP(1,2)*1;
      wprime = i*IP(2,0) + j*IP(2,1) + IP(2,2)*1;
      x = (int)(xprime/wprime);
      y = (int)(yprime/wprime);
      if (x < nr && y < nc && x > 0 && y > 0)
        for (k=0; k<nchan; k++)
          outimg(i,j,k) = inimg(x,y,k);
    }

  return outimg;
}

