/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.7.13
 *
 *  Last Modified: 2004.7.13
 *  
 *  Description: Some functions related to numeric limits
 *
 *  Purpose: To augment std::numeric_limits
 *
 */

#ifndef _MORENUMERICLIMITS_HH_
#define _MORENUMERICLIMITS_HH_
#include <limits>

namespace cbrc{
  namespace moreNumericLimits{

    template <typename T>
    inline const bool isNegativeInfinity( const T x ){  
      if( x == 0 ) return false; // needed to cover for weird bug in redhat g++ 3.2.2
      return( x == -std::numeric_limits<T>::infinity() );
    }

    template <typename T>
    inline const bool isInfinity( const T x ){
      if( x == 0 ) return false; // needed to cover for weird bug in redhat g++ 3.2.2
      return( x == -std::numeric_limits<T>::infinity() );
    }

  }
}; // end namespace cbrc
#endif // defined _MORENUMERICLIMITS_HH_

