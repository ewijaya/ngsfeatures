/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.10.22
 *  Last Modified: $Date: 2007/09/16 01:07:13 $
 *
 *  Purpose: try or run code involving ToDouble
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"

namespace cbrc{
  void tryToDouble( const std::string& stringToConvert ){
    std::cout << "string entered is: " << stringToConvert << std::endl;
    double val = perlish::toDouble( stringToConvert );
    std::cout << "after conversion to double: " << val << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "stringToConvert" );
  std::string stringToConvert = args.shiftOrDie();
  args.dieIfUnusedArgs();
  cbrc::tryToDouble( stringToConvert );
  return 1;
}

