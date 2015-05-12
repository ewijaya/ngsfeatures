/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2008/02/24 03:14:43 $
 *
 *  Purpose: try code involving SymmetricBitMatrix
 */
#include <iostream>
#include "../SymmetricBitMatrix.hh"


namespace cbrc{
  void trySymmetricBitMatrix(){
    SymmetricBitMatrix sbm( 79 );

    for( size_t i = 0; i < sbm.width(); ++i ){
      for( size_t j = 0; j < i; ++j ){
	sbm.set( j, i, (i+j) % 2 );  // purposely switch i, j for test.
      }
    }

    std::cout << "matrix is:\n" << sbm;

    std::cout << "size in bytes: " << sbm.sizeInBytes() << std::endl;
  }
} // end namespace cbrc



int main(){
  cbrc::trySymmetricBitMatrix();
  return 1;
}
