/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, 2007. Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.24
 *  Last Modified: $Date: 2008/06/27 12:32:11 $
 *
 *  Purpose: try or run code involving ResidueIndexMap_splitIntoProperOnlySubstrings
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapPredefined.hh"

namespace cbrc{
  void tryResidueIndexMap_splitIntoProperOnlySubstrings( const std::string& seq ){

    const ResidueIndexMap& rimExtended = DNAExtendedResidueIndexMap;

    ResidueIndexMap::vectorT seqResiduesVec = rimExtended.toResidueIndicesVector( seq );
    std::cout << "seq: " << seq << std::endl;
    std::vector<ResidueIndexMap::vectorT> splitSeqs 
      = rimExtended.splitIntoProperOnlySubstrings( seqResiduesVec );

    std::cout << "Array splitSeqs:" << std::endl;
    for( size_t i = 0; i < splitSeqs.size(); ++i ){
      std::string residues(   rimExtended.toResidues( splitSeqs[i] )   );
      std::cout << residues << std::endl;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "test_sequence" );
  std::string seq( "atggntaccnnagtgca" );
  args.set( seq, 1 );
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMap_splitIntoProperOnlySubstrings( seq );
  return 1;
}

