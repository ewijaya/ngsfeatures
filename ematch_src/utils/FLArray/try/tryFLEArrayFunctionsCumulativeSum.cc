/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.18
 *  Last Modified: $Date: 2006/09/18 13:31:57 $
 *
 *  Purpose: try or run code involving FLEArrayFunctionsCumulativeSum
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArrayFunctions.hh"

namespace cbrc{
  void tryFLEArrayFunctionsCumulativeSum(){
    const double a[] = { 0.1, -3.2, 5.3, 0.5 };
    FLENumArray<double> b( 4, a );
    FLENumArray<double> cumulativeSum;
    FLEArrayFunctions::cumulativeSum( cumulativeSum, b );
    std::cout << "original array is: " << b << std::endl;
    std::cout << "cumulative sum is: " << cumulativeSum << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args needed." );
  args.dieIfUnusedArgs();
  cbrc::tryFLEArrayFunctionsCumulativeSum();
  return 1;
}

