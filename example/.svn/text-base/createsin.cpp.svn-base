/*******************************************************************
  Machine Vision
  Chapter 4, Figure 4.1
  Create sinusoid pattern to compare different spatial frequencies
 
  Modified: 07/07/06
********************************************************************/

#include "Image.h"
#include "Dip.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

#define Usage "createsin imgname u v\n"

int main(int argc, char **argv)
{
  Image img;
  int nr, nc;
  int i, j;
  float u, v;

  if (argc < 4) {
    cout << Usage;
    exit(3);
  }

  nr = nc = 128;

  img.createImage(nr, nc);
 
  u = atof(argv[2]);
  v = atof(argv[3]);
 
  for (i=0; i<nr; i++)
    for (j=0; j<nc; j++)
      img(i,j) = 150*(sin((double)u*i+v*j)+1);

  writeImage(img, argv[1]);

  return 0;
}
