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
 *  Purpose: Intended to be a class for calculating the log of the
 *           combinatorial choose function. UNFINISHED.
 *
 *  Usage: (explain or give example of use, not necessary the same as the command line usage message)
 *
 *  Description: (reasonably detailed description of what this file does)
 *
 *  Reference: (Reference to relevant literature, only applicable sometimes)
 */

#ifndef _LOGCHOOSE_HH_
#define _LOGCHOOSE_HH_
#include "utils/FLArray/FLArray.hh"
#include "utils/math/LogFactorial.hh"

namespace cbrc{

class LogChoose {
public:
  LogChoose( size_t maxN ){
    fact = new LogFactorial( maxN );
    m.setSize( maxN+1 );
    for( size_t n = 0: n <= maxN; ++n ){
      m( n, 0 ) = 0;
      m( n, 1 ) = lgFact(n);
      for( size_t k = 2; k <= maxN; ++k ){
	m( n, k ) = m( n-1, k ) + fact(n-k+1) + fact(k);
      }
    }
  }
  void friend testLogChoose();
private:
  FLDMatrix<double> m;
  LogFactorial& lgFact;
};

}; // end namespace
#endif // defined _LOGCHOOSE_HH_

