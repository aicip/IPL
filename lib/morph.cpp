/**********************************************************
 * morph.cpp - contains morphological operators
 *
 *   - dilate: dilate a binary or gray-scale image
 *   - erode: erode a binary or gray-scale image
 *   - open: erode then dilate
 *   - close: dilate then erode
 *   - bdilate: binary dilation
 *   - gdilate: gray-scale dilation
 *   - berode: binary erosion
 *   - gerode: gray-scale erosion
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 02/06/06
 *
 * Modified:
 *  - 02/15/06: allow the origin of the se to be zero
 *  - 02/15/06: add gray-scale morphology and making it
 *              transparent to the user
 **********************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

/** 
 * Dilate a binary or a gray-scale image
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @param nt The image type, default is BINARY, nt can also be GRAY
 */
Image dilate(Image &inimg, Image &se, int origRow, int origCol, int nt) {
  Image temp;
  
  if (nt == BINARY)
    temp = bdilate(inimg, se, origRow, origCol);
  else if (nt == GRAY)
    temp = gdilate(inimg, se, origRow, origCol);
  else {
    cout << "dilate: The image should be either binary or gray scale\n.";
    exit(3);
  }
  
  return temp;
}


/** 
 * Erode a binary or a gray-scale image
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @param nt The image type, default is BINARY, nt can also be GRAY
 */
Image erode(Image &inimg, Image &se, int origRow, int origCol, int nt) {
  Image temp;
  
  if (nt == BINARY)
    temp = berode(inimg, se, origRow, origCol);
  else if (nt == GRAY)
    temp = gerode(inimg, se, origRow, origCol);
  else {
    cout << "erode: The image should be either binary or gray scale\n.";
    exit(3);
  }
  
  return temp;
}


/** 
 * Open a binary or a gray-scale image
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @param nt The image type, default is BINARY, nt can also be GRAY
 */
Image open(Image &inimg, Image &se, int origRow, int origCol, int nt) {
  Image temp1, temp2;
  
  if (nt == BINARY) {
    temp1 = berode(inimg, se, origRow, origCol);
    temp2 = bdilate(temp1, se, origRow, origCol);
  }
  else if (nt == GRAY) {
    temp1 = gerode(inimg, se, origRow, origCol);
    temp2 = gdilate(temp1, se, origRow, origCol);
  }
  else {
    cout << "open: The image should be either binary or gray scale\n.";
    exit(3);
  }
  
  return temp2;
}


/** 
 * Close a binary or a gray-scale image
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @param nt The image type, default is BINARY, nt can also be GRAY
 */
Image close(Image &inimg, Image &se, int origRow, int origCol, int nt) {
  Image temp1, temp2;
  
  if (nt == BINARY) {
    temp1 = bdilate(inimg, se, origRow, origCol);
    temp2 = berode(temp1, se, origRow, origCol);
  }
  else if (nt == GRAY) {
    temp1 = gdilate(inimg, se, origRow, origCol);
    temp2 = gerode(temp1, se, origRow, origCol);
  }
  else {
    cout << "close: The image should be either binary or gray scale\n.";
    exit(3);
  }
  
  return temp2;
}


/**
 * Dilate a binary image with structuring element
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @return The dilated image
 */
Image bdilate(Image &inimg, Image &se, int origRow, int origCol) {
  Image temp;
  int nr, nc, nchan, nt, nrse, ncse, nchanse, ntse;
  int i, j, m, n;

  nrse = se.getRow();
  ncse = se.getCol();
  nchanse = se.getChannel();
  ntse = se.getType();
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();
  
  if (nchan > 1 || nchanse > 1) {
    cout << "bdilate: This function can only dilate binary images\n";
    exit(3);
  }
  if (ntse != nt) {
    cout << "bdilate: Can't dilate two images of different types\n";
    exit(3);
  }
  if (origRow >= nrse || origCol >= ncse || origRow < 0 || origCol < 0) {
    cout << "bdilate: The origin of se needs to be inside se\n";
    exit(3);
  }
  if (!(int)se(origRow,origCol)) {    // Modified on 02/15/06
    se(origRow,origCol) = L;          // allow the origin of se to be zero
  }

  temp.createImage(nr, nc, nt);
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      if ((int)inimg(i,j)) {       
        // if the foreground pixel is not zero, then fill in the pixel
        // covered by the s.e.
        for (m=0; m<nrse; m++)
          for (n=0; n<ncse; n++) {
            if ((i-origRow+m) >= 0 && (j-origCol+n) >=0 && 
                (i-origRow+m) < nr && (j-origCol+n) < nc)
              if (!(int)temp(i-origRow+m, j-origCol+n))
                temp(i-origRow+m, j-origCol+n) = ( (int)se(m,n) ? L : 0 );
          }
      }
    }

  return temp;
}


/**
 * Dilate a gray-scale image with structuring element
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @return The dilated image
 */
Image gdilate(Image &inimg, Image &se, int origRow, int origCol) {
  Image temp;
  int nr, nc, nchan, nt, nrse, ncse, nchanse, ntse;
  int i, j, m, n, l;
  float *p;

  nrse = se.getRow();
  ncse = se.getCol();
  nchanse = se.getChannel();
  ntse = se.getType();
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();
  
  if (nchan > 1 || nchanse > 1) {
    cout << "gdilate: This function can only dilate gray-scale images\n";
    exit(3);
  }
  if (ntse != nt) {
    cout << "gdilate: Can't dilate two images of different types\n";
    exit(3);
  }
  if (origRow >= nrse || origCol >= ncse || origRow < 0 || origCol < 0) {
    cout << "gdilate: The origin of se needs to be inside se\n";
    exit(3);
  }

  temp.createImage(nr, nc, nt);
  p = (float *) new float [nrse * ncse];
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      l = 0;
      for (m=0; m<nrse; m++)
        for (n=0; n<ncse; n++) {
          if ((i-origRow+m) >= 0 && (j-origCol+n) >=0 && 
              (i-origRow+m) < nr && (j-origCol+n) < nc)
            p[l++] = inimg(i-origRow+m, j-origCol+n) + se(m,n);
        }
      bubblesort(p, l);
      temp(i,j) = p[l-1];
    }

  return temp;
}


/**
 * Erode a binary image with structuring element
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @return The eroded image
 */
Image berode(Image &inimg, Image &se, int origRow, int origCol) {
  Image temp;
  int nr, nc, nchan, nt, nrse, ncse, nchanse, ntse;
  int i, j, m, n, sum, count;

  nrse = se.getRow();
  ncse = se.getCol();
  nchanse = se.getChannel();
  ntse = se.getType();
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();
  
  if (nchan > 1 || nchanse > 1) {
    cout << "erode: This function can only dilate binary images\n";
    exit(3);
  }
  if (ntse != nt) {
    cout << "erode: Can't dilate two images of different types\n";
    exit(3);
  }
  if (origRow >= nrse || origCol >= ncse || origRow < 0 || origCol < 0) {
    cout << "erode: The origin of se needs to be inside se\n";
    exit(3);
  }
  if (!(int)se(origRow,origCol)) {    // Modified on 02/15/06
    se(origRow,origCol) = L;
  }

  temp.createImage(nr, nc);
  
  // count the number of foreground pixels in the s.e.
  sum = 0;
  for (i=0; i<nrse; i++)
    for (j=0; j<ncse; j++)
      if ((int)se(i,j))
        sum++;

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      if ((int)inimg(i,j)) {     // if the foreground pixel is 1
        count = 0;
        for (m=0; m<nrse; m++)
          for (n=0; n<ncse; n++) {
            if ((i-origRow+m) >= 0 && (j-origCol+n) >=0 && 
                (i-origRow+m) < nr && (j-origCol+n) < nc)
              count += (int)inimg(i-origRow+m,j-origCol+n) && (int)se(m,n);
          }
        // if all se foreground pixels are included in the foreground of 
        // the current pixel's neighborhood, then enable this pixel in erosion
        if (sum == count)  
          temp(i,j) = L;
      }
    }

  return temp;
}


/**
 * Erode a gray-scale image with structuring element
 * @param inimg The input image
 * @param se The structuring element (s.e.)
 * @param origRow The row coordinate of the origin of the s.e., default is 0
 * @param origCol The column coordinate of the origin of the s.e., default is 0
 * @return The dilated image
 */
Image gerode(Image &inimg, Image &se, int origRow, int origCol) {
  Image temp;
  int nr, nc, nchan, nt, nrse, ncse, nchanse, ntse;
  int i, j, m, n, l;
  float *p;

  nrse = se.getRow();
  ncse = se.getCol();
  nchanse = se.getChannel();
  ntse = se.getType();
  
  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();
  
  if (nchan > 1 || nchanse > 1) {
    cout << "gerode: This function can only erode gray-scale images\n";
    exit(3);
  }
  if (ntse != nt) {
    cout << "gerode: Can't dilate two images of different types\n";
    exit(3);
  }
  if (origRow >= nrse || origCol >= ncse || origRow < 0 || origCol < 0) {
    cout << "gerode: The origin of se needs to be inside se\n";
    exit(3);
  }

  temp.createImage(nr, nc, nt);
  p = (float *) new float [nrse * ncse];
  
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++) {
      l = 0;
      for (m=0; m<nrse; m++)
        for (n=0; n<ncse; n++) {
          if ((i-origRow+m) >= 0 && (j-origCol+n) >=0 && 
              (i-origRow+m) < nr && (j-origCol+n) < nc)
            p[l++] = inimg(i-origRow+m, j-origCol+n) - se(m,n);
        }
      bubblesort(p, l);
      temp(i,j) = p[0];
    }

  return temp;
}
