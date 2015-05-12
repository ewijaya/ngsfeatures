/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/06/20 03:25:55 $
 *  
 *  Input: Two multifasta files
 *
 *  Output: The closest match (by global affine gap measure) of each sequence in file1 to a sequence in file2
 *
 *  Sketch: For each sequence in file1:
 *            A quick lower bound calculation is done for each sequence in file2
 *            The sequences in file2 are sorted by this bound in decreasing  order.
 *            Exact calculation using lower bounds to improve running time is done for each sequence in
 *            file2.
 *
 *  The reason for sorting by lower bound is to try to align close matches first. The scores
 *  obtained are used as lower bounds and it is good to get good lower bounds early
 *  for more aggressive pruning.
 */
#include <iostream>
#include <fstream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "../SeqGlobalAffineAligner.hh"
#include "../AminoScorePam120.hh"


// Global variables
std::vector< cbrc::LabeledSequence >  seqs0;
std::vector< cbrc::LabeledSequence >  seqs1;
bool  ignoreSelf;


namespace cbrc{

  void allPairsNearestGlobalAffineScore(){

    AminoScorePam120 aminoScore;


    FLEArray<alignScoreT> seqs1Bounds( seqs1.size() );
    FLEArray<size_t> seqs1byBound = FLEArrayIdentityPermutation( seqs1.size() );
    if( seqs1.size() < 1 ){
      std::cout << "Error: second sequence set is empty\n";
      exit( -1 );
    }

    for( size_t i = 0; i < seqs0.size(); ++i ){
      size_t j = 0;
      while(  ignoreSelf && ( seqs0[i].name() == seqs1[j].name() )  ){
	seqs1Bounds[j] = alignScoreTMin;
	++j;
      }
      if( j >= seqs1.size() ) break;
      // first get rough feasible bound for each sequence.
      size_t bestMatchIndex = j;
      SeqGlobalAffineAligner gas( aminoScore, seqs0[i].residueIndices() );
      alignScoreT bestLowerBound = gas.feasibleBound( seqs1[j].residueIndices() );
      seqs1Bounds[j] = bestLowerBound;
      for( ; j < seqs1.size(); ++j ){
	if(  ignoreSelf && ( seqs0[i].name() == seqs1[j].name() )  ){
	  seqs1Bounds[j] = alignScoreTMin;
	  continue;
	}
	seqs1Bounds[j] = gas.feasibleBound( seqs1[j].residueIndices() );
	if( seqs1Bounds[j] < bestLowerBound ){ 
	  bestLowerBound = seqs1Bounds[j];
	  bestMatchIndex = j;
	}
      }
      // sort (indirectly) seqs1 by bound
      std::stable_sort( seqs1byBound.begin(), seqs1byBound.end(), 
      			FLEArrayByIndexSortingPredicate
			<alignScoreT, std::greater<alignScoreT> >(seqs1Bounds)
			);
      
      // do the actual alignment calculations.
      j = 0;
      while(  ignoreSelf && ( seqs0[i].name() == seqs1[seqs1byBound[j]].name() )  ) ++j;
      if( j >= seqs1.size() ) break;
      FLEArray<alignScoreT> suffixBounds = SeqGlobalAffineAligner::
	bestPossibleSuffixScore( aminoScore, seqs1[seqs1byBound[j]].residueIndices() );
      alignScoreT curBound = gas.scoreBounded( seqs1[seqs1byBound[j]].residueIndices(),
					       suffixBounds, bestLowerBound );
      if( curBound > bestLowerBound ){
	bestLowerBound = curBound;
	bestMatchIndex = seqs1byBound[j];
      }
      for( ; j < seqs1.size(); ++j ){
	if(  ignoreSelf && ( seqs0[i].name() == seqs1[seqs1byBound[j]].name() )  ) continue;

	FLEArray<alignScoreT> suffixBounds = SeqGlobalAffineAligner::
	  bestPossibleSuffixScore( aminoScore, seqs1[seqs1byBound[j]].residueIndices() );
	curBound = gas.scoreBounded( seqs1[seqs1byBound[j]].residueIndices(),
				    suffixBounds, bestLowerBound );
	if( curBound > bestLowerBound ){
	  bestLowerBound = curBound;
	  bestMatchIndex = seqs1byBound[j];
	}
      }
      std::cout << seqs0[i].name()              << " "
		<< seqs1[bestMatchIndex].name() << " "
		<< bestLowerBound               << std::endl;
    }
  }
}; // end namescape cbrc



#define IGNORE_SELF  -i|--ignore-self

int main( int argc, const char* argv[] ){


  /* ********** Initialize and Set Documentation ********** */

  cbrc::ArgvParser argvP( argc, argv, "[-i] file0 [file1]" );

  const std::string man(
			"\
NAME\n\
    $0\n\
\n\
SYNOPSIS\n\
    Compute most similar sequence from file1 of each sequence in file0\n\
\n\
ARGUMENTS\n\
    file0, file1\n\
        multifasta format sequence filenames. \"-\" can be used to denote standard in.\n\
        If file1 is omitted, all pairs withing file0 are compared.\n\
\n\
OPTIONS\n\
    "Q(IGNORE_SELF)"\n\
        Ignore sequence pairs with the same name.\n"
			);

  argvP.setDoc( "-help|--help|-man|--man", man );

  argvP.printDoc();


  /* ********** Parse Command Line ********** */

  ignoreSelf  =   argvP.hasFlag( Q(IGNORE_SELF) );

  std::string filename0;
  std::string filename1( "" );

  argvP.setOrDie( filename0, 1 );
  argvP.set     ( filename1, 2 );

  if( !filename1.size() )  filename1 = filename0;

  if( (filename0 == "-") && (filename1 == "-") ){
    argvP.die( "Standard in cannot be used for both input streams" );
  }

  argvP.dieIfUnusedArgs();

  
  /* ********** Read in Sequences ********** */

  const cbrc::ResidueIndexMap&  residueIndexMap  =  cbrc::aminoScorePam120.residueIndexMap();
  
  cbrc::FastaSeqSlurper seqSlurper;

  { // Open first file
    std::istream* fastaIstreamPtr  =  argvP.openIstream( filename0 );

    seqSlurper.slurpLabeledSequences( seqs0, *fastaIstreamPtr, residueIndexMap );

    // If filenames are the same, then just copy seqs0 to seqs1 and jump to main computation.
    if( filename0 == filename1 ){
      seqs1.assign( seqs0.begin(), seqs0.end() );
      cbrc::allPairsNearestGlobalAffineScore();  // main() EXIT POINT
    }

    argvP.closeIstream( fastaIstreamPtr );
    delete fastaIstreamPtr;
  }

  { // Open second file
    std::istream* fastaIstreamPtr  =  argvP.openIstream( filename1 );

    seqSlurper.slurpLabeledSequences( seqs1, *fastaIstreamPtr, residueIndexMap );

    argvP.closeIstream( fastaIstreamPtr );
    delete fastaIstreamPtr;
  }

  cbrc::allPairsNearestGlobalAffineScore();

  return 1;
}
