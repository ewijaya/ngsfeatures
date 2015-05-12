/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Creation Date: 2003.6.30
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: function to convert student t with the degree of freedom to
 *           probability
 *
 *  Usage: double TtoP(double t-value, double degree_of_freedom)
 *
 *  Description: (reasonably detailed description of what this file does)
 *
 *  Reference: (Reference to relevant literature, only applicable sometimes)
 */

#ifndef _TTABLE_HH_
#define _TTABLE_HH_


#include <cmath>
#include <limits>
#include <string>

extern "C" {
double lgamma(double);
int isnan(double);
}


namespace cbrc{

double betacf(const double& a, const double& b, const double& x) {

  double aa,c,d,del,h,qab,qam,qap;
  const int MAXIT = 100;
  const double FPMIN = 1.0e-300;
  
  const double EPSS = 3.0e-16;
  
  qab=a+b;
  qap=a+1.0;
  qam=a-1.0;
  c=1.0;
  d=1.0-qab*x/qap;
  if(fabs(d) < FPMIN) d = FPMIN;

  d=1.0/d;
  h=d;


  int m;
  for(m = 1; m <= MAXIT; m++) {
    
    int m2= 2 * m;
    
    aa=(double)m*(b-(double)m)*x/((qam+(double)m2)*(a+(double)m2));
    
    d=1.0+aa*d;
    if(fabs(d) < FPMIN) d = FPMIN;
    c=1.0+aa/c;
    if(fabs(c) < FPMIN) c = FPMIN;
    d=1.0/d;
    h *= d*c;
    
    aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));

    
    d=1.0+aa*d;
    if(fabs(d) < FPMIN) d = FPMIN;
    c=1.0+aa/c;
    if(fabs(c) < FPMIN) c = FPMIN;
    d=1.0/d;
    del=d*c;
    h *= d*c;
    
    if(fabs(del-1.0) < EPSS) break;
  }
  if(m > MAXIT) throw std::string("a or b too big, or MAXIT too small");
  return h;

}
  
  
double betai(const double& a, const double& b, const double& x) {

  double bt;
  if(x < 0.0 || x > 1.0) {
    throw std::string("bad x in betai.");
  }
  if(x == 0.0 || x == 1.0) bt = 0.0;
  else
    bt = exp(lgamma(a+b)-lgamma(a)-lgamma(b)+a*log(x)+b*(log(1.0-x)));


  if(x < (a+1.0)/(a+b+2.0))
    return bt*betacf(a,b,x)/a;
  else
    return 1.0-bt*betacf(b,a,1.0-x)/b;

}

double TtoP(const double& t_val, const double& dfree) {

  if(dfree < 1 || isnan(t_val)) {
    return std::numeric_limits<double>::signaling_NaN();
  }
  return betai(0.5 * dfree, 0.5, dfree/(dfree + t_val * t_val));

}

} // end namespace
#endif // defined _TTABLE_HH_

