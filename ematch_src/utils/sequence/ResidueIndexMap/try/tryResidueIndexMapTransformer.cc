/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.11
 *  Last Modified: $Date: 2008/08/11 11:22:43 $
 *
 *  Purpose: try code involving ResidueIndexMapTransformer
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapTransformer.hh"
#include "../ResidueIndexMapPredefined.hh"

namespace cbrc{
  void tryResidueIndexMapTransformer(){
    ResidueIndexMapTransformer transcriber( DNAProperResidueIndexMap,
					    RNAProperResidueIndexMap,
					    "acgt",
					    "ugca" );

    ResidueIndexMap::arrayT DNAseq( DNAProperResidueIndexMap.toResidueIndices( "atgtcc" ) );

    // copy DNA seq, then transform in place
    ResidueIndexMap::arrayT RNAseq( DNAseq );
    transcriber.transformInPlace( RNAseq );

    std::cout << "DNAseq: " << DNAProperResidueIndexMap.toResidues( DNAseq ) << std::endl;
    std::cout << "RNAseq: " << RNAProperResidueIndexMap.toResidues( RNAseq ) << std::endl;				    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args" );
  
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMapTransformer();
  return 1;
}

