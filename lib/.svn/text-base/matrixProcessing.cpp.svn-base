/**********************************************************
 * matrixProcessing.cpp - matrix manipulation 
 *
 *   - transpose: matrix transpose
 *   - inverse: matrix inverse
 *   - subImage: crop an image
 *   - pinv: matrix pseudo inverse
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 01/11/08
 *
 * Modified:
 *   - 07/30/09: add pinv() implementation
 **********************************************************/
#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// functions used by matrix inverse calculation
int findPivot(Image &, int);
void switchRow(Image &, int, int);
void dividePivot(Image &, int);
void eliminate(Image &, int);

/**
 * Calculate transpose of a matrix
 * @param inimg The input matrix.
 * @return The transpose of the input matrix.
 */
Image transpose(Image &inimg) {
  int i, j;
  int nr, nc, nchan;  
  Image temp;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "TRANSPOSE: Can only handle single-channel image.\n";
    exit(3);
  }
  temp.createImage(nc, nr);

  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      temp(j,i) = inimg(i,j);

  return temp;
}


/**
 * Calculate the inverse of a matrix using Gauss-Jordan elimination
 * @param inimg The input matrix.
 * @return The inverse of the input matrix.
 */
Image inverse(Image &inimg) {
  int i, j, m, k, pivot;
  int nr, nc, nchan;
  Image temp, img;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  if (nchan > 1) {
    cout << "INVERSE: Can only handle single-channel image.\n";
    exit(3);
  }
  if (nr != nc) {
    cout << "INVERSE: Cannot compute the inverse of a non-square matrix\n";
    exit(3);
  }

  temp.createImage(nr, 2*nc);

  // construct the A | I matrix
  k = 0;
  for (i=0; i<nr; i++) {
    for (j=0; j<nc; j++)
      temp(i,j) = inimg(i,j);
    temp(i,nc+k++) = 1;
  }

  pivot = 0;  
  while (pivot < nr) {
    m = findPivot(temp, pivot);    // partial pivoting

    if (m != pivot) {
      switchRow(temp, pivot, m);
    }
    else if (temp(pivot,pivot) == 0) {
      cout << "inverse: This is a singular matrix, noninvertible\n";
      exit(3);
    }

    if (temp(pivot, pivot) != 1.0) {
      dividePivot(temp, pivot);
    }
      
    eliminate(temp, pivot);

    pivot++;
  }

  img = subImage(temp, 0, nc, nr-1, 2*nc-1);

  return img;
}

// find the maximum absolute value in the pivotal column
int findPivot(Image &img, int pivot) {
  float maxi;
  int i, index;
  int nr;

  nr = img.getRow();
  maxi = fabs(img(pivot,pivot));
  index = pivot;

  for (i=pivot+1; i<nr; i++)
    if (fabs(img(i,pivot)) > maxi) {     // Chris reported problem with abs
      index = i;
      maxi = fabs(img(i,pivot));
    }

  return index;
}

// switch two rows pivot <--> m
void switchRow(Image &img, int pivot, int m) {
  int i, j;
  float tmp;
  int nc;

  nc = img.getCol();
  for (j=pivot; j<nc; j++) {
    tmp = img(pivot,j);
    img(pivot,j) = img(m,j);
    img(m,j) = tmp;
  }
}

// divide the pivotal row by pivot
void dividePivot(Image &img, int pivot) {
  float scale;
  int j;
  int nc;

  nc = img.getCol();
  scale = img(pivot,pivot);

  for (j=pivot; j<nc; j++) 
    img(pivot,j) /= scale;
}

// eliminate elements on the pivotal column
void eliminate(Image &img, int pivot) {
  int i, j;
  int nr, nc;
  float scale;

  nr = img.getRow();
  nc = img.getCol();
  
  for (i=pivot+1; i<nr; i++) {
    scale = img(i,pivot);
    for (j=pivot; j<nc; j++) 
      img(i,j) -= img(pivot,j) * scale;
  }

  for (i=pivot-1; i>=0; i--) {
    scale = img(i,pivot);
    for (j=pivot; j<nc; j++) 
      img(i,j) -= img(pivot,j) * scale;
  }
}
 

/**
 * Crop an image.
 * @param inimg The input matrix.
 * @param srow The row index of the top-left corner, starts from 0.
 * @param scol The column index of the top-left corner.
 * @param erow The row index of the lower-right corner.
 * @param ecol The column index of the lower-right corner.
 * @return The cropped image.
 */
Image subImage(Image &inimg, int srow, int scol, int erow, int ecol) {
  int i, j, k;
  int nr, nc, nchan, nt;
  Image temp;

  nr = inimg.getRow();
  nc = inimg.getCol();
  nchan = inimg.getChannel();
  nt = inimg.getType();  
  if (srow > erow || scol > ecol || erow > nr -1 || ecol > nc - 1) {
    cout << "subImage: Check the cropping index.\n";
    exit(3);
  }
  temp.createImage(erow-srow+1, ecol-scol+1, nt);

  for (i=srow; i<=erow; i++)
    for (j=scol; j<=ecol; j++)
      for (k=0; k<nchan; k++)
        temp(i-srow,j-scol,k) = inimg(i,j,k);

  return temp;
}


/**
 * Calculate the pseudo inverse of a matrix 
 * @param inimg The input matrix.
 * @return The pseudo-inverse of the input matrix.
 */
Image pinv(Image &inimg) {
  Image tinimg, temp, invtemp, outimg;

  tinimg = transpose(inimg);
  temp = tinimg ->* inimg;
  invtemp = inverse(temp);
  outimg = invtemp ->* tinimg;

  return outimg;
}
