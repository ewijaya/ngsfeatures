/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005,2006, Paul B. Horton, All rights reserved.
 *  Creation Date: : 2005.12.21
 *  Last Modified: $Date: 2008/06/27 08:57:14 $
 *
 *  Purpose: try or run code involving ResidueIndexWindow.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include "../ResidueIndexWindowFactory.hh"

namespace cbrc{
  void tryResidueIndexWindow( const std::string& seq ){
    const ResidueIndexMap rim = DNAProperResidueIndexMap;
    ResidueIndexMap::arrayT resIndices = rim.toResidueIndices( seq );
    ResidueIndexWindowFactory riwf( rim, &resIndices[0], resIndices.size() );
    size_t winSize = (4 < seq.size()) ? 4 : seq.size();
    ResidueIndexWindow riw = riwf.getResidueIndexWindow( 0, winSize );
    std::cout << riw << std::endl;
    while( riw.increment() ){
      std::cout << riw << std::endl;
    }
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "DNA_sequence" );
  std::string seq = args[1];
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexWindow( seq );
  return 1;
}
