/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.5
 *  Last Modified: $Date: 2008/04/05 23:19:39 $
 *
 *  Purpose: time FLENumArray operator+=
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void timeFLENumArray_operator_assign(){

    const size_t arraySize = 1000000;

    const size_t numIter = 1000;

    FLENumArray<double> a( arraySize, 3.14 );

    FLENumArray<double> b( arraySize );

    for( size_t i = 0; i < numIter; ++i ){
      b += a;
    }

    std::cout << "first element of b is: " << b[0] << std::endl;
    
    std::cout << "final element of b is: " << b[b.size()-1] << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::timeFLENumArray_operator_assign();
  return 1;
}

