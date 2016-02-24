/****************************************************************
 * mapmfa.cpp - use mean field annealing (mfa) to solve
 *   image restoration problems formulated by 
 *   maximum a-posteriori probability (map)
 *
 *   - mapmfa: mean field annealing for MAP
 *   - linear: linear piece-wise linear surface model 
 * 
 * Author: Hairong Qi (C) hqi@utk.edu
 * 
 * Created: 02/06/06
 *
 * Modified:
 *  - 02/15/06: bug in the calculation of dHP, by Tom Karnowski
 ****************************************************************/

#include "Image.h"
#include "Dip.h"
#include "MAP.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
 * Use mean field annealing to solve MAP optimization
 * @param gimg The measure image (corrupted by noise and blur)
 * @param par The parameters
 * @return The restored image.
 */
Image mapmfa(Image &gimg, Map &par) {
  Image fimg; 
  int done;

  // in order to calculate the psnr
  Image pimg = readImage("checkerboard_perfect.pgm");
  
  // set initial f equal to the input measurement image 
  fimg = gimg;    

  done = 0;                // flag to see if annealing is finished or not
  par.setT(par.getTi());   // set the starting temperature to ti
  par.setNann(0);          // set the annealing number to be 0
  
  // exponential annealing schedule 
  while (!done) {
  	par.incrNann();         // increase number of annealing by 1
       
    // for each temperature, start gradient descent
    // this specific one uses piece-wise linear as the surface model (the prior)
    if (linear (fimg, gimg, par) != CONVERGED) {
      cout << "mapmfa: Diverged or failed while performing gradient descent\n";
      break;
    }

    cout << "PSNR = " << psnr(pimg, fimg) << endl;
    
    par.setT(par.getT() * par.getTd());     // gradually reduce the temperature
    if (par.getT() < par.getTf() || par.getT() <= 0)
      done = 1;
  }
  
  return fimg;
}


/**
 * Use gradient descent to restore the corrupted image
 * assuming a piecewise linear surface prior model
 * @param fimg The restored image
 * @param gimg The measured image
 * @param par The parameters
 * @return Converge or not.
 */
int linear (Image &fimg, Image &gimg, Map &par) {
  static int first = 1;
  static int nr, nc, nt, npix;
  static float stepsize, prevsmod, prevH;
  static int diverges;
  static Image qxx(1,3), qyy(3,1), qxy(3,3), h;
  static Image rxx, ryy, rxy, sxx, syy, sxy, dxx, dyy, dxy, fs, fss, dHP, dHN;
  int done = 0;
  float t2, t3, alpha, s2, tmp, new_stepsize;
  float HN, HP, H, smod, mod, sum;
  int i, j;
  char filename[15];
      
  // if first time running this function, get the dimensions
  if (first) {
    // get the dimension
  	nr = gimg.getRow();
  	nc = gimg.getCol();
  	nt = gimg.getType();
  	npix = nr * nc;
  	
  	// fill in the quadratic kernel for the prior term
  	qxx(0,0) = qxx(0,2) = 1.0/sqrt(6.0); 
    qxx(0,1) = -2.0/sqrt(6.0);
    qyy = transpose(qxx);
    qxy(0,0) = qxy(2,2) = -0.5;
    qxy(0,2) = qxy(2,0) = 0.5;
    
    // fill in the Gaussian kernel for the noise term
    h = gaussianKernel(par.getSigma());

    // set stepsize
    stepsize = 0.001;
  	
  	diverges = 0;
    prevH = -10000;
    prevsmod = 10000;
  	
  	// allocate space for intermediate images
  	sxx.createImage(nr, nc, nt);
  	syy.createImage(nr, nc, nt);
  	sxy.createImage(nr, nc, nt);
  	rxx.createImage(nr, nc, nt);
  	ryy.createImage(nr, nc, nt);
  	rxy.createImage(nr, nc, nt);
  	dxx.createImage(nr, nc, nt);
  	dyy.createImage(nr, nc, nt);
  	dxy.createImage(nr, nc, nt);
    dHN.createImage(nr, nc, nt);
    dHP.createImage(nr, nc, nt);
    fs.createImage(nr, nc, nt);
    fss.createImage(nr, nc, nt);
  	
  	// set the first time usage to "false"
    first = 0;
  }

  // precalculate some parameters to save computation in each iteration
  t2 = 2 * par.getT() * par.getT();
  t3 = par.getT() * par.getT() * par.getT();
  alpha = par.getBeta() / t3;
  s2 = par.getSigma() * par.getSigma();

  par.setNiter(0);
  
  while (!done) {
    par.incrNiter();

    smod = 0.0;                    // average modifications
    sum = 0;                       // average intensity value of fimg
    HP = HN = H = 0.0;

    // compute the prior term and the noise term
    // following the notation on textbook (Snyder&Qi) pp.125-126

    // Step 1: calculate H = HN + HP
    rxx = conv(fimg, qxx);          // f convolves with quadratic kernels 
    ryy = conv(fimg, qyy);          // for the prior term
    rxy = conv(fimg, qxy);
    
    fs = conv(fimg, h);             // f convolves with gaussian kernel (h)
                                    // for the noise term
    
    for (i=0; i<nr; i++) {                     
      for (j=0; j<nc; j++) {
        tmp = exp(-rxx(i,j)*rxx(i,j)/t2);   // for the prior term
        HP += tmp;
        sxx(i,j) = rxx(i,j) * tmp;
        tmp = exp(-ryy(i,j)*ryy(i,j)/t2);
        HP += tmp;
        syy(i,j) = ryy(i,j) * tmp;
        tmp = exp(-rxy(i,j)*rxy(i,j)/t2);   // Modified on 02/15/06
        HP += tmp;
        sxy(i,j) = rxy(i,j) * tmp;
        
        fs(i,j) = fs(i,j) - gimg(i,j);       // for the noise term
        HN += fs(i,j) * fs(i,j); 
      }
    }
    HP /= par.getT();
    HN /= 2*s2;
    H = HN - par.getBeta() * HP;
            
    // Step 2: calculate dHN and dHP
    dxx = conv(sxx, qxx);                   // for the prior term
    dyy = conv(syy, qyy);
    dxy = conv(sxy, qxy);
    
    fss = conv(fs, h);                      // noise term (g-fxh)xhrev
    
    for (i=0; i<nr; i++)
      for (j=0; j<nc; j++) {
        dHP(i,j) = alpha * (dxx(i,j)+dyy(i,j)+dxy(i,j));
        dHN(i,j) = fss(i,j) / s2;
      }
        	
    // Step 3: gradient descent 
    for (i=0; i<nr; i++) {
      for (j=0; j<nc; j++) {
      	sum += fimg(i,j);
      	mod = stepsize * (dHN(i,j) + dHP(i,j));
      	fimg(i,j) -= mod;
        
        // handle out of bound values
        if (fimg(i,j) < 0.0) {
          mod = fimg(i,j);
          fimg(i,j) = 0.0;
        } 
        else if (fimg(i,j) > L) {
          mod = L - fimg(i,j);
          fimg(i,j) = L;
        }
        
        smod += fabs(mod);                
      }  
    }
        
    sum /= npix;
    smod /= npix;

    // Step 4: justify the stepsize
    if (prevH > H)                 // downhill 
      new_stepsize = sum*par.getRelax()*sqrt(par.getT()/par.getTf()) / smod;
    else                           // uphill 
      new_stepsize = sum*par.getRelax()*sqrt(par.getT()/par.getTf()) / smod/2.0;
    stepsize = sqrt(new_stepsize * stepsize);
    if (stepsize > 20)
      stepsize = 20;
    prevH = H;

    // Step 5: check if diverge or converge
    if (smod < par.getErr())
      done = CONVERGED;
    if (smod <= prevsmod)
      diverges = 0;
    else {
      if (++diverges >= par.getDiv()) {
        cout << "mapmfa: "
             << "Diverged after " << par.getNann() << " annealing, "
             << par.getNiter() << " iteration, "
             << "at temperature " << par.getT() << endl;
        return (DIVERGED);
      }
    }
    prevsmod = smod;

    cout << "ann=" << par.getNann()
         << ", iter=" << par.getNiter()
         << ", temperature=" << par.getT() 
         << ", smod=" << smod
         << ", stepsize=" << stepsize 
         << ", diverges=" << diverges << endl;

    if (par.getNiter() > 10)
      done = 1;

    if (par.getNiter() % 5 == 0) {
      sprintf(filename, "test.%d.%d.pgm", par.getNann(), par.getNiter());
      writeImage(fimg, filename);
    }    
  } 
  
  return CONVERGED;
}


