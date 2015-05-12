/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.7
 *  Last Modified: $Date: 2008/06/27 07:38:16 $
 *
 *  Purpose: try method ResidueIndexMap::toResidueIndices()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapPredefined.hh"

namespace cbrc{
  void tryResidueIndexMap_toResidueIndices( const std::string& seq ){
    const ResidueIndexMap& rim = DNAProperResidueIndexMap;

    // This way creates the residue indexed array and then copies it
    // see assignResidueIndices for alternative.
    ResidueIndexMap::arrayT residueIndices = rim.toResidueIndices( seq );

    std::cout << "indexed sequence:\n";
    for( size_t i = 0; i < residueIndices.size(); ++i ){
      std::cout << (unsigned int) residueIndices[i];
    }
    std::cout << std::endl;
    
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "asciiDNAsequence" );
  std::string seq( args.shiftOrDie() );
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMap_toResidueIndices( seq );
  return 1;
}

