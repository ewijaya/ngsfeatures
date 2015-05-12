/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.5.25
 *
 *  Last Modified: $Date: 2006/01/05 08:13:50 $
 *  
 *  Purpose: run code involving FileSlurpLine.
 */
#include <iostream>
#include <cstdio>
#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"

namespace cbrc{
  void runFileSlurpLine(){
    FILE* testFile = fopen( "aaa", "r" );
    assert( testFile );
    for( std::string line = perlish::slurpLine( testFile );
	 line.size();
	 line = perlish::slurpLine( testFile ) ){
      std::cout << line;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  cbrc::runFileSlurpLine();
  return 1;
}

