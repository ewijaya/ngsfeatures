/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul Horton, All rights reserved.
 *  Creation Date: 2004.3.8
 *  Last Modified: $Date: 2009/06/15 08:01:22 $
 *  
 *  Purpose: Calculate scores of all pairwise combinations of sequences from an input fasta file.
 */
#include <iostream>
#include <fstream>
#include <boost/foreach.hpp>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "../SeqGlobalAffineAligner.hh"


typedef  cbrc::Alignment::alignScoreT  alignScoreT;

std::string    arg_filename0;
std::string    arg_filename1;
std::ifstream  arg_fastaFile1;
bool           arg_useBound;
alignScoreT    arg_bound;

namespace cbrc{

  void allPairsGlobalAffineScore(){

    const AminoScorePam120 substitutionCostFunction;

    FastaSeqSlurper seqSlurper;

    std::vector<LabeledSequence> seqs0;

    std::ifstream fastaIfstream;

    perlish::openOrDie( fastaIfstream, arg_filename0 );

    seqSlurper.slurpLabeledSequences( seqs0, fastaIfstream, aminoResidueIndexMap );


    fastaIfstream.close();

    perlish::openOrDie( fastaIfstream, arg_filename1 );

    std::vector<LabeledSequence> seqs1;

    seqSlurper.slurpLabeledSequences( seqs1, fastaIfstream, aminoResidueIndexMap );


    BOOST_FOREACH( const LabeledSequence& seq0, seqs0 ){

      std::cout  <<  seq0.name();

      SeqGlobalAffineAligner gas( substitutionCostFunction, seq0.residueIndices() );

      BOOST_FOREACH( const LabeledSequence& seq1, seqs1 ){

	if( arg_useBound ){
	  if(  gas.lengthDifferenceBasedScoreUpperBound( seq1.residueIndices() )  <  arg_bound  ){
	    continue;
	  }

	  const Alignment::alignScoreT  score  =  gas.score( seq1.residueIndices() );

	  if( score >= arg_bound ) continue;

	  std::cout  <<  " "  <<  seq1.name()  <<  " "  <<  score;

	}
	else{ // do not use bound
	  std::cout  <<  " "  <<  seq1.name()  <<  " "  <<  gas.score( seq1.residueIndices() );
	}
      } // end for seq1

      std::cout  <<  std::endl;

    } // end for seq0

  } // end allPairsGlobalAffineScore

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "[-b scoreBound] file0 [file1]" );

  argvP.setDoc( "-h|--help",
		"\
All sequences in file0 are compared to all sequences in file1.\n\
If file1 is omitted, all pairs of sequences in file0 are compared"
		);

  argvP.printDoc();

  arg_useBound  =  (argvP.set( arg_bound, "-b" )).size();

  argvP.setOrDie( arg_filename0, 1 );

  argvP.set( arg_filename1, 2 );

  if( !arg_filename1.size() )  arg_filename1 = arg_filename0;

  argvP.dieIfUnusedArgs();

  cbrc::allPairsGlobalAffineScore();

  return 1;
}
