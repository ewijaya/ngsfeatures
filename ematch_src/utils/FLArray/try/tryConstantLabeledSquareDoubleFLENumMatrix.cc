/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.3
 *  Last Modified: $Date: 2007/09/12 10:45:03 $
 *
 *  Purpose: try code involving ConstantLabeledSquareDoubleFLENumMatrix
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ConstantLabeledSquareDoubleFLENumMatrix.hh"

namespace cbrc{
  void tryConstantLabeledSquareDoubleFLENumMatrix( std::istream& matrixInputStream ){
    ConstantLabeledSquareDoubleFLENumMatrix mat( matrixInputStream );

    std::cout << "matrix is:\n" << mat;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "matrixInputFile" );

  std::ifstream matrixInputFile;
  args.setOrDie( matrixInputFile, 1 );
  args.dieIfUnusedArgs();
  cbrc::tryConstantLabeledSquareDoubleFLENumMatrix( matrixInputFile );
  return 1;
}
