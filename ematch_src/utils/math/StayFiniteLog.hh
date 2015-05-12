/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.6.28
 *  Last Modified: $Date: 2006/11/23 15:01:26 $
 *  
 *  Description: log functor that returns a finite value for log(0).
 *
 *  Purpose: Avoid dealing with -inf, which is what log(0) gives.
 *
 */

#ifndef _STAYFINITELOG_HH_
#define _STAYFINITELOG_HH_
#include <limits>
#include <cmath>

namespace cbrc{

class StayFiniteLog{
public:
  StayFiniteLog(){ 
    logZero = log( std::numeric_limits<double>::min() );
  }
  double operator()( const double x ){
    assert( x > -std::numeric_limits<double>::min() );
    if( x <= std::numeric_limits<double>::min() ) return logZero;
    return log(x);
  }
  friend void testStayFiniteLog();
private:
  double logZero;
};

}; // end namespace cbrc
#endif // defined _STAYFINITELOG_HH_

