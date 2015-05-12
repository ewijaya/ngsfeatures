/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.5
 *  Last Modified: $Date: 2008/01/04 15:48:43 $
 *
 *  Purpose: try code involving ToVector
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../toVector.hh"

namespace cbrc{
  void tryToVector(){
    std::vector<std::string> svec(  toVector( "fred", "larry", "moe" )  );
    std::cout << "Array svec:" << std::endl;
    for( size_t i = 0; i < svec.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout << svec[i];
    }
    std::cout << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryToVector();
  return 1;
}

