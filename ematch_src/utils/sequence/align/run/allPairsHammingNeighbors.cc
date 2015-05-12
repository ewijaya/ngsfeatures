/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.10
 *  Last Modified: $Date: 2009/06/02 08:29:58 $
 *
 *  Input: 1) set of sequences multifasta format
 *         2) hamming distance threshold
 *
 *  Ouput: sequence pairs with hamming distance < THRESHOLD
 *
 */
#include <iostream>
#include <iomanip>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "../HammingDistanceComputer.hh"
#define  PERCENT_FLAGS  -p|--percent


namespace cbrc{
  void allPairsHammingNeighbors( const bool& byPercent,
				 const double& threshold,
				 std::istream& fastaInputStream
				 ){

    /* debug */ std::cout << __FILE__ << ":" << __LINE__ << " threshold: " << threshold;
    /* debug */ std::cout << " byPercent: " << byPercent << std::endl;

    FastaSeqSlurper seqSlurper;

    std::vector<LabeledSequence> seqs = seqSlurper.slurpLabeledSequences( fastaInputStream );


    for(  size_t seqIdx0 = 0;  seqIdx0 < seqs.size() - 1;  ++seqIdx0  ){
      const LabeledSequence& seq0 = seqs[seqIdx0];

      for(  size_t seqIdx1 = seqIdx0 + 1;  seqIdx1 < seqs.size();  ++ seqIdx1  ){
	const LabeledSequence& seq1 = seqs[seqIdx1];

	if( byPercent ){
	  const size_t shorterLen  =  std::min( seq0.length(), seq1.length() ); 

	  const double hammingDistanceThreshold = threshold * shorterLen / 100;

	  const size_t distance
	    = HammingDistanceComputer::computeBounded( seq0, seq1, hammingDistanceThreshold );

	  if(  distance < hammingDistanceThreshold  ){
	    std::cout << seq0.name() << "\t" << seq1.name() << "\t"
		      << std::setw( 5 )
		      << 100 * distance / (double) shorterLen << std::endl;
	  }
	}
	else{ // user stipulated threshold is absolute Hamming distance
	  size_t distance
	    = HammingDistanceComputer::computeBounded( seq0, seq1, threshold );

	  if( distance < threshold ){
	    std::cout << seq0.name() << "\t" << seq0.className() << "\t" 
		      << seq1.name() << "\t" << seq1.className() << "\t"
		      << distance << std::endl;
	  }
	}
      } // for seqIdx1
    } // for seqIdx0
  } // allPairsHammingNeighbors

} // end namescape cbrc


int main( int argc, const char* argv[] ){
  /* ***** Documentation ***** */
  cbrc::ArgvParser args( argc, argv, "["Q(PERCENT_FLAGS)"] threshold [multifastaFile]" );

  std::string man( "\
NAME\n\
    $0\n\
SYNOPSIS\n\
    $0 ["Q(PERCENT_FLAGS)"] threshold [multifastaFile]\n\
\n\
DESCRIPTION\n\
    Print sequence pairs from multifastaFile whose Hamming distance\n\
    is less than THRESHOLD\n\
\n\
OPTIONS\n\
\n\
    "Q(PERCENT_FLAGS)"\n\
\n\
        For each sequence pair comparison, interpreset THRESHOLD as percentage of\n\
        $l$, the length of the shorter sequence. Output pair if the Hamming distance\n\
        is less than THRESHOLD * $l$\n\
" ); // end manual

  args.setDoc(  "-help", man );
  args.setDoc( "--help", man );
  args.setDoc(  "-man",  man );
  args.setDoc( "--man",  man );



  /* ***** Argument Parsing ***** */
  size_t curArg = 0;

  bool byPercent(  args.hasFlagDelete( Q(PERCENT_FLAGS) )  );

  double threshold;
  args.setOrDie( threshold, ++curArg );

  std::istream& fastaInputStream(  args.getIstream( ++curArg )  );

  args.dieIfUnusedArgs();


  cbrc::allPairsHammingNeighbors( byPercent, threshold, fastaInputStream );
  return 1;
}

