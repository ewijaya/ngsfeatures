/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.3
 *  Last Modified: $Date: 2007/05/07 23:56:21 $
 *
 *  Purpose: try or run code involving ArgvParser::logCommandLine()
 *
 *  Caveat: Not finished 2007.5.8
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"

namespace cbrc{
  void tryArgvParserLogCommandLine(){
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "logFile messageToLog" );
  std::cout << "command line is: " << args.commandLine() << std::endl;
  std::string logFile = args.shiftMandatory();
  std::string messageToLog = args.shiftMandatory();
  args.dieIfUnusedArgs();
  cbrc::tryArgvParserLogCommandLine();
  return 1;
}

