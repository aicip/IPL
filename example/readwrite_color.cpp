// Test code to show how to read and write a color image
// Testing of color model conversion

#include "Image.h"         // need to include the image library header
#include "Dip.h"
#include <iostream>
#include <cstdlib>
using namespace std;

#define Usage "./readwrite_color input-img output-img \n"

int main(int argc, char **argv)
{
  Image img1, img2, red, green, blue;
  int nr, nc, nchan, ntype;
  int i, j, k;

  // check if the command argument is right
  if (argc < 3) {
    cout << Usage;
    exit(3);
  }

  // read in the input image
  img1 = readImage(argv[1]);          // readImage() member func in the library
  nr = img1.getRow();               // obtain the nr of rows and cols
  nc = img1.getCol();
  nchan = img1.getChannel();        // obtain the nr of channels of the image
  ntype = img1.getType();

  // write it to the output image
  // need to first allocate memory to this new image
  img2.createImage(nr, nc, ntype);

  // read different channels
  red = img1.getRed();
  blue = img1.getBlue();
  green = img1.getGreen();

  // write image2 to the output image
  img2.setRed(red);
  img2.setGreen(green);
  img2.setBlue(blue);

  writeImage(img2, argv[2]);

  // test color model conversion functions
  Image img1hsi, hue, sat, intensity, img3, imgdiff;

  img1hsi = RGB2HSI(img1);           // convert from RGB model to HSI model
  hue = img1hsi.getRed();             // get the hue
  sat = img1hsi.getGreen();           // get the saturation
  intensity = img1hsi.getBlue();      // get the intensity

  writeImage(hue, "testhue.pgm");
  writeImage(sat, "testsat.pgm");
  writeImage(intensity, "testint.pgm");

  img3 = HSI2RGB(img1hsi);           // convert from HSI back to RGB

  imgdiff = img3 - img1;              // check if the function works

  writeImage(imgdiff, "testdiff.pgm");
  

  return 0;
}
