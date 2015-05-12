/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.10.25
 *  Last Modified: $Date: 2009/05/17 23:06:33 $
 *  Description: See header file.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastaSeqSlurper.hh"

namespace cbrc{

  void fastaPrintDuplicateSequences( std::istream& fastaIstream ){

    FastaSeqSlurper fastaSlurper( fastaIstream );

    const std::vector<LabeledAsciiSeq> seqs = fastaSlurper.slurpSeqs();

    std::vector<bool> seqCovered( seqs.size(), false );
    std::vector<LabeledAsciiSeq> duplicates;

    for( size_t i = 0; i < seqs.size(); ++i ){
      if( seqCovered[i] ) continue;
      duplicates.clear();
      for( size_t j = 0; j < seqs.size(); ++j ){
	if( i == j ) continue;
	if( seqCovered[j] ) continue;
	if( seqs[i].seq() == seqs[j].seq() ){
	  duplicates.push_back( seqs[j] );
	  seqCovered[j] = true;
	}
      }
      if( duplicates.size() ){
	for(  size_t j = 0;  j < duplicates.size();  ++j  ){
	  std::cout  <<  duplicates[j];
	}
      }
    }
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "[fastaInputFile]" );

  args.setDoc( "-help|--help",
	       "This program prints any duplicate sequences found in the input stream" );

  args.printDoc();

  std::istream& fastaIstream(  args.getIstream(1)  );

  args.dieIfUnusedArgs();

  cbrc::fastaPrintDuplicateSequences( fastaIstream );
  return 1;
}

