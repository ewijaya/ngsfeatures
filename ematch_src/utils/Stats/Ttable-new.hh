/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Creation Date: 2006.4.24
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

typedef long double _LD;

//long double betai(const long double& x, const long double& a, const long double& b) {
long double betai(const long double& a, const long double& b, const long double& x) {

  const int MAXK = 200;
  int k;
  long double p1, q1, p2, q2, d, previous;

  if(a <= 0L) throw std::string("bad a in betai.");
  if(b <= 0L) {
    if(x < 1L) return 0L;
    if(x == 1L) return 1L;
    throw std::string("bad x in betai.");
  }
  if(x > (a + 1L) / (a + b + 2L))
    return 1L - betai(b, a, 1L - x);
  if(x <= 0L) return 0L;
  p1 = 0L; q1 = 1L;
  p2 = expl(a * logl(x) + b * logl(1L - x) + lgammal(a + b) - lgammal(a) - lgammal(b)) / a;
  q2 = 1L;
  for(k = 0; k < MAXK; ) {
    previous = p2;
    d = - (a + (_LD)k) * (a + b + (_LD)k) * x / ((a + 2L * (_LD)k) * (a + 2L * (_LD)k + 1L));
    p1 = p1 * d + p2; q1 = q1 * d + q2;
    k++;
    d = (_LD)k * (b - (_LD)k) * x / ((a + 2L * (_LD)k - 1L) * (a + 2L * (_LD)k));
    p2 = p2 * d + p1; q2 = q2 * d + q1;
    if(q2 == 0L) {
      p2 = std::numeric_limits<long double>::max(); continue;
    }
    p1 /= q2; q1 /= q2; p2 /= q2; q2 = 1L;
    if(p2 == previous) return p2;
  }
  std::cerr << "a:" << a << " b:" << b << "x:" << x << std::endl;
  throw std::string("Not converged.");
  return p2;

}

long double TtoP(const long double& t_val, const long double& dfree) {

  if(dfree < 1L || isnan(t_val)) {
    return std::numeric_limits<long double>::signaling_NaN();
  }
  //return betai(dfree/(dfree + t_val * t_val), 0.5L * dfree, 0.5L);
  return betai(0.5L * dfree, 0.5L, dfree/(dfree + t_val * t_val));

}

} // end namespace
#endif // defined _TTABLE_HH_

