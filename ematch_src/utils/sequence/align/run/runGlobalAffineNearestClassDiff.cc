/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul Horton, All rights reserved.
 *  Creation Date: 2004.5.1
 *  Last Modified: $Date: 2009/06/21 01:31:48 $
 *  
 *  Purpose: test GlobalAffineSearcher.
 */
#include <iostream>
#include <fstream>
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "utils/sequence/LabeledSequencePairPredicate_logicalOr.hh"
#include "../SimilaritySearchableSequences.hh"

namespace cbrc{
  void runGlobalAffineSearcher( const std::string& filename0,
				const std::string& filename1,
				const bool&        ignoreSelf ){

    FastaSeqSlurper seqSlurper;

    std::ifstream  fastaIstream;

    perlish::openOrDie( fastaIstream, filename0 );

    const std::vector<LabeledSequence> seqs0
      = seqSlurper.slurpLabeledSequences( fastaIstream );

    fastaIstream.close();

    perlish::openOrDie( fastaIstream, filename1 );

    const std::vector<LabeledSequence> seqs1
      = seqSlurper.slurpLabeledSequences( fastaIstream );

    fastaIstream.close();


    std::vector<LabeledSequencePairPredicate*> eliminationConditionList1;
    std::vector<LabeledSequencePairPredicate*> eliminationConditionList2;

    LabeledSequencePairPredicate_1stSeqClassFilter classFilter( "dummy" );
    eliminationConditionList2.push_back( &classFilter );
    if( ignoreSelf ){
      eliminationConditionList1
	.push_back( (LabeledSequencePairPredicate*) &labeledSequencePairPredicate_namesEqual );
      eliminationConditionList2
	.push_back( (LabeledSequencePairPredicate*) &labeledSequencePairPredicate_namesEqual );
    }

    const LabeledSequencePairPredicate_logicalOr eliminationFilter1( eliminationConditionList1 );
    const LabeledSequencePairPredicate_logicalOr eliminationFilter2( eliminationConditionList2 );

    const AminoScorePam120 aScore;
    SimilaritySearchableSequences searcher( aScore, seqs1 );
    for( size_t i = 0; i < seqs0.size(); ++i ){

      SimilaritySearchableSequences::indexScorePairT match1
	= searcher.bestMatch( seqs0[i], eliminationFilter1 );

      const std::string  matchClass  =  seqs1[match1.first].className();
      classFilter.setClassName( matchClass );

      SimilaritySearchableSequences::indexScorePairT match2
	= searcher.bestMatch( seqs0[i], eliminationFilter2 );

      if( match2.first == seqs1.size() ){
	std::cerr << "Error. No match found. Perhaps all the sequences are the same class?\n";
	exit( -1 );
      }
      
      const double bestPossibleScore =
	SeqGlobalAffineAligner
	::singleSeqBasedScoreUpperBound( aScore, seqs0[i].residueIndices() );

      std::cout  <<  seqs0[i].name()                  <<  "\t"
		 <<  seqs0[i].length()                <<  "\t"
		 <<  bestPossibleScore                <<  "\t"
 		 <<  seqs0[i].className()             <<  "\t"
		 <<  seqs1[match1.first].name()       <<  "\t"
		 <<  matchClass                       <<  "\t"
 		 <<  match1.second                    <<  "\t"
		 <<  seqs1[match2.first].name()       <<  "\t"
		 <<  seqs1[match2.first].className()  <<  "\t"
		 <<  match2.second                    <<  "\n"  <<  std::flush;
    }
  }
}; // end namescape cbrc


int main( int argc, char** argv ){
  std::string usage = "usage: ";
  bool ignoreSelf = true;
  int curArg = 1;
  usage += argv[0];
  usage += "[--noIgnore] fastaFile1 fastaFile2\n";
  if( argc < 3 ){
    std::cout << usage;
    exit( -1 );
  }
  if( !strcmp( argv[curArg], "--noIgnore" ) ){
    ++curArg;
    ignoreSelf = false;
  }
  if( argc-curArg != 2 ){
    std::cout << usage;
    exit( -1 );
  }
  std::clog << "opening multifasta files: " << argv[curArg] << " " << argv[curArg+1] << std::endl;
  std::string filename1( argv[curArg] );
  std::string filename2( argv[curArg+1] );
  cbrc::runGlobalAffineSearcher( filename1, filename2, ignoreSelf );
  return 1;
}
