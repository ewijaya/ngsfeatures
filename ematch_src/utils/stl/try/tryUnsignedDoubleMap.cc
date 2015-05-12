/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.24
 *  Last Modified: $Date: 2008/08/24 14:01:44 $
 *
 *  Purpose: try code involving UnsignedDoubleMap
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../UnsignedDoubleMap.hh"

namespace cbrc{
  void tryUnsignedDoubleMap(){
    UnsignedDoubleMap unsignedDoubleMap;

    unsignedDoubleMap( 0, 3.14 );
    unsignedDoubleMap( 2, 2.78 );

    std::cout << "map contents:\n"
	      << unsignedDoubleMap;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryUnsignedDoubleMap();
  return 1;
}

