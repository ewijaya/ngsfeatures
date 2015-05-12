/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.13
 *  Last Modified: $Date: 2009/07/29 23:44:42 $
 *
 *  Description: Compute (adjusted) identity of all pairs of sequences above
 *               a given threshold
 *
 *  Purpose: Created for updating WoLF PSORT dataset
 */
#include <iostream>
#include "boost/foreach.hpp"
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "utils/graph/ConnectedComponentOnlineComputer.hh"
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "../SeqGlobalAffineAligner.hh"
#include "../AminoScoreIdentity.hh"
#define IDENTITY_FLAGS    -i|--identity
#define ADJUSTMENT_FLAGS  -a|--adjustment-factor


static struct argsT{
  double         identityThreshold;
  double         lengthAdjustmentFactor;
  std::istream*  fastaIstreamPtr;
} args;



namespace cbrc{


void clusterByIdentity(){
    
  /* ** declare and read in sequences ** */
  FastaSeqSlurper seqSlurper;
    
  const std::vector<LabeledSequence>
    seqs(  seqSlurper.slurpLabeledSequences( *args.fastaIstreamPtr )  );

  /* ** declare and initialized connected componentent computer ** */
  ConnectedComponentOnlineComputer ccoc( seqs.size() );

  for( size_t seqIdx = 0;  seqIdx < seqs.size(); ++seqIdx ){
    ccoc.addNode( seqIdx );
  }
    

  const AminoScoreIdentity aminoScoreIdentity( aminoResidueIndexMap );

  for( size_t seqIdx0 = 0;  seqIdx0 < seqs.size() - 1;  ++seqIdx0 ){

    const LabeledSequence& seq0 = seqs.at(seqIdx0);


    SeqGlobalAffineAligner aligner( aminoScoreIdentity, seq0.residueIndices() );


      
    for( size_t seqIdx1 = seqIdx0 + 1; seqIdx1 < seqs.size(); ++seqIdx1 ){

      if(  ccoc.getComponent( seqIdx0 ) == ccoc.getComponent( seqIdx1 )  ){
	// seq0 and seq1 are known to be in the same cluster to skip alignment step
	continue;
      }


      const LabeledSequence& seq1 = seqs.at(seqIdx1);

      FLEArray<alignScoreT> suffixBounds = 
	SeqGlobalAffineAligner::bestPossibleSuffixScore( aminoScoreIdentity,
							 seq1.residueIndices() );

      /* ** compute minimum alignment score for edge in cluster graph
       * based on formula:
       *   adjusted identity = (# matches + adjust) / (max length + adjust)
       */


      const double maxLength  =  std::max( seq0.size(), seq1.size() );

      const double& af = args.lengthAdjustmentFactor;

      GDB_ASSERTF( af < maxLength,
		   "lengthAdjustmentFactor = %g,  too big for averageLength = %g",
		   args.lengthAdjustmentFactor, maxLength );

      double minScore
	=  args.identityThreshold * ( af + maxLength ) - af;

      // compensate for fact that SeqGlobalAffineAligner currently penalizes terminal gaps
      // would be better to modify SeqGlobalAffineAligner...
      if( seq0.length() != seq1.length() ){
	minScore  +=  abs( seq0.length() - seq1.length() ) * aminoScoreIdentity.gapExtension();
      }

      const double boundedScore
	= aligner.scoreBounded( seq1.residueIndices(),
				suffixBounds,
				(alignScoreT) minScore );

      /* ** add edge if seq0 and seq1 are sufficiently similar ** */
      if( boundedScore > minScore ){
	ccoc.addEdge( seqIdx0, seqIdx1 );
      }
    } // end for seqIdx1
  } // end for seqIdx0


  /* ** extract connected component information and transform it
   * ** so that the sequences can be printed with sequences in the
   * ** same connected component grouped together
   */
  FLEArray<nodeIndexT> seqComponents( seqs.size() );
  ccoc.getNodeComponents( seqComponents );

  /* ** compute permutation of sequence indices PERM, such that
   * ** i < j  -->  component[ perm[i] ]  ≦  component[ perm[j] ]
   */
  const FLEArray<size_t> seqIdx_sortedByComponent
    = FLEArrayIdentityPermutation( seqs.size() );

  std::stable_sort( seqIdx_sortedByComponent.begin(),
		    seqIdx_sortedByComponent.end(),
		    FLEArrayByIndexSortingPredicate<nodeIndexT>(seqComponents) );


  for( size_t seqIdx = 0;  seqIdx < seqIdx_sortedByComponent.size();  ++seqIdx ){

    /* ** skip singleton clusters ** */
    const nodeIndexT curComponent = seqComponents(seqIdx);
    if(   ( (seqIdx == seqs.size()-1) || (seqComponents(seqIdx+1) != curComponent) )
	  && ( (seqIdx == 0)          || (seqComponents(seqIdx-1) != curComponent) )  ){
      continue;
    }

    // print cluster
    std::cout << seqs.at( seqIdx ).name()   << "\t"
	      << seqs.at( seqIdx ).label(1) << "\t"
	      << seqComponents( seqIdx ) << std::endl;

    // mark cluster boundaries
    if( (seqIdx < seqs.size()-1) && (seqComponents(seqIdx+1) != curComponent) ){
      std::cout << "===\n";
    }
  }

} // end function clusterByIdentity

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv,
			 "-i identityThreshold [-a lengthAdjustmentFactor] [fastaFile]" );

  argP.setDoc( "--man", "\
OUTPUT\n\
      sequenceID  sequenceClassLabel  clusterNumber\n\
\n\
OPTIONS\n\
\n\
      "Q(ADJUSTMENT_FLAGS)"\n\
    Adjustment factor, positive value raises the \"identity score\" of long sequences\n\
    relative to their actually identity.\n\
\n\
        adjusted identity = (# matches + adjust) / (max length + adjust)\n\
");

  argP.printDoc();

  argP.setOrDie( args.identityThreshold, Q(IDENTITY_FLAGS)   );

  if( args.identityThreshold > 1.0 ){
    argP.die( "identity Threshold should be in (0.0,1.0)" );
  }

  args.lengthAdjustmentFactor = 0.0; // default.
  argP.set( args.lengthAdjustmentFactor, Q(ADJUSTMENT_FLAGS) );

  args.fastaIstreamPtr = argP.getIstreamPtr( 1 );
  
  argP.dieIfUnusedArgs();
  cbrc::clusterByIdentity();
  return 1;
}

