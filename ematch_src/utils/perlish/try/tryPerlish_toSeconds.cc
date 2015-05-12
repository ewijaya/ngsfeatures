/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.6.27
 *  Last Modified: $Date: 2007/06/27 10:35:49 $
 *
 *  Purpose: try or run code involving Perlish::toSeconds()
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "timeString" );

  std::string timeString;
  args.setOrDie( timeString, 1 );

  args.dieIfUnusedArgs();

  
  std::cout << "time in seconds is: " 
	    << cbrc::perlish::toSeconds( timeString ) << std::endl;

  return 1;
}

