/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.4.21
 *  Last Modified: $Date: 2009/05/17 22:19:36 $
 *
 *  Description: sorts fasta records by sequence length
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/LabeledAsciiSeq.hh"
#include "utils/sequence/LabeledAsciiSeqPairPredicate.hh"
#include "../FastaSeqSlurper.hh"


struct argT{
  std::istream* fastaIstreamPtr;
} args;


namespace cbrc{
  void sortFastaByLength(){
    FastaSeqSlurper fastaSlurper;

    std::vector<LabeledAsciiSeq> seqs;

    fastaSlurper.slurpLabeledAsciiSeqs( seqs, *args.fastaIstreamPtr );

    std::sort( seqs.begin(), seqs.end(),
	       labeledAsciiSeqPairPredicate::seqShorter );

    for( size_t i = 0; i < seqs.size(); ++i ){
      std::cout << seqs[i];
    }
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){

  cbrc::ArgvParser argP( argc, argv, "[fastaFile]" );

  args.fastaIstreamPtr = argP.getIstreamPtr( 1 );

  argP.dieIfUnusedArgs();

  cbrc::sortFastaByLength();
  return 1;
}

