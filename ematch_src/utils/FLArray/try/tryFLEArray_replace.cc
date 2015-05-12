/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.5
 *  Last Modified: $Date: 2008/04/06 03:00:10 $
 *
 *  Purpose: try code involving FLENumArray_operator=
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLEArray_replace(){

    double av[] = { 1, 3, 6, 2, 1 };
    FLENumArray<double> a( sizeof(av)/sizeof(av[0]), av );


    std::cout << "array a is: " << a << std::endl;


    double bv[] = { 3, 4, 1 };
    FLENumArray<double> b( sizeof(bv)/sizeof(bv[0]), bv );

    std::cout << "array b is: " << b << std::endl;

    
    a.replace( 1, b );

    std::cout << "after a.replace( 1, b )\na is: " << a << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLEArray_replace();
  return 1;
}

