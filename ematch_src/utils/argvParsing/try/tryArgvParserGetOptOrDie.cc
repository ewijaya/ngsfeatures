/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2008/01/04 13:56:51 $
 *
 *  Purpose: try or run code involving ArgvParser::getOpt()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, 
			 "\
Usage: $0 --flag stringOpt\n\
\n\
MANDATORY FLAGGED ARGUMENTS\n\
\n\
    -f|--flag stringOpt\n\
" );


  std::string stringOpt = args.getOptOrDie( "-f|--flag" );

  std::cout << "stringOpt value is: " << stringOpt << std::endl;

  args.dieIfUnusedArgs();
}


