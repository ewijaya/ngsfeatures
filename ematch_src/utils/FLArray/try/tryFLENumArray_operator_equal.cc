/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.5
 *  Last Modified: $Date: 2008/04/06 02:04:38 $
 *
 *  Purpose: try code involving FLENumArray_operator=
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLENumArray_operator_equal(){

    double v[] = { 1, 3, 6, 2, 1 };
    FLENumArray<double> a( sizeof(v)/sizeof(v[0]), v );

    FLENumArray<double> b( a );

    std::cout << "After coping a to b\n";

    std::cout << (  (a==b) ? "a==b true" : "a==b false"  ) << std::endl;
    std::cout << (  (a!=b) ? "a!=b true" : "a!=b false"  ) << std::endl;


    a[3] = 4;

    std::cout << "\nAfter changing one element in b\n";

    std::cout << (  (a==b) ? "a==b true" : "a==b false"  ) << std::endl;
    std::cout << (  (a!=b) ? "a!=b true" : "a!=b false"  ) << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLENumArray_operator_equal();
  return 1;
}

