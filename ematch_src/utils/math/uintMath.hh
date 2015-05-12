/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.11
 *  Last Modified: $Date: 2008/07/25 02:54:32 $
 *
 *  Purpose: Provide some simple math functions for unsigned ints
 *
 */

#ifndef _UINTMATH_HH_
#define _UINTMATH_HH_
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <limits>

namespace cbrc{

  namespace uintMath{

    // return x^y or throw exception.
    unsigned int pow( unsigned int x, unsigned int y ) throw( std::overflow_error );
  }; // end namespace uintMath

}; // end namespace cbrc
#endif // defined _UINTMATH_HH_
