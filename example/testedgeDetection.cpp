/**********************************************************
 * This is a test program for edgeDetection routines
 * and lowpass routines 
 * 
 * Author: Hairong Qi
 * Date: 01/26/06
 **********************************************************/
 
#include "Image.h"
#include "Dip.h"
#include <iostream>

using namespace std;

#define Usage "./testsedgeDetection\n"

int main()
{
  Image img;      

  img = readImage("../testimage/checkerboard_perfect.pgm");
  
  // test different functions
  Image tmp1 = prewitt(img);
  //  Image tmp98 = threshold(tmp1, 40);
  //  writeImage(tmp98, "checkerboardP-edge_prewitt-thresh40.pgm");
  writeImage(tmp1, "checkerboardP-edge_prewitt.pgm");

  Image tmp2 = roberts(img);
  //  Image tmp99 = threshold(tmp2, 10);
  //writeImage(tmp99, "checkerboardP-edge_roberts-thresh10.pgm");
  writeImage(tmp2, "checkerboardP-edge_roberts.pgm");

  Image tmp3 = sobel(img);
  //  Image tmp97 = threshold(tmp3, 50);
  //  writeImage(tmp98, "checkerboardP-edge_prewitt-thresh40.pgm");
  writeImage(tmp3, "checkerboardP-edge_sobel.pgm");

  Image tmp5 = marr(img, 6.0);
  writeImage(tmp5, "checkerboardP-edge_marr.pgm");
 
  /*
  Image tmp6 = average(img);
  writeImage(tmp6, "lowpass_avg.pgm");
  
  Image tmp7 = gaussianSmooth(img);
  writeImage(tmp7, "lowpass_gaussian.pgm");
  
  Image tmp8 = median(img, 5);
  writeImage(tmp8, "lowpass_median.pgm");
  
  Image tmp9 = contrah(img, 2);
  writeImage(tmp9, "lowpass_contrah.pgm");
  */
  
  Image tmp10 = canny(img, 6.0);
  writeImage(tmp10, "checkerboardP-edge_canny.pgm");
 
  Image tmp4 = laplacian(img, 2);
  //  Image tmp96 = threshold(tmp4, 10);
  //  writeImage(tmp96, "checkerboardP-edge_laplacian-thresh10.pgm");   
  writeImage(tmp4, "checkerboardP-edge_laplacian.pgm");

  /*
  Image tmp11 = quadratic(img);
  writeImage(tmp11, "checkerboardP-edge_quad.pgm");
  */

  return 0;
}
