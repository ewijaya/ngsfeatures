/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.9.4
 *
 *  Last Modified: 2003.9.4
 *  
 *  Purpose: Speed up computations in which the log factorial function
 *           is used repeatedly in the same range.
 *
 *  Usage: LogFactorial lf(10)
 *         std::cout << "lg( 6! ) = " << lf(6); // overloaded operator()
 *
 */

#ifndef _LOGFACTORIAL_HH_
#define _LOGFACTORIAL_HH_
#include "utils/FLArray/FLArray.hh"

namespace cbrc{

class LogFactorial{
public:
  LogFactorial( const size_t maxN, const double base=2.0 ){
    if( !maxN ) return;
    f.setSize( maxN+1 );
    f[0] = 0;  // not sure what 0! should be... 
    if( maxN < 1 ) return;
    f[1] = 0;
    for( size_t i = 2; i <= maxN; ++i ){
      f[i] = f[i-1] + log(i);
    }
    f *= ( 1 / log(base) );
  }
  size_t size() const{ return f.size(); }
  double operator()( const size_t i ) const{ return f[i]; }
  void friend testLogFactorial();
private:
  FLNumArray<double> f;
};

}; // end namespace
#endif // defined _LOGFACTORIAL_HH_

