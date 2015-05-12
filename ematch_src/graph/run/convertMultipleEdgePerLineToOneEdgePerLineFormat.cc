/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.12.11
 *  Last Modified: $Date: 2007/12/11 10:18:28 $
 *
 *  Description: Convert multiple edge per line format to one edge per line format
 *
 *  Purpose: Originally created to manage taboo list files
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../OneEdgePerLineFormat.hh"

namespace cbrc{
  void convertMultipleEdgePerLineToOneEdgePerLineFormat( std::istream& inStream ){
    std::string line;

    while( !inStream.eof() ){
      // get non-empty line or try again
      std::string line = perlish::slurpLine( inStream );
      if( !line.size() ) continue;

      std::vector<std::string> fields = perlish::split( OneEdgePerLineFormat::delimiter(), line );
      assert( fields.size() > 1 );
      for( size_t curFieldNo = 1; curFieldNo < fields.size(); ++curFieldNo ){
	std::cout << fields[0]
		  << OneEdgePerLineFormat::delimiter()
		  << fields[curFieldNo]
		  << std::endl;
      }
    }
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "No args, pipe in from standard in" );

  args.dieIfUnusedArgs();
  cbrc::convertMultipleEdgePerLineToOneEdgePerLineFormat( std::cin );
  return 1;
}


