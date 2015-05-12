/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.16
 *  Last Modified: $Date: 2007/06/16 23:44:58 $
 *
 *  Purpose: try out code using ArgvParser::SetOrDie( std::ifstream&, ...
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"


void assertFileOkayAndPrintFirstLine( std::ifstream& ifs ){
  std::string line;
  assert( !ifs.bad() );
  std::getline( ifs, line );
  std::cout << "first line is: " << line << std::endl;
}
  

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "filename1 --filename2 filename2" );

  args.dumpArgvInfo();


  // open file from positional arg
  std::ifstream file1;
  std::string filename1 = args.setOrDie( file1, 1 );

  std::cout << "opened file: " << filename1 << std::endl;
  assertFileOkayAndPrintFirstLine( file1 );

  args.dumpArgvInfo();


  // open file from flagged arg
  std::ifstream file2;
  std::string filename2 = args.setOrDie( file2, "--filename2" );

  std::cout << "opened file: " << filename2 << std::endl;
  assertFileOkayAndPrintFirstLine( file2 );
  

  args.dieIfUnusedArgs();

  return 1;
}
