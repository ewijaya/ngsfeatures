/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2005.12.20
 *  Last Modified: $Date: 2008/06/27 07:09:12 $
 *
 *  Purpose: try code involving ConstResidueIndexSequenceBoundaries.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include "../ConstResidueIndexSequenceBoundaries.hh"

namespace cbrc{
  void tryConstResidueIndexSequenceBoundaries( const std::string& aminoSeq ){
    const ResidueIndexMap& rim = aminoResidueIndexMap;
    ResidueIndexMap::arrayT residueIndices = rim.toResidueIndices( aminoSeq );
    ConstResidueIndexSequenceBoundaries seqBoundaries2( rim, 
							&residueIndices[0], 
							residueIndices.size() );
    ConstResidueIndexSequenceBoundaries seqBoundaries( seqBoundaries2 );
    const ResidueIndexMap::indexT* startPtr = seqBoundaries.startPtr();
    for( size_t i = 0; i < seqBoundaries.size(); ++i ){
      std::cout << rim.toResidue( startPtr[i] );
    }
    std::cout << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "amino sequence" );
  std::string seq = args[1];
  args.dieIfUnusedArgs();
  cbrc::tryConstResidueIndexSequenceBoundaries( seq );
  return 1;
}

