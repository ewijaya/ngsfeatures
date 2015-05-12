/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.20
 *  Last Modified: $Date: 2007/09/20 12:04:09 $
 *
 *  Purpose: try out ResidueIndexMap::read() and write()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../ResidueIndexMap.hh"

namespace cbrc{
  void tryResidueIndexMap_binaryIO( const std::string& filename,
				    const std::string& alphabetProper,
				    const std::string& alphabet,
				    const bool& caseSensitivity ){

    ResidueIndexMap rimToWrite( alphabetProper, alphabet, caseSensitivity );

    /* ***** Write rimToWrite to file ***** */

    // open file
    std::ofstream os;
    perlish::openCautiously( os, filename );

    std::cout << "Writing ResidueIndexMap object: \"" << rimToWrite << " to file\n";
    rimToWrite.write( os );

    os.close();


    /* ***** read rim from file ***** */
    std::ifstream is;
    perlish::openOrDie( is, filename );

    ResidueIndexMap rimToRead;

    std::cout << "Reading string from file\n";
    rimToRead.read( is );

    std::cout << "ResidueIndexMap object read in is: \"" << rimToRead << "\"\n";

} // end tryResidueIndexMap_binaryIO

} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "usage: $0 filename alphabetProper alphabet [--case-sensitive]\n\
\n\
A ResidueIndexMap object constructed by ResidueIndexMap( alphabetProper. alphabet, caseSensitivity )\n\
will be written to file FILENAME, then read back in and printed to std::cout in text form\n\
" );

  size_t curArg = 0;

  std::string filename;
  args.setOrDie( filename, ++curArg );

  std::string alphabetProper;
  args.setOrDie( alphabetProper, ++curArg );

  std::string alphabet;
  args.setOrDie( alphabet, ++curArg );

  bool caseSensitivity = args.hasFlag( "--case-sensitive" );

  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMap_binaryIO( filename, alphabetProper, alphabet, caseSensitivity );
  return 1;
}
