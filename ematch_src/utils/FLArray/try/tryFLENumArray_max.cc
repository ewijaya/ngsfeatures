/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.5
 *  Last Modified: $Date: 2008/05/06 21:49:13 $
 *
 *  Purpose: try code involving FLENumArray_operator=
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLENumArray_operator_equal(){

    double v[] = { 1, 3, 6, 2, 6, 1 };
    FLENumArray<double> a( sizeof(v)/sizeof(v[0]), v );

    std::cout << "a is: " << a << std::endl;

    std::cout << "min of a is: "
	      << a.min()
	      << std::endl;

    std::cout << "first min index of a is: "
	      << a.firstMinIndex()
	      << std::endl;

    std::cout << "final min index of a is: "
	      << a.finalMinIndex()
	      << std::endl;

    std::cout << "min of range [1,4) of a is: "
	      << a.min( 1, 4 )
	      << std::endl;


    std::cout << "max of a is: "
	      << a.max()
	      << std::endl;

    std::cout << "first max index of a is: "
	      << a.firstMaxIndex()
	      << std::endl;

    std::cout << "final max index of a is: "
	      << a.finalMaxIndex()
	      << std::endl;

    std::cout << "max of first 2 elements of a is: "
	      << a.max( 0, 2 )
	      << std::endl;
    
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLENumArray_operator_equal();
  return 1;
}

