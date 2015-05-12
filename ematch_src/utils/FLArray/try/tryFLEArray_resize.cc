/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.3.29
 *  Last Modified: $Date: 2008/03/29 10:18:21 $
 *
 *  Purpose: try code involving FLEArray::resize()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLEArray_resize(){
    
    int v[] = { 0, 1, 2, 3, 4, 5 };
    FLEArray<int> a( sizeof(v)/sizeof(v[0]), v );

    std::cout << "array is: " << a << std::endl;

    // truncate
    a.resize( 3 );

    std::cout << "after resizing to 3: " << a << std::endl;

    
    // expand
    a.resize( 9 );
    a[7] = 88;
    
    std::cout << "after resizing to 9: " << a << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLEArray_resize();
  return 1;
}

