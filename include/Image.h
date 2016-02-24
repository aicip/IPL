/********************************************************************
 * Image.h - header file of the Image library which defines 
 *           a new class "Image" and the associated member functions
 *
 * Author: Hairong Qi, hqi@utk.edu, ECE, University of Tennessee
 *
 * Created: 02/05/02
 *
 * Note: 
 *   This is a simple C++ library for image processing. 
 *   The purpose is not high performance, but to show how 
 *   the algorithm works through programming.
 *   This library can only read in PGM/PPM format images. 
 *
 * Modification:
 *   07/31/09 - moving header files for colorProcessing, imageIO, and
 *               matrixProcessing to this file
 *   01/22/06 - reorganize the Image library such that the Image class
 *              only contains member functions related to the most 
 *              fundamental image operation
 *   11/12/05 - add wavelet transform function
 *   09/26/05 - add Fourier transform related functions
 *   09/07/05 - add overloading function for "/"
 *   09/07/05 - modify createImage() function
 *   09/07/05 - fix problems with copy constructor
 *   08/07/05 - regrouping functions
 ********************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cmath>

using namespace std;

#define PGMRAW   1                     // magic number is 'P5'
#define PPMRAW   2                     // magic number is 'P6'
#define PGMASCII 3                     // magic number is 'P2'
#define PPMASCII 4                     // magic number is 'P3'
#define GRAY     10                    // gray-level image
#define BINARY   11                    // binary image

#define NBIT 8         
#define L ( pow(2.0,NBIT)-1 )    // the largest intensity represented by NBIT

class Image {
  friend ostream & operator<<(ostream &, Image &);
  friend Image operator/(Image &, double);    // image divided by a scalar
  friend Image operator*(Image &, double);    // image multiplied by a scalar
  friend Image operator+(Image &, double);    // image add a scalar
  friend Image operator-(Image &, double);    // image subtract a scalar
    
 public:
  // constructors and destructor
  Image();                             // default constructor 
  Image(int,                           // constructor with row
    	int,                           // column
    	int t=PGMRAW);                 // type (use PGMRAW, PPMRAW, 
                                       // PGMASCII, PPMASCII)
  Image(const Image &);                // copy constructor 
  ~Image();                            // destructor 

  // create an image
  void createImage();                  // create an image, parameters all set
  void createImage(int,                // create an image with row
		   int c=1,            // column (default 1, a column vector)
		   int t=PGMRAW);      // and type, default is PGMRAW
  void initImage(float init=0.0);      // initiate the pixel value of an img
                                       // the default is 0.0

  // get and set functions
  int getRow() const;                  // get row # / the height of the img 
  int getCol() const;                  // get col # / the width of the image 
  int getChannel() const;              // get channel number of the image
  int getType() const;                 // get the image type 
  float getMaximum() const;            // get the maximum pixel value
  void getMaximum(float &,             // return the maximum pixel value
		  int &, int &);       // and its indices
  float getMinimum() const;            // get the mininum pixel value
  void getMinimum(float &,             // return the minimum pixel value
		  int &, int &);       // and its indices
  Image getRed() const;                // get the red channel
  Image getGreen() const;              // get the green channel
  Image getBlue() const;               // get the blue channel
  Image getImage(int) const;           // get the kth channel image, 
                                       // k starts at 0

  void setRow(int);                    // set row number 
  void setCol(int);                    // set column number 
  void setChannel(int);                // set the number of channel
  void setType(int t=PGMRAW);          // set the image type
  void setRed(Image &);                // set the red channel
  void setGreen(Image &);              // set the green channel
  void setBlue(Image &);               // set the blue channel
  void setImage(Image &, int);         // set the kth channel image,
                                       // k starts at 0
                                       
  // operator overloading functions
  float & operator()(int,                  // operator overloading (i,j,k)
		     int c = 0,            // when c=k=0, a column vector 
		     int k = 0) const;     
  const Image operator=(const Image &);    // = operator overloading
  Image operator+(const Image &) const;    // overloading + operator
  Image operator-(const Image &) const;    // overloading - operator
  Image operator*(const Image &) const;    // overloading pixelwise *
  Image operator/(const Image &) const;    // overloading pixelwise division
  Image operator->*(const Image &) const;  // overloading ->* operator 
                                           // (matrix multiplication) 

  bool IsEmpty() const { return (image==NULL); }

 private:
  int row;                  // number of rows / height 
  int col;                  // number of columns / width 
  int channel;              // nr of channels (1 for gray, 3 for color)
  int type;                 // image type (PGM, PPM, etc.)
  int maximum;              // the maximum pixel value
  float *image;             // image buffer
};


////////////////////////////////////
// image I/O
Image readImage(char *);             // read image
void writeImage(Image &,             // write an image
                char *,
                int flag=0);         // flag for rescale, rescale when == 1
Image rescale(Image &,               // rescale an image
              float a=0.0,           // lower bound
              float b=L);            // upper bound


////////////////////////////////////
// color processing routines
Image RGB2HSI(Image &);              // convert from RGB to HSI model
Image HSI2RGB(Image &);              // convert from HSI to RGB model


////////////////////////////////////
// matrix manipulation
Image transpose(Image &);            // image transpose
Image inverse(Image &);              // image inverse
Image pinv(Image &);                 // image pseudo-inverse
Image subImage(Image &,              // crop an image
               int,                  // starting row index
               int,                  // starting column index
               int,                  // ending row index
               int);                 // ending column index


#endif
