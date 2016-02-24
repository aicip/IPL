/********************************************************************
 * Map.h - header file of the MAP class
 *
 * Author: Hairong Qi, hqi@utk.edu, ECE, University of Tennessee
 *
 * Created: 02/12/06
 *
 * Modification:
 ********************************************************************/
#ifndef _MAP_H_
#define _MAP_H_

#define CONVERGED 1
#define DIVERGED -1

class Map {
 public:
  // constructors and destructor
  Map() { }                             // default constructor 
  ~Map() { }                            // destructor 

  // get and set functions (use inline functions)
  float getT() const { return t; }                  
  float getTf() const { return tf; }                 
  float getTi() const { return ti; }                 
  float getTd() const { return td; }                 
  float getSigma() const { return sigma; }
  float getBeta() const { return beta; }
  float getRelax() const { return relax; }
  int getNiter() const { return niter; }
  int getNann() const { return nann; }
  float getErr() const { return err; }
  int getDiv() const { return div; }

  void setT(float t) { this->t = t; }
  void setTf(float tf=0.1) { this->tf = tf; }
  void setTi(float ti=20) { this->ti = ti; }
  void setTd(float td=0.75) { this->td = td; }    
  void setSigma(float sigma=3.0) { this->sigma = sigma; }
  void setBeta(float beta=1.0) { this->beta = beta; }
  void setRelax(float relax=0.05) { this->relax = relax; }
  void setNiter(int niter=0) { this->niter = niter; }
  void setNann(int nann=0) { this->nann = nann; }
  void setErr(float err=0.05) { this->err = err; }
  void setDiv(int div=25) { this->div = div; }
      
  // increment functions
  int incrNann() { nann++; }
  int incrNiter() { niter++; }
  
 private:
  float t;                    // the current temperature 
  float tf;                   // the final temperature
  float ti;                   // the initial temperature
  float td;                   // the decreasing rate of the temperature

  float sigma;                // the std of Gaussian in the noise term
  float beta;                 // the weight of the prior term
  float relax;                // the relaxation ratio

  int niter;                  // total number of iteration
  int nann;                   // total number of annealing
  
  float err;                  // the tolerance rate of average changes
  int div;                    // the maximum contiguous divergence iterations
};

#endif //_MAP_H_
