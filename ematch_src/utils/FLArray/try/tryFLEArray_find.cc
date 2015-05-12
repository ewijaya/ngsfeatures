/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.3.29
 *  Last Modified: $Date: 2008/07/31 01:58:28 $
 *
 *  Purpose: try code involving FLEArray::find
 */
#include <iostream>
#include "../FLEArray.hh"


namespace cbrc{
  void tryFLEArray_find(){

    double v[] = { 2.34, 2.59, 2.34, 9.29, 10.9 };

    FLENumArrayFromCArray( double, a, v );

    
    std::cout << "array: " << a << std::endl;

    std::cout << "first index of 2.34: " << a.find( 2.34 )
	      << std::endl;

    std::cout << "first index of 2.34 after index 1: " << a.find( 2.34, 1 )
	      << std::endl;

    std::cout << "first index of 2.34 in [2,3) index 1: " << a.find( 2.34, 2, 3 )
	      << std::endl;

    size_t piIndex = a.find( 3.14 );
    std::cout << "the values 3.14 is"
	      << (  a.indexInRange( piIndex )
		    ? "" : " not "  )
	      << "present in the array\n";

    std::cout << "final index of 2.34: " << a.rfind( 2.34 )
	      << std::endl;

    std::cout << "final index of 2.34: " << a.rfind( 2.34, 0, a.size() )
	      << std::endl;

    std::cout << "final index of 2.34 in range [1,-2) " << a.rfind( 2.34, 0, a.size()-2 )
	      << std::endl;
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::tryFLEArray_find();
  return 1;
}

