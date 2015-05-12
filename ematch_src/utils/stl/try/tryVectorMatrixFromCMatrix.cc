/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.16
 *  Last Modified: $Date: 2006/09/16 12:01:45 $
 *
 *  Purpose: try or run code involving VectorMatrixFromCMatrix
 */
#include <iostream>
// #include "utils/argvParsing/ArgvParser.hh"
#include "../vectorMatrix.hh"

namespace cbrc{
  void tryVectorMatrixFromCMatrix(){
    double a[][4] = {
      { 0.3, 0.6, 0.1, 0.2 },
      { 0.7, 0.6, 0.1, 0.2 },
      { 0.4, 0.5, 0.1, 0.3 } };
    std::vector< std::vector<double> > b = vectorMatrixFromCMatrix( a );
    std::cout << "b is:\n" << b;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  //  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  //  args.dieIfUnusedArgs();
  cbrc::tryVectorMatrixFromCMatrix();
  return 1;
}

