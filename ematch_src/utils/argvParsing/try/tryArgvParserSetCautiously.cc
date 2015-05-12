/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.16
 *  Last Modified: $Date: 2007/06/17 02:37:04 $
 *
 *  Purpose: try out code using ArgvParser::setCautiously( std::ofstream& ofs,...)
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv,
			 "outputFilename1 --filename2 outputFilename2 --message messageToWriteToFile" );

  args.dumpArgvInfo();


  // open file from positional arg
  std::ofstream file1;
  std::string filename1 = args.setCautiously( file1, 1 );

  std::cout << "opened file: " << filename1 << std::endl;
  
  args.dumpArgvInfo();

  std::string messageToWriteToFile;
  args.setOrDie( messageToWriteToFile, "--message" );

  args.dumpArgvInfo();

  // write message to file1
  file1 << messageToWriteToFile << std::endl;

  // open file from flagged arg
  std::ofstream file2;
  std::string filename2 = args.setCautiously( file2, "--filename2" );

  std::cout << "opened file: " << filename2 << std::endl;
  
  // write message to file2
  file2 << messageToWriteToFile << std::endl;

  args.dieIfUnusedArgs();

  return 1;
}
