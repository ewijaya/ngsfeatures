/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.9
 *  Last Modified: $Date: 2007/05/09 02:50:39 $
 *
 *  Purpose: try or run code involving ArgvParser functions
 *           for directly accessing the argument vector.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"

namespace cbrc{
  void tryArgvParserArgvAccess(){
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "any command line okay, try out difference versions!" );

  std::cout << "The values of argc(), size(), and numOpts() are: "
	    << args.argc() << ", "
	    << args.size() << ", "
	    << args.numOpts() << std::endl;

  std::cout << "Argument vector as accessed by \"[]\":" << std::endl;
  for( size_t i = 0; i < args.size(); ++i ){
    if( i ) std::cout << ", ";
    std::cout << args[i];
  }
  std::cout << std::endl;
  
  args.dieIfUnusedArgs();
  cbrc::tryArgvParserArgvAccess();
  return 1;
}

