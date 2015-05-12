/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2007/11/25 02:50:21 $
 *
 *  Purpose: try code involving ArgvParser::splitOnVerticalBar()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#warning Note: you must make ArgvParser::splitOnVerticalBar() temporarily public to compile.


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "string" );

  std::vector<std::string> fields = args.splitOnVerticalBar( args.shiftOrDie() );

  std::cout << "Array fields:" << std::endl;
  for( size_t i = 0; i < fields.size(); ++i ){
    if( i ) std::cout << ", ";
    std::cout << fields[i];
  }
  std::cout << std::endl;
  
}


