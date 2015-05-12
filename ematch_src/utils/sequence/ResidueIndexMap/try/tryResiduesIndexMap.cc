/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.12.7
 *  Last Modified: $Date: 2008/07/21 03:09:00 $
 *
 *  Purpose: try code involving ResiduesIndexMap
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResiduesIndexMap.hh"

namespace cbrc{
  void runResiduesIndexMap( const std::string& seqTypeName,
			    const std::string& seq ){

    const ResidueIndexMap&    rim(  residueIndexMapFromName( seqTypeName )  );
    const ResiduesIndexMap&  rsim(  residueIndexMapFromName( seqTypeName )  );
    std::cout << "max representable string length is: " << rsim.maxStringLen() << std::endl;

    ResidueIndexMap::arrayT testIndices = rim.toResidueIndices( seq );
    ResiduesIndexMap::indexT residuesIndex =
      rsim( (const ResidueIndexMap::indexT* const) &testIndices[0],
						   testIndices.size() );
    std::cout << "residues index for: " << seq << " = " 
	      << residuesIndex << std::endl;

    ResidueIndexMap::vectorT
      resIndexVec = rsim.toResidueIndicesVector( residuesIndex, testIndices.size() );

    std::string testSeqRecreated = rim.toResidues( resIndexVec );
    std::cout << "round trip transformation of seq gives: " << testSeqRecreated << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "seqType seq" );
  std::string seqTypeName = args[1];
  std::string seq = args[2];
  args.dieIfUnusedArgs();
  cbrc::runResiduesIndexMap( seqTypeName, seq );
  return 1;
}

