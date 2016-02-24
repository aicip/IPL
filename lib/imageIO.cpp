/**********************************************************
 * imageIO.cpp - read/write image 
 *               (can only r/w PGM/PPM image so far)
 *
 *   - readImage: read an image from a file
 *   - writeImage: write an image to a file   
 *   - rescale: rescale the pixel value of an image
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/26/06
 *
 * Modified:
 *   - 07/31/09: move rescale() to this file
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


/**
 * Read image from a file                     
 * @param fname The name of the file 
 * @return An Image object
 */
Image readImage(char *fname) {
  ifstream ifp;
  char dummy[80];
  unsigned char *img;
  int i, j, k;
  int tmp;
  Image outimg;
  int nr, nc, nt, nchan, maxi;

  ifp.open(fname, ios::in | ios::binary);

  if (!ifp) {
    cout << "readImage: Can't read image: " << fname << endl;
    exit(1);
  }

  // identify image format
  ifp.getline(dummy, 80, '\n');

  if (dummy[0] == 'P' && dummy[1] == '5') {
    nt = PGMRAW;
    nchan = 1;
  }
  else if (dummy[0] == 'P' && dummy[1] == '6') {
    nt = PPMRAW;
    nchan = 3;
  }
  else if (dummy[0] == 'P' && dummy[1] == '2') {
    nt = PGMASCII;
    nchan = 1;
  }
  else if (dummy[0] == 'P' && dummy[1] == '3') {
    nt = PPMASCII;
    nchan = 3;
  }
  else 
    cout << "readImage: Can't identify image format." << endl;

  // skip the comments
  ifp.getline(dummy, 80, '\n');

  while (dummy[0] == '#') {
    ifp.getline(dummy, 80, '\n');
  }

  // read the row number and column number
  sscanf(dummy, "%d %d", &nc, &nr);

  // read the maximum pixel value
  ifp.getline(dummy, 80, '\n');
  sscanf(dummy, "%d", &maxi); 

  // create the image
  outimg.createImage(nr, nc, nt);

  // read the image data
  img = (unsigned char *) new unsigned char [nr * nc * nchan];
  if (!img) {
    cout << "READIMAGE: Out of memory.\n";
    exit(1);
  }

  // added capability to process ASCII format as well
  if (nt == PGMRAW || nt == PPMRAW) {
    ifp.read((char *)img, (nr * nc * nchan * sizeof(unsigned char)));
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++)
        for (k=0; k<nchan; k++)
          outimg(i,j,k) = (float)img[(i*nc+j)*nchan+k];
  }
  else {   // ASCII formats
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++)
        for (k=0; k<nchan; k++) {
          ifp >> tmp;
          outimg(i,j,k) = (float)tmp;
        }
  }

  ifp.close();
  delete [] img;
  
  return outimg;
}


/**
 * Write image buffer to a file.
 * @param inimg The image to be output.
 * @param fname The output file name.
 * @param flag The rescale flag. Rescale when true.
 */
void writeImage(Image &inimg, char *fname, int flag) {
  ofstream ofp;
  int i, j, k;
  int nr, nc, nchan, nt;
  unsigned char *img;
  Image temp;

  ofp.open(fname, ios::out | ios::binary);

  if (!ofp) {
    cout << "writeImage: Can't write image: " << fname << endl;
    exit(1);
  }

  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();

  // Write the format ID
  switch (nt) {
  case PGMRAW:
    ofp << "P5" << endl;
    break;
  case PPMRAW:
    ofp << "P6" << endl;
    break;
  case PGMASCII:
    ofp << "P2" << endl;
    break;
  case PPMASCII:
    ofp << "P3" << endl;
    break;
  otherwise:
    cout << "writeImage: Can't identify image type\n";
  }

  ofp << nc << " " << nr << endl;

  // if user allow rescale
  if (flag) {
    temp = rescale(inimg);
  }
  else {  // if the maximum has been set, any intensity that is
          // larger than the maximum would be set as maximum
    temp = inimg;
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++)
        for (k=0; k<nchan; k++) {
          if (temp(i,j,k) > L)
            temp(i,j,k) = L;
          if (temp(i,j,k) < 0)
            temp(i,j,k) = 0;
        }
  }
  
  ofp << L << endl;

  // convert the image data type back to unsigned char
  img = (unsigned char *) new unsigned char [nr * nc * nchan];
  if (!img) {
    cout << "WRITEIMAGE: Out of memory.\n";
    exit(1);
  }

  if (nt == PGMRAW || nt == PPMRAW) {
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++)
        for (k=0; k<nchan; k++) 
          img[(i*nc+j)*nchan+k] = (unsigned char)temp(i,j,k);
    ofp.write((char *)img, (nr * nc * nchan * sizeof(unsigned char)));
  }
  else {   // ASCII format
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++)
        for (k=0; k<nchan; k++) 
          ofp << (unsigned int)temp(i,j,k) << endl; 
  }

  ofp.close();
  delete [] img;
}


/** 
 * Rescale the image to be between min and max.
 * @param inimg The input image.
 * @param a The lower bound.
 * @param b The upper bound
 * @return Rescaled image.
 */
Image rescale(Image &inimg, float a, float b) {
  int i, j, k;
  int nr, nc, nt, nchan;
  Image temp;
  float maxi, mini;
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nt = inimg.getType();
  nchan = inimg.getChannel();  
  if (a<0 || b>L || a>b) {
    cout << "rescale: the specified min and max need to be " 
         << "between [0, " << L << "]\n"
         << "\tand min should be less than or equal to max\n";
    exit(3);
  }
  if (nchan>1) {
    cout << "rescale: can only handle single-channel image\n";
    exit(3);
  }
  temp.createImage(nr, nc, nt);
     
  // get the maximum and minimum of each channel
  maxi = inimg.getMaximum();
  mini = inimg.getMinimum();
    
  // rescale
  if (maxi == mini) {
    if (maxi <= 0.0) {
      for (i=0; i<nr; i++)
	for (j=0; j<nc; j++)
	  temp(i,j) = a;
    }
    else {
      for (i=0; i<nr; i++)
	for (j=0; j<nc; j++)
	  temp(i,j) = b;
    }
  }
  else {
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++)
	temp(i,j) = 
	  (inimg(i,j)-mini) * (b-a) / (maxi-mini) + a; 
  }
  
  return temp;
}
