/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.12.7
 *
 *  Last Modified: $Date: 2005/12/07 10:59:07 $
 *  
 *  Description: See header files.
 *
 *  Purpose:
 */

#include "uintMath.hh"

namespace cbrc{

namespace uintMath{
  // 0^0 defined as 1 here.
  unsigned int pow( unsigned int x, unsigned int y ) throw( std::overflow_error ){
    if( y == 0 ) return 1;
    if( x == 0 ) return 0;
    if( x == 1 ) return 1;
    if( y == 1 ) return x;
    unsigned int lim = std::numeric_limits<unsigned int>::max() / x;
    unsigned int retVal = 1;
    for( unsigned int i = 1; i < y; ++i ){
      retVal *= x;
      if( retVal > lim ){
	char errorMessage[128];
	sprintf( errorMessage, "%s: line %d  pow( %u, %u ) too big!", __FILE__, __LINE__, x, y );
#ifndef CBRC_QUIET_EXCEPTIONS	  
	std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
	throw std::overflow_error( errorMessage );
      }
    }
    return retVal*x;
  }
}; // end namespace uintMath

}; // end namespace cbrc
