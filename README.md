# README #

The Image Processing and Machine Vision Library (IPL)
Electrical Engineering and Computer Science Department
University of Tennessee (UT)

Note: This library was initially created in the Fall semester of 2000, used by both the Digital Image Processing and the Pattern Recognition class offered at UT. The goal is to set up a simple programming structure for students to develop and practice algorithms studied in class.  

The author thanks the contribution from all students over the years. 
For bugs and comments, please contact Hairong Qi, hqi@utk.edu.

The library contains the following files:

README - this file

\include - header files\\
  Image.h: defines the new Image class.\\
  Dip.h: declares various functions for DIP and MV\\
  Map.h: contains parameters used for MAP\\

\lib - library files
  Makefile: makefile to build the image library
  wt.cpp: 1-D and 2-D wavelet transform
      (daub4, wt1d, wt2d)
  pointProcessing.cpp: point-based image enhancement routines
      (negative, cs, logtran, powerlaw, threshold, bitplane, 
      sampling, quantization, histeq)
  addNoise.cpp: adds different noise distribution to an image
      (gaussianNoise, sapNoise)
  conv.cpp: kernel operation
      (conv)
  lowpassFilter.cpp: low-pass filters (linear and nonlinear)
      (average, gaussianSmooth, median, contrah, gmean, amedian)
  freqFilter.cpp: frequency-domain filters
      (ideal, butterworth, Gaussian, inverse, wiener)
  edgeDetection.cpp: simple edge detectors
      (prewitt, roberts, sobel, laplacian)
  marr.cpp: Marr-Hildreth edge detector
      (marr, LoG, zeroCrossing)
  canny.cpp: Canny edge detector
      (canny, nonmax, hThreshold, estThreshold, DoGX, DoGY, gaussianKernel)
  morph.cpp: morphological operators
      (dilate, erode, open, close)
  transform.cpp: various affine transforms and perspective transform
      (rotate, scale, shear, translate, perspective)
  geocorr.cpp: geometric correction using 2nd degree polynomial approximation
  colorProcessing.cpp: color processing routines
      (RGB2HSI, HSI2RGB)
  imageIO.cpp: image read/write
      (readImage, writeImage, rescale)
  matrixProcessing.cpp: matrix manipulation routines
      (transpose, inverse, pinv, subImage)
  utility.cpp: commonly used utility routines
      (bubblesort, gaussrand, psnr, power, sqrt)
  mapmfa.cpp: use mfa to solve map
      (mfamap, linear)

\example - test codes
  Makefile: to compile all the test codes
  testImage.cpp: test code for the Image class.
  testmatrixProcessing.cpp: test code for matrix processing algorithms.
  testpointProcessing.cpp: test code for all point-based enhancement algorithms.
  testedgeDetection.cpp: test code for both high-pass and low-pass filters
  testaddNoise.cpp: test code for add noise functions and psnr
  testcolorProcessing.cpp: test code for color model conversion routines
  testMorph.cpp: test code for morphological operators
  testmap.cpp: test code for mapmfa
