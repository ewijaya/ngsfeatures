/*  
 *  Author: Please set BPLT_AUTHOR environment variable
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Please set BPLT_AUTHOR environment variable, All rights reserved.
 *  Creation Date: 2008.1.13
 *  Last Modified: $Date: 2008/01/15 04:59:39 $
 *
 *  Purpose: try code involving ArgvParser_getIstream
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"


namespace cbrc{
  void tryArgvParser_getIstream( std::istream& inputFileStream ){

    while( 1 ){
      std::string curLine = perlish::slurpLine( inputFileStream );
      if( !curLine.size() )  break;
      std::cout << curLine << std::endl;
    }

  } // end tryArgvParser_getIstream
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "$0 [file]\n\
\n\
Optional Arguments:\n\
  file  file to print to screen. If deleted, std::cin is used\n"
);

  std::istream& inputStream(   args.getIstream( 1 )   );
  args.dieIfUnusedArgs();
  cbrc::tryArgvParser_getIstream( inputStream );
  return 1;
}

