/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Creation Date: 2006.3.12
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: function to convert student t with the degree of freedom to
 *           probability
 *
 *  Usage: long double TtoP(long double t-value, long double degree_of_freedom)
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
long double lgammal(long double);
long double fabsl(long double);
}


namespace cbrc{

long double betacf(const long double& a, const long double& b, const long double& x) {

  long double aa,c,d,del,h,qab,qam,qap;
  const int MAXIT = 150;
  const long double EPS = std::numeric_limits<long double>::epsilon();
  const long double FPMIN = std::numeric_limits<long double>::min()/EPS;
  
  qab=a+b;
  qap=a+1.0L;
  qam=a-1.0L;
  c=1.0L;
  d=1.0L-qab*x/qap;
  if(fabsl(d) < FPMIN) d = FPMIN;

  d=1.0L/d;
  h=d;

  int m;
  for(m = 1; m <= MAXIT; m++) {
    
    int m2= 2 * m;
    
    aa=(long double)m*(b-(long double)m)*x/((qam+(long double)m2)*(a+(long double)m2));
    
    d=1.0L+aa*d;
    if(fabsl(d) < FPMIN) d = FPMIN;
    c=1.0L+aa/c;
    if(fabsl(c) < FPMIN) c = FPMIN;
    d=1.0L/d;
    h *= d*c;

    
    aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));

    
    d=1.0L+aa*d;
    if(fabsl(d) < FPMIN) d = FPMIN;
    c=1.0L+aa/c;
    if(fabsl(c) < FPMIN) c = FPMIN;
    d=1.0L/d;
    del=d*c;
    h *= d*c;

    if(fabsl(del-1.0L) < EPS) break;
  }
  if(m > MAXIT) {
    std::cerr << "a:" << a << " b:" << b << "m:" << m << std::endl;
    throw std::string("a or b too big, or MAXIT too small");
  }
  return h;

}
  
  
long double betai(const long double& a, const long double& b, const long double& x) {

  long double bt;
  if(x < 0.0L || x > 1.0L) {
    throw std::string("bad x in betai.");
  }
  if(x == 0.0L || x == 1.0L) bt = 0.0L;
  else
    bt = expl(lgammal(a+b)-lgammal(a)-lgammal(b)+a*logl(x)+b*(logl(1.0L-x)));


  if(x < (a+1.0L)/(a+b+2.0L))
    return bt*betacf(a,b,x)/a;
  else
    return 1.0L-bt*betacf(b,a,1.0L-x)/b;

}

long double TtoP(const long double& t_val, const long double& dfree) {

  if(dfree < 1L || isnan(t_val)) {
    return std::numeric_limits<long double>::signaling_NaN();
  }
  return betai(0.5L * dfree, 0.5L, dfree/(dfree + t_val * t_val));

}

} // end namespace
#endif // defined _TTABLE_HH_

