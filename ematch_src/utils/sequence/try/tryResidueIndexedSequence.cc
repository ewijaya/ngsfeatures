/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.10
 *  Last Modified: $Date: 2009/06/13 07:19:32 $
 *
 *  Purpose: try or run code involving ResidueIndexedSequence
 */
#include <iostream>

#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefinedAlphabets.hh"
#include "utils/sequence/readers/fasta/FastaRecordReader.hh"
#include "../ResidueIndexedSequence.hh"


std::istream* arg_fastaIstreamPtr;


namespace cbrc{
  void tryResidueIndexedSequence(){

    FastaRecordReader fr( *arg_fastaIstreamPtr );

    const FastaRecord* const  recordPtr  = fr.nextRecordOrDie();

    const ResidueIndexMap& rim = DNAExtendedResidueIndexMap;

    ResidueIndexedSequence indSeq( rim, recordPtr->seq(), recordPtr->name() );

    std::cout << "sequence name is: " << indSeq.name() << std::endl;
    
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "[DNAsequenceFile]" );

  arg_fastaIstreamPtr  =  argvP.getIstreamPtr( 1 );

  argvP.dieIfUnusedArgs();

  cbrc::tryResidueIndexedSequence();

  return 1;
}

