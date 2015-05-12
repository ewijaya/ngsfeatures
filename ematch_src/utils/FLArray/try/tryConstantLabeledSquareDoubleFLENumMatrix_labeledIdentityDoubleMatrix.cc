/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.3
 *  Last Modified: $Date: 2007/09/12 10:49:58 $
 *
 *  Purpose: try code involving ConstantLabeledSquareDoubleFLENumMatrix
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ConstantLabeledSquareDoubleFLENumMatrix.hh"

namespace cbrc{
  void tryConstantLabeledSquareDoubleFLENumMatrix_labeledIdentityDoubleMatrix(){

    std::cout << "Simple test of identity matrix idm\n"
	      << "idm(2,3) = " << labeledIdentityDoubleMatrix( 2, 3 ) << std::endl
	      << "idm(4,4) = " << labeledIdentityDoubleMatrix( 4, 4 ) << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no arguments" );

  args.dieIfUnusedArgs();
  cbrc::tryConstantLabeledSquareDoubleFLENumMatrix_labeledIdentityDoubleMatrix();
  return 1;
}
