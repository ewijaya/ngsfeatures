/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.7.16
 *  Last Modified: $Date: 2007/07/16 09:53:41 $
 *
 *  Purpose: try code involving reading a FLSorted2TypePairSet object
 *           from a text file.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLSorted2TypePairSet.hh"

namespace cbrc{
  void tryFLSorted2TypePairSet_readFromTextFile( std::ifstream& inputTextFile ){

    FLSorted2TypePairSet<std::string, std::string> pairList( inputTextFile );

    std::cout << "list is:\n"
	      << pairList
	      << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv,
			 "\
Usage: $0 pairSetTextFile\n\
\n\
    pairSetTextFile\n\
\n\
        pairSetTextFile should hold data in text form for constructing\n\
        FLSorted2TypePairSet<std::string, std::string> object\n\
"
			 );

  std::ifstream inputTextFile;
  args.setOrDie( inputTextFile, 1 );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLSorted2TypePairSet_readFromTextFile( inputTextFile );
  return 1;
}

