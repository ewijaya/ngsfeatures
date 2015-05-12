/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2007/11/25 03:18:19 $
 *
 *  Purpose: try code involving ArgvParser::setDoc();
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "string" );

  args.setDoc( "--help|--options",
	       "options are:\n\
\n\
    -v|--verbose  print a lot of worthless crap\n\
\n\
    -s|--silent  quietly reformat your hard drive\n"
 );

  args.printDoc();
}
