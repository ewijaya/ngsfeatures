/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2007/05/13 02:21:43 $
 *
 *  Purpose: try or run code involving ArgvParser::hasFlagDelete()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "\
$0 [-p|--panda] zooname1 zooname2\n\
\n\
Tries out ArgvParser::hasFlagDelete\n\
\n\
try:\n\
\n\
$0 ueno -p beijing\n\
$0 ueno --panda beijing\n\
$0 --panda ueno beijing\n\
$0 ueno beijing -p\n\
$0 ueno beijing\n"
);

  if(   args.hasFlagDelete( "-p", "--panda" )   ){
    if( args.numArgs() != 2 ){
      std::cout << "Command line error: expected two zoo names in addition to the panda flag";
    }
    std::cout << "The zoos where you might find a Panda are:\n"
	      << args.shiftMandatory()
	      << ", "
	      << args.shiftMandatory()
	      << std::endl;
  }
  else{
    std::cout << "** no panda flag given ** ";
  }

  args.dieIfUnusedArgs();
  return 1;
}

