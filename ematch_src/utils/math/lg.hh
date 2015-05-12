/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.7.5
 *
 *  Last Modified: $Date: 2005/12/16 06:42:45 $
 *  
 *  Description: logarithm base 2.
 *
 */
#ifndef LG_HH
#define LG_HH
#include <cmath>

namespace cbrc{

inline double lg( const double x ){
  return( 1.44269504088896340737 * log(x) );
}

}; // end namespace cbrc.
#endif // defined LG_HH

