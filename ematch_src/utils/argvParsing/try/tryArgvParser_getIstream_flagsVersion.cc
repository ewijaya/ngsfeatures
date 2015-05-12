/*  
 *  Author: Please set BPLT_AUTHOR environment variable
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Please set BPLT_AUTHOR environment variable, All rights reserved.
 *  Creation Date: 2008.1.13
 *  Last Modified: $Date: 2008/03/04 21:38:20 $
 *
 *  Purpose: try out version of ArgvParser::getIstream() which takes string flags.
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"


namespace cbrc{
  void tryArgvParser_getIstream_flagsVersion( std::istream& inputFileStream ){

    while( 1 ){
      std::string curLine = perlish::slurpLine( inputFileStream );
      if( !curLine.size() )  break;
      std::cout << curLine << std::endl;
    }

  } // end tryArgvParser_getIstream
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage: $0 -f|--file FILENAME\n\
\n\
Argument:\n\
  -f|--file  file to print to screen. If deleted or FILENAME is "Q(ARGVPARSER_STDIN_INDICATOR_STRING)", std::cin is used\n"
			 );

  std::istream& inputStream(   args.getIstream( "-f|--file" )   );
  args.dieIfUnusedArgs();
  cbrc::tryArgvParser_getIstream_flagsVersion( inputStream );
  return 1;
}

