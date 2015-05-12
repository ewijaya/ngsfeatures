/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.3.29
 *  Last Modified: $Date: 2008/03/29 06:02:38 $
 *
 *  Purpose: try code involving FLEArray_fill
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLEArray_fill(){

    FLEArray<double> a( 10, 3.14 );

    std::cout << "filled with pi\n";
    std::cout << "array: " << a << std::endl;


    a.fill( 2.78, 3, 6 );

    std::cout << "\nfilled (3,5) with e\n";
    std::cout << "array: " << a << std::endl;


    a.fill( 7, 2 );

    std::cout << "\nfilled (2- with 7\n";
    std::cout << "array: " << a << std::endl;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLEArray_fill();
  return 1;
}

