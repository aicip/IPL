// Test code to show how to read and write an image

#include "Image.h"         // need to include the image library header
#include <iostream>
#include <cstdlib>
using namespace std;

// define how to run this code
// for example, use "./readwrite lena.ppm lenacopy.ppm"
#define Usage "./readwrite input-img output-img \n"

int main(int argc, char **argv)
{
  Image img1, img2;
  int nr, nc, ntype, nchan;
  int i, j, k;

  // check if the command argument is right
  if (argc < 3) {
    cout << Usage;
    exit(3);
  }

  // read in the input image
  img1 = readImage(argv[1]);      // readImage is a member func in the Image lib
  nr = img1.getRow();           // obtain the nr of rows and col
  nc = img1.getCol();
  ntype = img1.getType();       // obtain the type of the image
  nchan = img1.getChannel();    // obtain the nr of channels of the image

  // write it to the output image
  // need to first allocate memory to this new image
  img2.createImage(nr, nc, ntype);
  
  // perform image processing on the input image and save it to the output img
  // the following is the simplest as it simply copies image 1 to image 2
  for (i=0; i<nr; i++) {
    for (j=0; j<nc; j++) {
      for (k=0; k<nchan; k++)
        img2(i, j, k) = img1(i, j, k);
    }
  }

  // write image2 to the output image
  writeImage(img2, argv[2]);

  return 0;
}
