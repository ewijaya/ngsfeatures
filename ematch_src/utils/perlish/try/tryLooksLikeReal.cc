/*
 *  Author: "Paul B. Horton"
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.12.29
 *
 *  Last Modified: 2004.12.29
 *  
 *  Purpose: run code involving LooksLikeReal.
 */
#include <iostream>
//#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"

namespace cbrc{
  void runLooksLikeReal(){
    std::string q;
    std::cout << "type in a string\n";
    std::cin >> q;
    std::cout << "string: " << q << " is " 
	      << ( perlish::looksLikeReal( q ) ? " a real\n" : " not a real\n" );
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::runLooksLikeReal();
  return 1;
}

