/**********************************************************
 * edgeDetection.cpp - contains simple edge detectors
 *
 *   - prewitt: 1st derivative
 *   - roberts: 1st derivative
 *   - sobel: 1st derivative
 *   - laplacian: 2nd derivative
 *   - quadratic: 2nd derivative
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/24/06
 *
 * Modified:
 *   - 02/12/06: added quadratic variation
 *   - 02/12/06: modified laplacian() such that abs(img) is 
 *               returned instead of img
 **********************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;


/**
 * Prewitt edge detector.
 *        mask 1                mask 2
 *        -1  0  1             -1  -1  -1
 *        -1  0  1              0   0   0
 *        -1  0  1              1   1   1
 * @param inimg The input image
 * @return The edge image using the Prewitt kernels
 */
Image prewitt(Image &inimg) {
  Image outimg, outimg1, outimg2, mask1, mask2;

  // create the two masks and initialize to zero  
  mask1.createImage(3,3);
  mask2.createImage(3,3);

  mask1(0,0) = mask1(1,0) = mask1(2,0) = -1;
  mask1(0,2) = mask1(1,2) = mask1(2,2) = 1;

  mask2 = transpose(mask1);       

  // apply kernel operation
  outimg1 = conv(inimg, mask1);  // edge in the vertical direction
  outimg2 = conv(inimg, mask2);  // edge in the horizontal direction
  
  // combine edges in both directions
  Image temp = outimg1 * outimg1 + outimg2 * outimg2;
  outimg = sqrt(temp);
      
  return outimg;
}
 
 
/**
 * Roberts edge detector.
 *        mask 1                mask 2
 *        1   0                 0   1
 *        0  -1                 -1  0
 * @param inimg The input image
 * @return The edge image using the Roberts kernels
 */
Image roberts(Image &inimg) {
  Image outimg, outimg1, outimg2, mask1, mask2;

  // create the mask  
  mask1.createImage(2,2);
  mask2.createImage(2,2);
  mask1(0,0) = 1;
  mask1(1,1) = -1;
  mask2(0,1) = 1;
  mask2(1,0) = -1;

  // apply kernel operation
  outimg1 = conv(inimg, mask1);  // edge in the 135 degree direction
  outimg2 = conv(inimg, mask2);  // edge in the 45 degree direction
  
  // combine edges in both directions
  Image temp = outimg1 * outimg1 + outimg2 * outimg2;
  outimg = sqrt(temp);

  return outimg;
}


/**
 * Sobel edge detector
 *        mask 1                 mask 2
 *        -1  0  1             -1  -2  -1
 *        -2  0  2              0   0   0
 *        -1  0  1              1   2   1
 * @param inimg The input image
 * @return The edge image using the Sobel kernels
 */
Image sobel(Image &inimg) {
  Image outimg, outimg1, outimg2, mask1, mask2;

  // create the masks
  mask1.createImage(3,3);
  mask2.createImage(3,3);

  mask1(0,0) = mask1(2,0) = -1;
  mask1(0,2) = mask1(2,2) = 1;
  mask1(1,0) = -2;
  mask1(1,2) = 2;

  mask2 = transpose(mask1);

  // kernel operation
  outimg1 = conv(inimg, mask1);  // vertical edge
  outimg2 = conv(inimg, mask2);  // horizontal edge
  
  // combine edges in both directions
  Image temp = outimg1 * outimg1 + outimg2 * outimg2;
  outimg = sqrt(temp);
        
  return outimg;
}


/**
 * Sobel edge detector
 *        mask 1                 mask 2
 *        -1  0  1             -1  -2  -1
 *        -2  0  2              0   0   0
 *        -1  0  1              1   2   1
 * An overloading function to sobel() such that both the magnitude
 * and the gradient of the edge will be returned
 *
 * @param inimg The input image.
 * @param mag The magnitude of the edge.
 * @param gradient The direction of the edge.
 */
void sobel(Image &inimg, Image &mag, Image &gradient) {
  Image outimg, outimg1, outimg2, mask1, mask2, edge;

  // create the masks
  mask1.createImage(3,3);
  mask2.createImage(3,3);

  mask1(0,0) = mask1(2,0) = -1;
  mask1(0,2) = mask1(2,2) = 1;
  mask1(1,0) = -2;
  mask1(1,2) = 2;

  mask2 = transpose(mask1);

  // kernel operation
  outimg1 = conv(inimg, mask1);  // vertical edge
  outimg2 = conv(inimg, mask2);  // horizontal edge
  
  // combine edges in both directions
  Image temp = outimg1 * outimg1 + outimg2 * outimg2;
  edge = sqrt(temp);
  
  // calculate gradient
  gradient = outimg2 / outimg1;
  
  // call auto thresholding to generate binary image
  mag = autoThreshold(edge);
}


/**
 * Laplacian edge detector
 * @param inimg The input image
 * @param nt The type of the Laplacian mask.
 * When nt = 1, [0 1 0; 1 -4 1; 0 1 0] (default)
 * When nt = 2, [1 1 1; 1 -8 1; 1 1 1]
 * When nt = 3, [-1 2 -1; 2 -4 2; -1 2 -1].
 * @return
 **********************************************************************/
Image laplacian(Image &inimg, int nt) {
  Image outimg, mask;

  // create the mask  
  mask.createImage(3,3);
  switch (nt) {
  case 1:
    mask(0,1) = mask(1,0) = mask(1,2) = mask(2,1) = 1;
    mask(1,1) = -4;
    break;
  case 2:
    mask(0,0) = mask(0,1) = mask(0,2) = mask(1,0) = mask(1,2) = 1;
    mask(2,0) = mask(2,1) = mask(2,2) = 1;
    mask(1,1) = -8;
    break;
  case 3:
    mask(0,0) = mask(0,2) = mask(2,0) = mask(2,2) = -1;
    mask(0,1) = mask(1,0) = mask(1,2) = mask(2,1) = 2;
    mask(1,1) = -4;  
    break;
  default:
    cout << "laplacian: the mask type is wrong. Choose between 1 and 3\n";
  }
   
  outimg = conv(inimg, mask);  
  outimg = zeroCrossing(outimg);  
    
  return abs(outimg);
}


/**
 * Quadratic edge detector
 *   qxx = [0 0 0; 1 -2 1; 0 0 0]/sqrt(6)
 *   qyy = [0 1 0; 0 -2 0; 0 1 0]/sqrt(6)
 *   qxy = [-1 0 1; 0 0 0; 1 0 -1]/2
 *   Q = qxx^2 + qyy^2 + 2*qxy^2
 * @param inimg The input image
 * @return The quadratic variation of the image
 **********************************************************************/
Image quadratic(Image &inimg) {
  Image qxx, qyy, qxy;
  Image inxx, inyy, inxy;

  // create the mask  
  qxx.createImage(1,3);        // only one row is nonzero, thus a row vector
  qyy.createImage(3,1);        // a column vector
  qxy.createImage(3,3);
  qxx(0,0) = qxx(0,2) = 1.0/sqrt(6.0); 
  qxx(0,1) = -2.0/sqrt(6.0);
  qyy = transpose(qxx);
  qxy(0,0) = qxy(2,2) = -0.25;
  qxy(0,2) = qxy(2,0) = 0.25;
  
  // calculate the convolution   
  inxx = conv(inimg, qxx);
//  inxx = inxx * inxx;  
  inxx = abs(inxx);
  inyy = conv(inimg, qyy);
//  inyy = inyy * inyy;
  inyy = abs(inyy);
  inxy = conv(inimg, qxy);
//  inxy = inxy * inxy;
  inxy = abs(inxy);
  inxy = inxy * 2;
  
  // calculate the final quadratic variation
  // (note that because images are passed to a function using reference 
  // but returned from a function as Image, cascading the operators wouldn't
  // work.)
  inxx = inxx + inyy;
  inxx = inxx + inxy;
  
  return inxx;
}

