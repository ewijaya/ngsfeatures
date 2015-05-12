/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.16
 *  Last Modified: $Date: 2007/06/17 23:25:21 $
 *
 *  Purpose: try out code using ArgvParser::set( unsigned int& ...
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#define ARG1_VALUE_DEFAULT 3
#define A_VALUE_DEFAULT 7


int main( int argc, const char* argv[] ){
  std::ostringstream usage;
  usage << "Usage: $0 arg1Value [--aFlag aValue]\n"
	<< "\n"
	<< "This is a program for testing parsing of unsigned int arguments.\n"
	<< "\n"
	<< "Arguments:\n"
	<< "arg1Value\n"
	<< "\n"
	<< "arg1Value defaults to " << ARG1_VALUE_DEFAULT << std::endl
	<< "Options:\n"
	<< "\n"
	<< "-a,--aFlag aValue\n"
	<< "aValue is an a-ish type of value\n"
	<< "defaults to " << A_VALUE_DEFAULT << std::endl;
  cbrc::ArgvParser args( argc, argv, usage.str() );

  unsigned int aValue = A_VALUE_DEFAULT;
  args.set( aValue, "-a", "--aFlag" );

  unsigned int arg1Value = ARG1_VALUE_DEFAULT;
  args.set( arg1Value, 1 );

  args.dieIfUnusedArgs();

  // print results
  std::cout << "arg1Value: " << arg1Value << std::endl;
  std::cout << "aValue: " << aValue << std::endl;
  return 1;
}

