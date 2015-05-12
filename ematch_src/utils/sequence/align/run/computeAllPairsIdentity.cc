/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.13
 *  Last Modified: $Date: 2009/08/05 12:57:39 $
 *
 *  Description: Compute (adjusted) identity of all pairs of sequences above
 *               a given threshold
 *
 *  Purpose: Created for updating WoLF PSORT dataset
 */
#include <iostream>
#include "boost/foreach.hpp"
#include "utils/argvParsing/ArgvParser.hh"
#include "../GlobalAffineIdentityAligner.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#define IDENTITY_FLAGS    -i|--identity
#define ADJUSTMENT_FLAGS  -a|--adjustment-factor


struct argsT{
  double         identityThreshold;
  double         lengthAdjustmentFactor;
  std::istream*  fastaIstreamPtr;
} args;


namespace cbrc{
  void computeAllPairsIdentity(){

    const ResidueIndexMap& residueIndexMap = A_to_Z_residueIndexMap;
    
    /* ** declare and read in sequences ** */
    FastaSeqSlurper seqSlurper;
   
    std::vector<LabeledSequence> seqs;
    seqSlurper.slurpLabeledSequences( seqs, *args.fastaIstreamPtr );

    for( size_t i = 0; i < seqs.size(); ++i ){
      
      GlobalAffineIdentityAligner aligner( residueIndexMap,
					   seqs[i].residueIndices() );

      for( size_t j = i+1; j < seqs.size(); ++j ){
	std::cout << seqs[i].name() << "\t" << seqs[j].name()
		  << "\t" << 100 * aligner.identity( seqs[j].residueIndices() )
		  << std::endl;
      }
    }
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "[-a lengthAdjustmentFactor] -i identityThreshold [fastaFile] " );

  argP.setOrDie( args.identityThreshold, Q(IDENTITY_FLAGS)   );

  argP.set( args.lengthAdjustmentFactor, Q(ADJUSTMENT_FLAGS) );

  args.fastaIstreamPtr = argP.getIstreamPtr( 1 );
  
  argP.dieIfUnusedArgs();
  cbrc::computeAllPairsIdentity();
  return 1;
}

