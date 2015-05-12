/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.16
 *  Last Modified: $Date: 2007/06/17 02:38:17 $
 *
 *  Purpose: try out code using ArgvParser::SetOrDie( std::string&...
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "shiftArg posArg --flag flaggedOpt" );

  args.dumpArgvInfo();


  std::string shiftArg = args.shiftOrDie();
  std::cout << "in main, shiftArg is: " << shiftArg << std::endl;

  args.dumpArgvInfo();


  std::string flaggedOpt;
  args.setOrDie( flaggedOpt, "-f", "--flag" );
  std::cout << "in main, flaggedOpt is: " << flaggedOpt << std::endl;

  args.dumpArgvInfo();

  
  std::string posArg;
  args.setOrDie( posArg, 1 );
  std::cout << "in main, posArg is: " << posArg << std::endl;

  args.dumpArgvInfo();

  args.dieIfUnusedArgs();

  return 1;
}
