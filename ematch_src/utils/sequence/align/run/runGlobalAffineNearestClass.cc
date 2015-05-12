/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.5.1
 *  Last Modified: $Date: 2009/06/21 01:26:12 $
 *  
 *  Purpose: Ouput data to investigate the relationship between the alignment score
 *           to the best database hit and the probability of the sequence belonging
 *           to the same class as the database hit.
 *           
 */
#include <iostream>
#include <fstream>
#include <boost/foreach.hpp>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "../SimilaritySearchableSequences.hh"


std::string  arg_filename0;
std::string  arg_filename1;
bool         arg_ignoreNotSelf;


namespace cbrc{

  void runGlobalAffineNearestClass(){

    FastaSeqSlurper seqSlurper;
    std::ifstream fastaIfstream;


    /* ***** Open Files and slurp in sequences ***** */
    perlish::openOrDie( fastaIfstream, arg_filename0 );

    const std::vector<LabeledSequence> seqs0
      = seqSlurper.slurpLabeledSequences( fastaIfstream );

    fastaIfstream.close();

    perlish::openOrDie( fastaIfstream, arg_filename1 );

    const std::vector<LabeledSequence> seqs1
      = seqSlurper.slurpLabeledSequences( fastaIfstream );
    


    const AminoScorePam120 aScore;

    SimilaritySearchableSequences searcher( aScore, seqs0 );

    std::cout  <<  "#       query info         |   neighbor info    | match\n";
    std::cout  <<  "#   ID      class    length     ID      class     score \n";


    BOOST_FOREACH(  const LabeledSequence&  seq1,  seqs1  ){

      const SimilaritySearchableSequences::indexScorePairT match1
	= (  arg_ignoreNotSelf
	? searcher.bestMatch( seq1 )
	: searcher.bestMatch( seq1, labeledSequencePairPredicate_namesEqual )
	     );
	
      const std::string  matchClass  =  seqs1[match1.first].className();

      printf( "%10s %9s %5zu  %10s %9s %6d\n",
	      seq1.name().c_str(),
	      seq1.className().c_str(),
	      seq1.length(),
	      seqs0[match1.first].name().c_str(),
	      matchClass.c_str(),
	      match1.second );
    }

  } // end runGlobalAffineSearcher

}; // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "[--ignore-not-self] file0 [file1]" );

  argvP.setDoc( "-h|--help",
		"\
All sequences in file0 are compared to all sequences in file1.\n\
If file1 is omitted, all pairs of sequences in file0 are compared"
		);

  argvP.printDoc();

  arg_ignoreNotSelf  =  argvP.hasFlag( "--ignore-not-self" );

  argvP.setOrDie( arg_filename0, 1 );

  argvP.set( arg_filename1, 2 );

  if( !arg_filename1.size() )  arg_filename1 = arg_filename0;

  argvP.dieIfUnusedArgs();

  cbrc::runGlobalAffineNearestClass();

  return 1;
}
