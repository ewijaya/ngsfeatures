/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.7
 *  Last Modified: $Date: 2009/05/21 10:56:09 $
 *
 *  Purpose: try code involving SprotToFasta
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/LabeledAsciiSeq.hh"
#include "utils/sequence/readers/fasta/FastaWriter.hh"
#include "../SprotReader.hh"


struct argsT{
  std::istream* sprotIstreamPtr;
} args;


namespace cbrc{
  void sprotToFasta(){
    SprotReader sprotReader;
    FastaWriter fastaWriter;

    while( sprotReader.readRecord( *args.sprotIstreamPtr ) ){

      std::string fastaId(  sprotReader.accession() + std::string("|") + sprotReader.id()  );

      LabeledAsciiSeq labeledSeq( sprotReader.sequence(), fastaId );

      fastaWriter.print( labeledSeq );
    }

  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "[sprotFile]" );
  
  args.sprotIstreamPtr = argP.getIstreamPtr( 1 );

  argP.dieIfUnusedArgs();
  cbrc::sprotToFasta();
  return 1;
}

