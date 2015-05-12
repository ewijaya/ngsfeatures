/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2007/05/12 08:27:11 $
 *
 *  Purpose: try or run code involving ArgvParser::hasFlag()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"

namespace cbrc{
  void tryArgvParserHasFlag(){
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "\
$0 flags...\n\
\n\
Tests ArgvParser::hasFlag\n"
);
  std::cout << "is flag \"-p\" or \"--panda\" present?\n";
  if(   args.hasFlag( "-p", "--panda" )   ){
    std::cout << "Yes, flag present\n";
  }else{
    std::cout << "No, flag absent\n";
  }
  args.dieIfUnusedArgs();
  cbrc::tryArgvParserHasFlag();
  return 1;
}

