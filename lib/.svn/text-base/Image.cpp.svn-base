/**********************************************************
 * Image.cpp - the image library which implements
 *             the member functions defined in Image.h
 *
 * Author: Hairong Qi, ECE, University of Tennessee
 *
 * Created: 02/05/02
 *
 * Copyright (C) hqi@utk.edu
 **********************************************************/

#include "Image.h"
#include "Dip.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <cmath>
using namespace std;


/**
 * Default constructor.
 */ 
Image::Image() {
  image = 0;
  createImage(0, 0);
}

/**
 * Constructor for grayscale/color images.
 * @param r Numbers of rows (height).
 * @param c Number of columns (width).
 * @param t Type of image to be created (see below for types). 
 * @see PGMRAW.
 * @see PGMASCII.
 * @see PPMRAW.
 * @see PPMASCII.
 * @return The created image.
 */
Image::Image(int r, int c, int t) {
  if (r<=0 || c<=0) {
    cout << "Image: Index out of range.\n";
    exit(3);
  }
  image = 0;
  createImage(r, c, t);
}

/**
 * Copy constructor. 
 * @param img Copy image.
 * @return The created image.
 */
Image::Image(const Image &img) {
  int i, j, k;

  image = 0;
  row = img.getRow();
  col = img.getCol();
  type = img.getType();
  if (type == PGMRAW || type == PGMASCII)
    channel = 1;
  else if (type == PPMRAW || type == PPMASCII)
    channel = 3;
  else
    cout << "Copy constructor: Undefined image type!\n";

  createImage(row, col, type);             // allocate memory
  
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      for (k=0; k<channel; k++)
	image[(i*col+j)*channel+k] = img(i,j,k);
}

/**
 * Destructor.  Frees memory.
 */
Image::~Image() {
  if (image)
    delete [] image;       // free the image buffer
}

/**
 * Allocate memory for the image and initialize the content to be 0.
 */
void Image::createImage() {

  if (image)
    delete [] image;

  if (type == PGMRAW || type == PGMASCII)
    channel = 1;
  else if (type == PPMRAW || type == PPMASCII)
    channel = 3;
  else
    cout << "createImage: Undefined image type!\n";
  maximum = 255;

  image = (float *) new float [row * col * channel];
  if (!image) {
    cout << "CREATEIMAGE: Out of memory.\n";
    exit(1);
  }

  initImage();
}

/**
 * Allocate memory for the image and initialize the content to be zero.
 * @param r Numbers of rows (height).
 * @param c Number of columns (width).
 * @param t Type of image to be created (see below for types). Default is
 * PGMRAW.
 * @see PGMRAW.
 * @see PGMASCII.
 * @see PPMRAW.
 * @see PPMASCII.
 */
void Image::createImage(int r, int c, int t) {
  
  if (image)
    delete [] image;

  row = r;
  col = c;
  type = t;
  if (type == PGMRAW || type == PGMASCII)
    channel = 1;
  else if (type == PPMRAW || type == PPMASCII)
    channel = 3;
  else
    cout << "createImage: Undefined image type!\n";
  maximum = 255;

  image = (float *) new float [row * col * channel];
  if (!image) {
    cout << "CREATEIMAGE: Out of memory.\n";
    exit(1);
  }

  initImage();
}

/**
 * Initialize the image.
 * @para init The value the image is initialized to. Default is 0.0.
 */
void Image::initImage(float init) {
  int i;

  for (i=0; i<row*col*channel; i++)
    image[i] = init;
}

/**
 * Returns the total number of rows in the image.
 * @return Total number of rows.
 * \ingroup getset
 */
int Image::getRow() const {
  return row;
}

/**
 * Returns the total number of columns in the image.
 * @return Total number of columns.
 * \ingroup getset
 */
int Image::getCol() const {
  return col;
}

/**
 * Returns the total number of channels in the image.
 * @return Total number of channels.
 * \ingroup getset
 */
int Image::getChannel() const {
  return channel;
}

/**
 * Returns the type of the image.
 * For example, ppm, pgm, etc.
 * @return The type of the image.
 * \ingroup getset
 */
int Image::getType() const {
  return type;
}

/**
 * Returns the maximum pixel value of a gray-level image. 
 * @return The intensity of that pixel.
 * \ingroup getset
 */
float Image::getMaximum() const {
  int i, j;
  float maxi=0.0;

  if (channel > 1) {
    cout << "GETMAXIMUM: Can only handle single-channel image\n";
    exit(3);
  }
  
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      if (maxi < image[i*col+j])
	maxi = image[i*col+j];
  
  return maxi;
}


/**
 * Returns the maximum pixel value of a gray-scale image,
 * as well as its indices 
 * @return the maximum intensity, its row and column indices
 * \ingroup getset
 */
void Image::getMaximum(float &intensity, int &ix, int &iy) {
  int i, j;

  intensity = 0;
  ix = iy = 0;

  if (channel > 1) {
    cout << "GETMAXIMUM: Can only handle single-channel image\n";
    exit(3);
  }
    
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      if (intensity < image[i*col+j]) {
        intensity = image[i*col+j];
        ix = j;
        iy = i;
      }
}


/**
 * Returns the minimum pixel value of the image.
 * @return The minimum pixel value.
 * \ingroup getset
 */
float Image::getMinimum() const {
  int i, j;
  float mini=256; 

  if (channel > 1) {
    cout << "GETMINIMUM: Can only handle single-channel image\n";
    exit(3);
  }

  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      if (mini > image[i*col+j])
	mini = image[i*col+j];

  return mini;
}


/**
 * Returns the minimum pixel value of a gray-scale image,
 * as well as its indices 
 * @return the miniimum intensity, its row and column indices
 * \ingroup getset
 */
void Image::getMinimum(float &intensity, int &ix, int &iy) {
  int i, j;

  intensity = 256.0;
  ix = iy = 0;

  if (channel > 1) {
    cout << "GETMINIMUM: Can only handle single-channel image\n";
    exit(3);
  }
    
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      if (intensity > image[i*col+j]) {
        intensity = image[i*col+j];
        ix = j;
        iy = i;
      }
}

/**
 * Returns the red channel of a color image.  If the original image is not
 * 3 channels (RGB), then the function will error.
 * @return Grayscale image representing the red channel of a RGB image.
 * \ingroup getset
 */
Image Image::getRed() const  {
  Image temp;

  temp = getImage(0);
  
  return temp;
}

/**
 * Returns the green channel of a color image.  If the original image is not
 * 3 channels (RGB), then the function will error.
 * @return Grayscale image representing the green channel of a RGB image.
 * \ingroup getset
 */
Image Image::getGreen() const {
  Image temp;

  temp = getImage(1);
  
  return temp;
}

/**
 * Returns the blue channel of a color image.  If the original image is not
 * 3 channels (RGB), then the function will error.
 * @return Grayscale image representing the blue channel of a RGB image.
 * \ingroup getset
 */
Image Image::getBlue() const {
  Image temp;
  
  temp = getImage(2);

  return temp;
}

/**
 * Returns the kth channel of the image. If k is outside [0, channel-1], 
 * then the function will error.
 * @param k The channel number. Starts at 0.
 * @return The kth channel as a gray-scale image
 * \ingroup getset
 */
Image Image::getImage(int k) const {
  Image temp;
  int i, j;
  
  if (k>=channel || k<0) {
    cout << "getImage: Check the value of the input channel. \n"
         << "\tThe value should be within [0, " << channel-1 << "]\n";
    exit(3);
  }
  
  temp.createImage(row, col);   // temp is a gray-scale image
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      temp(i,j) = image[(i*col+j)*channel+k];
      
  return temp;
}

/**
 * Sets the total number of rows in an image.
 * @param r Total number of rows.
 * \ingroup getset
 */
void Image::setRow(int r) {
  row = r;
}

/**
 * Sets the total number of columns in an image.
 * @param c Total number of columns.
 * \ingroup getset
 */
void Image::setCol(int c) {
  col = c;
}

/**
 * Sets the total number of channels in an image.
 * @param c Total number of channels.
 * \ingroup getset
 */
void Image::setChannel(int c)
{
  channel = c;
}

/**
 * Sets the image type.  (see below for types). Default is PGMRAW.
 * @see PGMRAW.
 * @see PGMASCII.
 * @see PPMRAW.
 * @see PPMASCII.
 * @param t The type of image desired.
 * \ingroup getset
 */
void Image::setType(int t) {
  type = t;
  if (t == PGMRAW || t == PGMASCII)
    channel = 1;
  else if (t == PPMRAW || t == PPMASCII)
    channel = 3;
  else
    cout << "setType: Undefined image type!\n";
}

/**
 * Sets the red channel of a color image given a grayscale image
 * that represents the red channel.
 * @param red Grayscale image to replace the current red channel.
 * \ingroup getset
 */
void Image::setRed(Image &red) {
  setImage(red, 0);
}

/**
 * Sets the green channel of a color image given a grayscale image
 * that represents the green channel.
 * @param green Grayscale image to replace the current green channel.
 * \ingroup getset
 */
void Image::setGreen(Image &green) {
  setImage(green, 1);
}

/**
 * Sets the blue channel of a color image given a grayscale image
 * that represents the blue channel.
 * @param blue Grayscale image to replace the current blue channel.
 * \ingroup getset
 */
void Image::setBlue(Image &blue) {
  setImage(blue, 2);
}

/**
 * Sets the kth channel of the image given a grayscale image. 
 * If k is outside [0, channel-1], then the function will error.
 * @param img Grayscale image to replace the specified channel of the image.
 * @param k The channel number. Starts at 0.
 * \ingroup getset
 */
void Image::setImage(Image &img, int k) {
  int i, j;

  if (channel < k+1) {
    cout << "setImage: The image does not have the specified channel.\n";
    exit(3);
  }
  
  if (img.getChannel() != 1) {
  	cout << "setImage: The input image should be grayscale.\n";
  	exit(3);
  }

  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      image[(i*col+j)*channel+k] = img(i,j);
}

/**
 * Overloading () operator
 * \ingroup overload
 * @param i Row
 * @param j Column
 * @param k Channel
 */
float & Image::operator()(int i, int j, int k) const {
  return image[(i*col+j)*channel+k];
}

/**
 * Overloading = operator.
 * \ingroup overload
 * @param img Image to copy.
 * @return Newly copied image.
 */
const Image Image::operator=(const Image& img) {
  int i, j, k;

  if (this == &img)
    return *this;

  // double delete, bug found by Brian Bodkin of class Fall 2009
  if (image) {
    delete [] image;
    image = 0;
  }

  row = img.getRow();
  col = img.getCol();
  channel = img.getChannel();
  type = img.getType();
  createImage(row, col, type);             

  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      for (k=0; k<channel; k++)
	(*this)(i,j,k) = img(i,j,k);
  //        image[(i*col+j)*channel+k] = img(i,j,k);

  return *this;
}

/**
 * Overloading + operator.
 * \ingroup overload
 * @param img Image to add to specified image.
 * @return Addition of the two images.
 */
Image Image::operator+(const Image& img) const {
  int i, j, k, nr, nc, nchan, nt;
  Image temp;

  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  if (nr != row || nc != col || nchan != channel || nt != type) {
    cout << "operator+: "
         << "Images are not of the same size or type, can't do addition\n";
    exit(3);
  }
  temp.createImage(row, col, type);             
  
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      for (k=0; k<channel; k++)
        temp(i,j,k) = image[(i*col+j)*channel+k] + img(i,j,k);

  return temp;
}

/**
 * Overloading - operator.
 * \ingroup overload
 * @param img Image to subtract from specified image.
 * @return Subtraction of the two images.
 */
Image Image::operator-(const Image &img) const {
  int i, j, k, nr, nc, nchan, nt;
  Image temp;

  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  if (nr != row || nc != col || nchan != channel || nt != type) {
    cout << "operator-: "
         << "Images are not of the same size or type, can't do subtraction\n";
    exit(3);
  }
  temp.createImage(row, col, type);             
  
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      for (k=0; k<channel; k++)
        temp(i,j,k) = image[(i*col+j)*channel+k] - img(i,j,k);

  return temp;
}

/**
 * Overloading * operator.  This function does pixel by pixel multiplication.
 * \ingroup overload
 * @param img Image to multiply with specified image.
 * @return Multiplication of the two images.
 */
Image Image::operator*(const Image &img) const {
  int i, j, k, nr, nc, nchan, nt;
  Image temp;

  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  if (nr != row || nc != col || nchan != channel || nt != type) {
    cout << "operator*: Images are not of the same size or type, "
         << "can't do multiplication\n";
    exit(3);
  }
  temp.createImage(row, col, type);
  
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      for (k=0; k<channel; k++)
        temp(i,j,k) = image[(i*col+j)*channel+k] * img(i,j,k);

  return temp;
}

/**
 * Overloading / operator.  This function does pixel by pixel division.
 * Specified image is the dividend.
 * \ingroup overload
 * @param img Image to be divided (divisor).
 * @return Quotient of the two images.
 */
Image Image::operator/(const Image &img) const {
  int i, j, k, nr, nc, nchan, nt;
  Image temp;

  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  if (nr != row || nc != col || nchan != channel || nt != type) {
    cout << "operator/: "
         << "Images are not of the same size or type, can't do division\n";
    exit(3);
  }
  temp.createImage(row, col, type);
  
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      for (k=0; k<channel; k++)
        temp(i,j,k) = image[(i*col+j)*channel+k] / (img(i,j,k)+0.000001);

  return temp;
}

/**
 * Overloading ->* operator.  This function does matrix
 * multiplication of Image files.
 * \ingroup overload
 * @param img Image (matrix) to multiply specified image with.
 * @result Result from doing matrix multiplication.
 */
Image Image::operator->*(const Image &img) const {
  int i, j, k, m, nr, nc, nchan, nt;
  Image temp;
  float tmp;

  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  if (col != nr || nt != type) {
    cout << "operaotr->*: "
         << "Image size is not consistent, can't do multiplication\n";
    exit(3);
  }
  temp.createImage(row, nc, type);
  
  for (k=0; k<channel; k++)
    for (i=0; i<row; i++)
      for (j=0; j<nc; j++) {
        tmp = 0.0;
        for (m=0; m<col; m++) {
          tmp += image[(i*col+m)*channel+k] * img(m,j,k);
          temp(i,j,k) = tmp;
        }
      }

  return temp;
}

/**
 * Overloading << operator.  Output the image to the specified destination.
 * \ingroup overload
 * @param out The specified output stream (or output destination).
 * @param img Image to be output.
 * @result Output image to the specified file destination.
 */
ostream & operator<<(ostream &out, Image &img) {
  int i, j, k;
  
  for (k=0; k<img.getChannel(); k++) {
    out << endl;
    for (i=0; i<img.getRow(); i++) {
      for (j=0; j<img.getCol(); j++)
        out << setw(4) << img(i,j,k) << ' ';
      out << endl;
    }
  }

  return out; 
}

/**
 * Overloading / operator.  The left operand is the image and the right
 * is the dividend (a double point number). Each pixel in the image is 
 * divided by the double point number.
 * \ingroup overload
 * @param img Image as the left operand.
 * @param dev A double point number as the right operand.
 * @result Image divided by a double point number.
 */
Image operator/(Image &img, double dev) {
  int i, j, k, nr, nc, nt, nchan;
  Image temp;
  
  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  temp.createImage(nr, nc, nt);
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        temp(i,j,k) = img(i,j,k) / dev;
  
  return temp;
}

/**
 * Overloading * operator.  The left operand is the image and the right
 * is a double point number. Each pixel in the image is multiplied by the
 * double point number.
 * \ingroup overload
 * @param img Image as the left operand.
 * @param s A double point number as the right operand.
 * @result Image multiplied by a double point scalar.
 */
Image operator*(Image &img, double s) {
  int i, j, k, nr, nc, nt, nchan;
  Image temp;
  
  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  temp.createImage(nr, nc, nt);
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        temp(i,j,k) = img(i,j,k) * s;
  
  return temp;
}


/**
 * Overloading + operator.  The left operand is the image and the right
 * is a double point number. Each pixel in the image is added by the
 * double point number.
 * \ingroup overload
 * @param img Image as the left operand.
 * @param s A double point number as the right operand.
 * @result Image add a double point scalar.
 */
Image operator+(Image &img, double s) {
  int i, j, k, nr, nc, nt, nchan;
  Image temp;
  
  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  temp.createImage(nr, nc, nt);
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        temp(i,j,k) = img(i,j,k) + s;
  
  return temp;
}


/**
 * Overloading - operator.  The left operand is the image and the right
 * is a double point number. Each pixel in the image is subtracted by the
 * double point number.
 * \ingroup overload
 * @param img Image as the left operand.
 * @param s A double point number as the right operand.
 * @result Image subtract a double point scalar.
 */
Image operator-(Image &img, double s) {
  int i, j, k, nr, nc, nt, nchan;
  Image temp;
  
  nr = img.getRow();
  nc = img.getCol();
  nchan = img.getChannel();
  nt = img.getType();
  temp.createImage(nr, nc, nt);
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      for (k=0; k<nchan; k++)
        temp(i,j,k) = img(i,j,k) - s;
  
  return temp;
}

