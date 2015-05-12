/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.12
 *  Last Modified: $Date: 2008/06/12 11:51:43 $
 *
 *  Purpose: try code involving Perlish_min
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"

namespace cbrc{
  void tryPerlish_min(){
    
    std::cout << "min( 5, 9, 6 ) = "    << perlish::min( 5, 9, 6 )    << std::endl;
    std::cout << "min( 3, 1, 4, 5 ) = " << perlish::min( 3, 1, 4, 5 ) << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::tryPerlish_min();
  return 1;
}

