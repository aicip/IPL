/*******************************************************************
  Machine Vision
  Chapter 4, Figure 4.1
  Create Mach band to compare different spatial frequencies
 
  Modified: 07/07/06
********************************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
using namespace std;

#define Usage "machband filename row col width\n"

int main(int argc, char **argv)
{
  Image img;
  int nr, nc;
  int i, j, k;
  int width;

  if (argc < 5) {
    cout << Usage;
    exit(3);
  }

  nr = atoi(argv[2]);
  nc = atoi(argv[3]);
  width = atoi(argv[4]);
  
  img.createImage(nr, nc);

  for (i=0; i<nr; i++) {
    for (j=0; j<width; j++) {
      img(i, j) = 50;
    }

    k = 0;
    for (j=width; j<2*width; j++) {
      img(i, j) = 50 + k;
      k += 1;
    }

    for (j=width*2; j<nc; j++) {
      img(i, j) = 50 + k - 1;
    }
  }

  writeImage(img, argv[1]);

  return 0;
}
