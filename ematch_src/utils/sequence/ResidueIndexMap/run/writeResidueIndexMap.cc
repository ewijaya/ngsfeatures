/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.23
 *  Last Modified: $Date: 2007/09/23 12:17:37 $
 *
 *  Description: ResidueIndexMap binary writer
 *
 *  Input: alphabetProper and alphabet for ResidueIndexMap object
 *         output filename
 *
 *  Output: ResidueIndexMap binary filestream, written to output file
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMap.hh"

namespace cbrc{
  void writeResidueIndexMap( std::ostream& os, 
			     const std::string& alphabetProper,
			     const std::string& alphabet,
			     const bool& caseSensitive
			     ){

    ResidueIndexMap rim( alphabetProper, alphabet, caseSensitive );

    rim.write( os );
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "outputFilename alphabetProper [alphabet] [--case-sensitive]" );

  /* ******* process optional flags ******* */
  bool caseSensitive = args.hasFlag( "--case-sensitive" );


  /* ******* process positional args ******* */
  size_t curArg = 0;

  std::ofstream ofs;
  args.setCautiously( ofs, ++curArg );

  std::string alphabetProper;
  args.setOrDie( alphabetProper, ++curArg );

  std::string alphabet( alphabetProper ); // default
  args.set( alphabet, ++curArg );         // optional

  args.dieIfUnusedArgs();

  cbrc::writeResidueIndexMap( ofs, alphabetProper, alphabet, caseSensitive );
  return 1;
}

