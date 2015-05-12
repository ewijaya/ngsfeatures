/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.14
 *  Last Modified: $Date: 2008/11/16 05:12:02 $
 *
 *  Purpose: try or run code involving StringBinaryIO
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../stringBinaryIO.hh"

namespace cbrc{
  void tryStringBinaryIO( const std::string& s,
			  const std::string& testFilename ){

    /* ***** Write s to testFile ***** */

    // open file
    std::ofstream os;
    perlish::openCautiously( os, testFilename );

    std::cout << "Writing string: \"" << s << " to outfile\n";
    stringBinaryIO::write( s, os );

    os.close();


    /* ***** read r from testFile ***** */
    std::ifstream is;
    perlish::openOrDie( is, testFilename );

    std::string r;

    std::cout << "Reading string from outfile\n";
    stringBinaryIO::read( r, is );

    std::cout << "String read in is: \"" << r << "\"\n";
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "testString testFile" );

  size_t curArg = 0;

  std::string testString;
  args.setOrDie( testString, ++curArg );

  std::string testFilename;
  args.setOrDie( testFilename, ++curArg );

  args.dieIfUnusedArgs();
  cbrc::tryStringBinaryIO( testString, testFilename );
  return 1;
}

