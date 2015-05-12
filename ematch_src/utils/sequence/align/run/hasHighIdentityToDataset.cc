/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2008.6.13
 *  Last Modified: $Date: 2009/06/09 05:58:28 $
 *
 *  Description: For each query sequence, compute whether or not it has high
 *               identity with some dataset sequence(s)
 *
 *  Purpose: Created for updating WoLF PSORT dataset
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/FLArray/FLEArray.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "../SeqGlobalAffineAligner.hh"
#include "../AminoScoreIdentity.hh"
#define ADJUSTMENT_FLAG        -a|--adjustment-factor
#define IDENTITY_FLAG          -i|--identity
#define OUTPUT_ALL_PAIRS_FLAG  -o|--output-all-pairs
#define PROGRESS_TO_STDERR     -p|--progress-to-stderr
#define USAGE  "[-o] [-p] -i identityThreshold [-a lengthAdjustmentFactor] datasetFastafile queryFastaFile"


bool           arg_outputAllPairs;
bool           arg_progressToStderr;
double         arg_identityThreshold;
double         arg_lengthAdjustmentFactor;
std::istream*  arg_fastaIstreamPtr1;
std::istream*  arg_fastaIstreamPtr2;



namespace cbrc{


void hasHighIdentityToDataset(){
    
  /* ** declare and read in sequences ** */
  FastaSeqSlurper seqSlurper;
    
  const std::vector<LabeledSequence>
    datasetSeqs(  seqSlurper.slurpLabeledSequences( *arg_fastaIstreamPtr1 )  );

  const std::vector<LabeledSequence>
    querySeqs(  seqSlurper.slurpLabeledSequences( *arg_fastaIstreamPtr2 )  );


  const AminoScoreIdentity aminoScoreIdentity( aminoResidueIndexMap );

  for( size_t querySeqIdx = 0;  querySeqIdx < querySeqs.size() - 1;  ++querySeqIdx ){

    const LabeledSequence& querySeq = querySeqs.at(querySeqIdx);

    if( arg_progressToStderr )  std::cerr << querySeq.name() << std::endl;

    SeqGlobalAffineAligner aligner( aminoScoreIdentity, querySeq.residueIndices() );


    bool queryMatches  =   false;
      
    for( size_t datasetSeqIdx = querySeqIdx + 1;
	 datasetSeqIdx < datasetSeqs.size();
	 ++datasetSeqIdx ){

      const LabeledSequence& datasetSeq = datasetSeqs.at(datasetSeqIdx);

      FLEArray<alignScoreT> suffixBounds = 
	SeqGlobalAffineAligner::bestPossibleSuffixScore( aminoScoreIdentity,
							 datasetSeq.residueIndices() );

      /* ** compute minimum alignment score for edge in cluster graph
       * based on formula:
       *   adjusted identity = (# matches + adjust) / (average length + adjust)
       */
      const double maxLength  =  std::max( querySeq.size(), datasetSeq.size() );

      const double& af = arg_lengthAdjustmentFactor;

      GDB_ASSERTF( af < maxLength,
		   "lengthAdjustmentFactor = %g,  too big for averageLength = %g",
		   arg_lengthAdjustmentFactor, maxLength );

      double minScore
	=  arg_identityThreshold * ( af + maxLength ) - af;

      // compensate for fact that SeqGlobalAffineAligner currently penalizes terminal gaps
      // would be better to modify SeqGlobalAffineAligner...
      if( querySeq.length() != datasetSeq.length() ){
	minScore  +=  abs( querySeq.length() - datasetSeq.length() )
	  * aminoScoreIdentity.gapExtension();
      }

      const double boundedScore
	= aligner.scoreBounded( datasetSeq.residueIndices(),
				suffixBounds,
				(alignScoreT) minScore );

      if( boundedScore > minScore ){

	// print query sequence name if first match
	if( !queryMatches )   std::cout  <<  querySeq.name();

	queryMatches  =  true;

	// print match
	std::cout  <<  "\t"  <<  datasetSeq.name();

	// done, unless all similar pairs are to be output
	if( !arg_outputAllPairs ){
	  goto NEXT_QUERY_SEQ;
	}
      }

    } // end for datasetSeqIdx

  NEXT_QUERY_SEQ:;
    if( queryMatches )   std::cout  <<  std::endl;
  } // end for querySeqIdx

} // end function hasHighIdentityToDataset

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, USAGE );

  argvP.setOrDie( arg_identityThreshold, Q(IDENTITY_FLAG)   );

  if( arg_identityThreshold > 1.0 ){
    argvP.die( "identity Threshold should be in (0.0,1.0)" );
  }

  arg_outputAllPairs  =  argvP.hasFlag( Q(OUTPUT_ALL_PAIRS_FLAG) );

  arg_progressToStderr  =  argvP.hasFlag( Q(PROGRESS_TO_STDERR) );

  argvP.set( arg_lengthAdjustmentFactor, Q(ADJUSTMENT_FLAG) );

  arg_fastaIstreamPtr1 = argvP.getIstreamPtr( 1 );

  arg_fastaIstreamPtr2 = argvP.getIstreamPtr( 2 );

  argvP.dieIfUnusedArgs();

  cbrc::hasHighIdentityToDataset();

  return 1;
}

