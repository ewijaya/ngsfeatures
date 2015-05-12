/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.16
 *  Last Modified: $Date: 2007/05/19 23:07:15 $
 *
 *  Purpose: try out code using ArgvParser::set( const std::string&...
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#define ARG1_VALUE_DEFAULT "a-ish value"
#define A_VALUE_DEFAULT "a-ish value"
#define B_VALUE_DEFAULT "b-ish value"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv,
			 "Usage: $0 arg1Value [--aFlag aValue] [--bFlag bValue]\n\
\n\
Arguments:\n\
    arg1Value\n\
\n\
        arg1Value defaults to \""ARG1_VALUE_DEFAULT"\"\n\
Options:\n\
\n\
    -a,--aFlag aValue\n\
        aValue is an a-ish type of value\n\
        defaults to \""A_VALUE_DEFAULT"\"\n\
\n\
    -b,--bFlag bValue\n\
        bValue is an b-ish type of value\n\
        defaults to \""B_VALUE_DEFAULT"\"\n"
			 );

  std::string aValue = A_VALUE_DEFAULT;
  args.set( aValue, "-a", "--aFlag" );

  std::string bValue = B_VALUE_DEFAULT;
  args.set( bValue, "-b", "--bFlag" );

  std::string arg1Value = ARG1_VALUE_DEFAULT;
  args.set( arg1Value, 1 );

  args.dieIfUnusedArgs();

  // print results
  std::cout << "arg1Value: " << arg1Value << std::endl;
  std::cout << "aValue: " << aValue << std::endl;
  std::cout << "bValue: " << bValue << std::endl;
  return 1;
}

