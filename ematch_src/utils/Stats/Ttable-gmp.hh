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

#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>

#include <iostream>


namespace cbrc{

void betacf(const double& a, const double& b, const double& x, mpfr_t& f_bcf) {

  mpfr_t aa,c,d,del,h,qab,qam,qap, f_tmp;
  mpfr_init(aa);
  mpfr_init(c);
  mpfr_init(d);
  mpfr_init(del);
  mpfr_init(h);
  mpfr_init(qab);
  mpfr_init(qam);
  mpfr_init(qap);

  mpfr_init(f_tmp);

  mpfr_t MAXIT;
  mpfr_init(MAXIT);
  mpfr_set_ui(MAXIT, 100, GMP_RNDN);

  mpfr_t FPMIN;
  mpfr_init(FPMIN);
  mpfr_set_d(FPMIN, 1.0e-10000, GMP_RNDN);

  mpfr_t EPSS;
  mpfr_init(EPSS);
  mpfr_set_d(EPSS, 3.0e-16, GMP_RNDN);
//  const double EPSS = 3.0e-7;

//  qab=a+b;
//  qap=a+1.0;
//  qam=a-1.0;
//  c=1.0;
//  d=1.0-qab*x/qap;

  mpfr_set_d(qab, a + b, GMP_RNDN);
  mpfr_set_d(qap, a + 1.0, GMP_RNDN);
  mpfr_set_d(qam, a - 1.0, GMP_RNDN);
  mpfr_set_d(c, 1.0, GMP_RNDN);
  mpfr_set_d(d, 1.0 - (a+b)*x/(a+1.0), GMP_RNDN);


//  if(fabs(d) < FPMIN) d = FPMIN;
  if(mpfr_cmpabs(FPMIN, d) > 0) mpfr_set(d, FPMIN, GMP_RNDN);

/****
// for check
  std::cout << "#d = ";
  mpfr_out_str (stdout, 10, 10, d, GMP_RNDN);
  std::cout << std::endl;
***/


//  d=1.0/d;
//  h=d;
  mpfr_set_d(f_tmp, 1.0, GMP_RNDN);
  mpfr_div(d, f_tmp, d, GMP_RNDN);
  mpfr_set(h, d, GMP_RNDN);


//  int m;
//  for(m = 1; m <= MAXIT; m++) {

  mpfr_t m;
  mpfr_init(m);
  mpfr_set_d(m, 1, GMP_RNDN);
  mpfr_t m2;
  mpfr_init(m2);
  for(; mpfr_cmp(MAXIT,m) >= 0; mpfr_add_ui(m, m, 1, GMP_RNDN)) {

    mpfr_mul_ui(m2, m, 2, GMP_RNDN);

    mpfr_set_d(aa, b, GMP_RNDN);
    mpfr_sub(aa, aa, m, GMP_RNDN);
    mpfr_mul(aa, aa, m, GMP_RNDN);
    mpfr_set_d(f_tmp, x, GMP_RNDN);
    mpfr_mul(aa, aa, f_tmp, GMP_RNDN);

    mpfr_add(f_tmp, qam, m2, GMP_RNDN);
    mpfr_div(aa, aa, f_tmp, GMP_RNDN);

    mpfr_set_d(f_tmp, a, GMP_RNDN);
    mpfr_add(f_tmp, f_tmp, m2, GMP_RNDN);
    mpfr_div(aa, aa, f_tmp, GMP_RNDN);

    

    mpfr_mul(d, aa, d, GMP_RNDN);
    mpfr_add_ui(d, d, 1, GMP_RNDN);

    if(mpfr_cmpabs(FPMIN,d) > 0) mpfr_set(d, FPMIN, GMP_RNDN);

    mpfr_div(c, aa, c, GMP_RNDN);
    mpfr_add_ui(c, c, 1, GMP_RNDN);

    if(mpfr_cmpabs(FPMIN,c) > 0) mpfr_set(c, FPMIN, GMP_RNDN);

    mpfr_set_d(f_tmp, 1.0, GMP_RNDN);
    mpfr_div(d, f_tmp, d, GMP_RNDN);
    mpfr_mul(f_tmp, d, c, GMP_RNDN);
    mpfr_mul(h, h, f_tmp, GMP_RNDN);

//    aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));

    mpfr_set_d(f_tmp, a, GMP_RNDN);
    mpfr_add(f_tmp, f_tmp, m, GMP_RNDN);
    mpfr_set(aa, f_tmp, GMP_RNDN);
    mpfr_neg(aa, aa, GMP_RNDN);
    mpfr_add(f_tmp, qab, m, GMP_RNDN);
    mpfr_mul(aa, aa, f_tmp, GMP_RNDN);
    mpfr_set_d(f_tmp, x, GMP_RNDN);
    mpfr_mul(aa, aa, f_tmp, GMP_RNDN);
    mpfr_set_d(f_tmp, a, GMP_RNDN);
    mpfr_add(f_tmp, f_tmp, m2, GMP_RNDN);
    mpfr_div(aa, aa, f_tmp, GMP_RNDN);
    mpfr_set(f_tmp, qap, GMP_RNDN);
    mpfr_add(f_tmp, f_tmp, m2, GMP_RNDN);
    mpfr_div(aa, aa, f_tmp, GMP_RNDN);

//    d=1.0+aa*d;
    mpfr_mul(d, aa, d, GMP_RNDN);
    mpfr_add_ui(d, d, 1, GMP_RNDN);

//    if(fabs(d) < FPMIN) d = FPMIN;
    if(mpfr_cmpabs(FPMIN,d) > 0) mpfr_set(d, FPMIN, GMP_RNDN);

//    c=1.0+aa/c;
    mpfr_div(c, aa, c, GMP_RNDN);
    mpfr_add_ui(c, c, 1, GMP_RNDN);

//    if(fabs(c) < FPMIN) c = FPMIN;
    if(mpfr_cmpabs(FPMIN,c) > 0) mpfr_set(c, FPMIN, GMP_RNDN);

//    d=1.0/d;
//    del=d*c;
//    h *= d*c;
    mpfr_set_d(f_tmp, 1.0, GMP_RNDN);
    mpfr_div(d, f_tmp, d, GMP_RNDN);
    mpfr_mul(del, d, c, GMP_RNDN);
    mpfr_mul(h, h, del, GMP_RNDN);
//    
//    if(fabs(del-1.0) < EPSS) break;
    mpfr_sub_ui(f_tmp, del, 1, GMP_RNDN);
    if(mpfr_cmpabs(EPSS,f_tmp) > 0) break;
  }

//  if(m > MAXIT) throw std::string("a or b too big, or MAXIT too small");
  if(mpfr_cmp(m,MAXIT) > 0) throw std::string("a or b too big, or MAXIT too small");

//  return h;
  mpfr_set(f_bcf, h, GMP_RNDN);
  return;
//

}
  
  
void betai(const double& a, const double& b, const double& x) {

//double bt;
  mpfr_t f_bt;
  mpfr_init(f_bt);

  if(x < 0.0 || x > 1.0) {
    throw std::string("bad x in betai.");
  }

//const double a, b, x;

  mpfr_t f_a;
  mpfr_init(f_a);
  mpfr_set_d(f_a, a, GMP_RNDN);
  mpfr_t f_b;
  mpfr_init(f_b);
  mpfr_set_d(f_b, b, GMP_RNDN);

//  if(x == 0.0 || x == 1.0) bt = 0.0;
//  else
//    bt = exp(lgamma(a+b)-lgamma(a)-lgamma(b)+a*log(x)+b*(log(1.0-x)));
//  if(x < (a+1.0)/(a+b+2.0))
//    return bt*betacf(a,b,x)/a;
//  else
//    return 1.0-bt*betacf(b,a,1.0-x)/b;

  if(x == 0.0 || x == 1.0) {
  } else {
    mpfr_t f_ab;
    mpfr_init(f_ab);
    mpfr_set_d(f_ab, a+b, GMP_RNDN);

    mpfr_t f_ga;
    mpfr_init(f_ga);

    mpfr_t f_gb;
    mpfr_init(f_gb);

    mpfr_t f_gab;
    mpfr_init(f_gab);

    mpfr_gamma(f_gab, f_ab, GMP_RNDN);
    mpfr_log(f_gab, f_gab, GMP_RNDN);
    mpfr_gamma(f_ga, f_a, GMP_RNDN);
    mpfr_log(f_ga, f_ga, GMP_RNDN);
    mpfr_gamma(f_gb, f_b, GMP_RNDN);
    mpfr_log(f_gb, f_gb, GMP_RNDN);

    mpfr_t f_x;
    mpfr_init(f_x);
    mpfr_set_d(f_x, x, GMP_RNDN);
    mpfr_t f_1x;
    mpfr_init(f_1x);
    mpfr_set_d(f_1x, 1.0 - x, GMP_RNDN);

    mpfr_log(f_x, f_x, GMP_RNDN);
    mpfr_log(f_1x, f_1x, GMP_RNDN);

    mpfr_mul(f_x, f_a, f_x, GMP_RNDN);
    mpfr_mul(f_1x, f_b, f_1x, GMP_RNDN);

    mpfr_sub(f_bt, f_gab, f_ga, GMP_RNDN);
    mpfr_sub(f_bt, f_bt, f_gb, GMP_RNDN);
    mpfr_add(f_bt, f_bt, f_x, GMP_RNDN);
    mpfr_add(f_bt, f_bt, f_1x, GMP_RNDN);
    mpfr_exp(f_bt, f_bt, GMP_RNDN);


    //bt = exp(lgamma(a+b)-lgamma(a)-lgamma(b)+a*log(x)+b*(log(1.0-x)));
  }

  mpfr_t f_bcf;
  mpfr_init(f_bcf);

//  if(x < (a+1.0)/(a+b+2.0))
//    return bt*betacf(a,b,x)/a;
//  else
//    return 1.0-bt*betacf(b,a,1.0-x)/b;


  if(x < (a+1.0)/(a+b+2.0)) {

    betacf(a,b,x,f_bcf);
    mpfr_div(f_bcf, f_bcf, f_a, GMP_RNDN);
    mpfr_mul(f_bt, f_bt, f_bcf, GMP_RNDN);
    
  } else {

    mpfr_t f_one;
    mpfr_init(f_one);
    mpfr_set_d(f_one, 1.0, GMP_RNDN);

    betacf(b,a,1.0-x, f_bcf);
    mpfr_div(f_bcf, f_bcf, f_b, GMP_RNDN);
    mpfr_mul(f_bt, f_bt, f_bcf, GMP_RNDN);
    mpfr_sub(f_bt, f_one, f_bt, GMP_RNDN);

  }

  mpfr_out_str (stdout, 10, 10, f_bt, GMP_RNDN);
  //mpfr_log10(f_bt, f_bt, GMP_RNDN);
  //mpfr_out_str (stdout, 10, 10, f_bt, GMP_RNDN);

  mpfr_clear(f_bcf);

}

void TtoP(const double& t_val, const double& dfree) {

  if(dfree < 1 || isnan(t_val)) {
    std::cout << std::numeric_limits<double>::signaling_NaN() << std::endl;
  }
  betai(0.5 * dfree, 0.5, dfree/(dfree + t_val * t_val));

}

} // end namespace
#endif // defined _TTABLE_HH_

