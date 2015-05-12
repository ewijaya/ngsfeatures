/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.3.29
 *  Last Modified: $Date: 2008/03/29 08:35:45 $
 *
 *  Purpose: try code involving FLEArray_constructFromString
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLEArray_constructFromString(){
    std::string s( "Freddy Krueger" );

    FLEArray<char> a( s.size(), s.data() );

    std::cout << "array is: " << a << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLEArray_constructFromString();
  return 1;
}

