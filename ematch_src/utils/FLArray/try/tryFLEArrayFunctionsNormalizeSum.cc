/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.18
 *  Last Modified: $Date: 2006/09/18 14:28:05 $
 *
 *  Purpose: try or run code involving FLEArrayFunctions::normalizeSum()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArrayFunctions.hh"

namespace cbrc{
  void tryFLEArrayFunctionsNormalizeSum(){
    const double a[] = { 0.1, 3.2, 5.3, 0.5 };
    FLENumArray<double> b( 4, a );
    std::cout << "          original array is: " << b << std::endl;
    FLEArrayFunctions::normalizeSum( b, 100 );
    std::cout << "percent normalized array is: " << b << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args needed." );
  args.dieIfUnusedArgs();
  cbrc::tryFLEArrayFunctionsNormalizeSum();
  return 1;
}

