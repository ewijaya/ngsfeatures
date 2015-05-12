/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2008.6.13
 *  Last Modified: $Date: 2009/06/09 09:23:14 $
 *
 *  Purpose: try code involving computeGlobalAffineIdentity
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../GlobalAffineIdentityAligner.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#define ALIGN_FLAG  -a|--align


namespace cbrc{
  void computeGlobalAffineIdentity( const bool&         doAlignment,
				    /***/ std::istream& multifastaIstream ){
    FastaSeqSlurper fr( multifastaIstream );

    const ResidueIndexMap& residueIndexMap = A_to_Z_residueIndexMap;

    fr.nextRecordOrDie();
    LabeledSequence seq0( fr.getLabeledSequence( residueIndexMap ) );

    fr.nextRecordOrDie();
    LabeledSequence seq1( fr.getLabeledSequence( residueIndexMap ) );

    
    ResidueSimilarityAffineEdit residueSimilarity( residueIndexMap );

    GlobalAffineIdentityAligner aligner(  residueSimilarity,
					  seq0.residueIndices()  );

    std::cout << "identity is: " << 100 * aligner.identity( seq1.residueIndices() )
	      << std::endl;

    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "["Q(ALIGN_FLAG)"] [multifastaFile]" );

  bool doAlignment(  args.hasFlagDelete( Q(ALIGN_FLAG) )  );
  
  std::istream&  multifastaIstream(  args.getIstream( 1 )  );

  args.dieIfUnusedArgs();
  cbrc::computeGlobalAffineIdentity( doAlignment, multifastaIstream );
  return 1;
}

