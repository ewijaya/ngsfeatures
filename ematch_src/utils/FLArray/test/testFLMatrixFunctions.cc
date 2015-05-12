/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.6.18
 *  
 *  Last Modified: 2003.7.18
 *  
 *  Purpose: test FLMatrixFunctions.
 */
#include <iostream>
#include "../FLMatrixFunctions.hh"

void cbrc::testFLMatrixFunctions(){
  unsigned int size0 = 2;
  unsigned int size1 = 4;
  FLNumMatrix<double> m( size0, size1 );
  m( 0, 0 ) = 1;
  m( 0, 1 ) = 2;
  m( 0, 2 ) = 3;
  m( 0, 3 ) = 4;
  m( 1, 0 ) = 10;
  m( 1, 1 ) = 20;
  m( 1, 2 ) = 30;
  m( 1, 3 ) = 40;

  std::cout << m;

  unsigned char indices[2] = {1, 0};

  printf( "product is %g\n", FLMatrixFunctions::indexedProduct( m, indices ) );
}

  
}

int main(){
  cbrc::testFLMatrixFunctions();
  return 1;
}

