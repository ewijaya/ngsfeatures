/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.5
 *  Last Modified: $Date: 2008/04/05 23:22:25 $
 *
 *  Purpose: try code involving FLENumArray_operator=
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLENumArray_operator_assign(){

    double av[] = { 1, 3, 6, 2, 1 };
    FLENumArray<double> a( sizeof(av)/sizeof(av[0]), av );


    FLENumArray<double> b = a;
    std::cout << "array b is: " << b << std::endl;


    double cv[] = { 3, 4, 1, 3, 6 };
    FLENumArray<double> c( sizeof(cv)/sizeof(cv[0]), cv );

    std::cout << "array c is: " << c << std::endl;

    c += b;
    std::cout << " after c += b,\narray c is: "
	      << c << std::endl;

    c -= b;

    std::cout << " after subtracting b back out, c is: "
	      << c << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLENumArray_operator_assign();
  return 1;
}

