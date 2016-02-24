/********************************************************************
 * Dip.h - header file of the Image processing library
 *
 * Author: Hairong Qi, hqi@utk.edu, ECE, University of Tennessee
 *
 * Created: 01/22/06
 *
 * Modification:
 ********************************************************************/

#ifndef DIP_H
#define DIP_H

#include "Image.h"
#include "Map.h"

#define PI 3.1415926

// flag for lowpass or highpass filter 
#define HIGH 1
#define LOW 0

//////////////////////////////////////////////
// point-based image enhancement processing

Image negative(Image &);             // image negative
Image cs(Image &,                    // contrast stretching 
         float,                      // slope
         float);                     // intercept
Image logtran(Image &);              // dynamic range compression
Image powerlaw(Image &,              // power-law transformation
               float);               // the gamma value
Image threshold(Image &,             // thresholding an image
                float,               // the threshold
                int nt=BINARY);      // binary or grey-level thresholding
                                     // use "BINARY" or "GRAY"
Image threshold(Image &,             // thresholding an image
                float,               // the lower threshold
		float,               // the upper threshold
                int nt=BINARY);      // binary or grey-level thresholding
                                     // use "BINARY" or "GRAY"
Image autoThreshold(Image &);        // use Otsu's automatic thresholding
void bitplane(Image &, Image []);    // bitplane slicing
Image sampling(Image &,              // downsample the image
               int);                 // the sampling ratio, should be > 1
Image quantization(Image &,          // quantize the image 
                   int);             // number of quantization levels
Image histeq(Image &);               // histogram equalization

// add various types of noise to the image
Image gaussianNoise(Image &inimg,    // add Gaussian noise
                    float std,       // standard deviation of Gaussian
                    float mean=0.0); // mean of Gaussian, default is zero
Image sapNoise(Image &,              // add salt and pepper noise
               float);               // probability


/////////////////////////////////////
// neighbor-based processing

Image conv(Image &,                  // convolution between an image
           Image &);                 // and a mask image (kernel operation)

// low-pass filters
Image average(Image &,               // average lowpass filter
              int size=3);           // masksize (assume square mask)
Image gmean(Image &,                 // geometric mean
	    int size=3);             // masksize (assume square mask)
Image gaussianSmooth(Image &);       // Gaussian lowpass filter
Image median(Image &,                // median filter
             int size=3);            // masksize (assume square mask)
Image amedian(Image &,               // adaptive median filter
	      int size=7);           // maximum mask size
Image contrah(Image &,               // contraharmonic filter
              float,                 // the order of the filter
              int size=3);           // the size (radius) of neighborhood

// edge detectors
Image roberts(Image &);              // Roberts edge detector
Image sobel(Image &);                // Sobel edge detector
void sobel(Image &img,               // Sobel edge detector
           Image &mag,               // edge magnitude
           Image &gradient);         // edge direction
Image prewitt(Image &);              // Prewitt edge detector
Image laplacian(Image &,             // Laplacian edge detector
                int nt = 1);         // use different types of kernel
                                     // 1 - [0 1 0; 1 -4 1; 0 1 0]
                                     // 2 - [1 1 1; 1 -8 1; 1 1 1]
                                     // 3 - [-1 2 -1; 2 -4 2; -1 2 -1]
Image quadratic(Image &);            // quadratic variation

// frequency-domain filters
Image ideal(Image &inimg,            // the ideal filter
	    int flag,                // HIGH for highpass, LOW for lowpass
	    float radius);
Image butterworth(Image &inimg,      // the butterworth filter
		  int flag,          // HIGH for highpass, LOW for lowpass
		  float radius,
		  int n);            // controls the sharpness of the curve
Image gaussianFreq(Image &inimg,     // the Gaussian filter
		   int flag,         // HIGH for highpass, LOW for lowpass
		   float std);       // standard deviation
Image inverseFilter(Image &inimg,    // the inverse filter
		    float std);      // standard deviation of the blur kernel
Image wiener(Image &inimg,           // the input image
	     float K,                // power of noise / power of signal
	     float std);             // standard deviation of the blur kernel
                                     // assuming it's Gaussian

// The Marr-Hildreth edge detector
Image marr(Image &,                 
           float);                   // std of Gaussian
Image LoG(float);                    // generate the Laplacian of Gaussian
                                     // kernel with certain standard deviation
Image zeroCrossing(Image &);         // find zero crossings in the image

// Canny edge detector
Image canny(Image &,                 // Canny edge detector
            float);                  // std of Gaussian
Image nonmax(Image &,                // nonmax suppression, the edge in x dire
             Image &);               // the edge image in the y direction
Image hThreshold(Image &);           // double thresholding
void estThreshold(Image &,           // estimate the thresholds used above 
                  int *,             // higher threshold
                  int *);            // lower threshold
Image DoGX(float);                   // difference of Gaussian kernel
                                     // check for edge in the vertical direction
Image DoGY(float);                   // DoG in the horizontal direction
Image gaussianKernel(float);         // generate a Gaussian smooth kernel


////////////////////////////////////////////
// image restoration and feature extraction
Image mapmfa(Image &gimg,            // use mean field annealing to solve MAP
             Map &par);              // parameter object
int linear(Image &fimg,              // piece-wise linear surface model
           Image &gimg,              // the measured image
           Map &par);                // parameter
                 

///////////////////////////////////////////
// Hough transform
Image houghLine(Image &img);         // locate a line segment
void houghPara(Image &img);          // locate parabolic curves

                            
////////////////////////////////////
// geometric transformations
Image translate(Image &,             // translation
                float, float);       // tx and ty
Image shear(Image &,                 // shear
            float, float);           // hx and hy
Image scale(Image &,                 // scale
            float, float);           // sx and sy
Image rotate(Image &,                // rotation
             float);                 // angle in degree (counterclock is +)
Image perspective(Image &,           // perspective transform
                  Image &);          // coordinates of n tiepoints
                                     // an nx4 matrix with the first two cols
                                     // coordinates from the original image
                                     // and the last two cols those of the 
                                     // transformed image
Image geocorr(Image &);              // geometric correction


////////////////////////////////////
// Fourier transform
void fft(Image &inimg,               // forward transform 
	 Image &mag, 
	 Image &phase);
void ifft(Image &outimg,             // inverse transform
	  Image &mag,
	  Image &phase);
void fftifft(Image &inimg,           // FFT
	     Image &mag,
	     Image &phase,
	     int scale);             // 1: forward trans; -1: inverse trans


////////////////////////////////////
// wavelet transform
Image wt2d(Image &,                  // the 2-D image
	   int,                      // level of wt decomposition/coefficients
	   int);                     // forward transform (>=0) or inverse (-1)
Image wt1d(Image &,                  // the 1-D row vector
	   int);                     // forward transform (>=0) or inverse (-1)
void daub4(Image &,                  // the 1-D row vector 
	   int,                      // level of transform
	   int);                     // forward transform (>=0) or inverse (-1)


////////////////////////////////////
// morphological operators
Image dilate(Image &,                // the original image
             Image &,                // the structuring element
             int origRow=0,          // row coordinate of the origin of s.e.
             int origCol=0,          // col coordinate of the origin of s.e.
             int nt=BINARY);         // type of image, BINARY or GRAY
Image erode(Image &,                 // the original image
            Image &,                 // the structuring element
            int origRow=0,           // row coordinate of the origin of s.e.
            int origCol=0,           // col coordinate of the origin of s.e.
            int nt=BINARY);          // type of image, BINARY or GRAY
Image open(Image &,                  // the original image
           Image &,                  // the structuring element
           int origRow=0,            // row coordinate of the origin of s.e.
           int origCol=0,            // col coordinate of the origin of s.e.
           int nt=BINARY);           // type of image, BINARY or GRAY
Image close(Image &,                 // the original image
            Image &,                 // the structuring element
            int origRow=0,           // row coordinate of the origin of s.e.
            int origCol=0,           // col coordinate of the origin of s.e.
            int nt=BINARY);          // type of image, BINARY or GRAY
Image bdilate(Image &img, Image &se, // binary dilation
              int origRow, int origCol);
Image berode(Image &img, Image &se,  // binary erosion
             int origRow, int origCol);
Image gdilate(Image &img, Image &se, // gray-scale dilation
              int origRow, int origCol);
Image gerode(Image &img, Image &se,  // gray-scale erosion
             int origRow, int origCol);
             

////////////////////////////////////
// color processing routines
Image RGB2HSI(Image &);              // convert from RGB to HSI model
Image HSI2RGB(Image &);              // convert from HSI to RGB model
 

////////////////////////////////////
// other utility functions
float psnr(Image &,                  // peak SNR, original image
           Image &);                 // degraded image
float rmse(Image &,                  // root mean square error, original image
	   Image &);                 // degraded image
double gaussrand();                  // generate a Gaussian random # (-3,3)
void bubblesort(float *,             // bubblesort, pointer to the array
                int);                // size of the array
void bubblesort(float *,             // bubblesort, pointer to the array
                int,                 // size of the array
                int *p);             // pointer to the index
float power(Image &);                // return the power of an image
Image sqrt(Image &);                 // function overloading of sqrt()
                                     // pixel-wise processing
Image abs(Image &);                  // overloading function abs(), pixel-wise
float norm(float, float);            // calculate magnitude
float sum(Image &img);               // summation of all pixel intensities
void outofMemory();                  // print error message
int floorPower2(int);                // the largested power of 2 <= the given

#endif
