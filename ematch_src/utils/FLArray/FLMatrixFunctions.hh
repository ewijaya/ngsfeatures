/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.6.18
 *  
 *  Last Modified: 2003.6.18
 *  
 *  Description: Some functions that use FLArray or FLMatrix
 *
 */
#include "FLMatrix.hh"
#ifndef _FLMATRIXFUNCTIONS_HH_
#define _FLMATRIXFUNCTIONS_HH_

namespace cbrc{

namespace FLMatrixFunctions{

  template <typename elemType>
  elemType indexedProduct( const FLNumMatrix<elemType>& m, const unsigned char* const indices ){
    elemType product = 1;
    for( unsigned int i = 0; i < m.size0(); ++i ){
      product *= m( i , indices[i] );
    }
    return product;
  }

}; // end namespace
#endif // defined _FLMATRIXFUNCTIONS_HH_

