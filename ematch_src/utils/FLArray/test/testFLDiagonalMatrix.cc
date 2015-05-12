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
 *  Purpose: test FLDiagonalMatrix.
 */
#include <iostream>
#include "../FLDiagonalMatrix.hh"

void testFLDiagonalMatrix(){
  cbrc::FLDiagonalMatrix<double> fldm( 5 );

  fldm.zero();
  fldm( 3, 2 ) = 3.41;

  std::cout << "fldm( 3, 2 ) " << fldm(3,2) << std::endl;

  std::cout << "fldm:\n" << fldm << std::endl;
}

int main(){
  testFLDiagonalMatrix();
  return 1;
}

