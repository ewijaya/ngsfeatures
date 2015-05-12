/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.10.25
 *  Last Modified: $Date: 2009/05/18 22:12:33 $
 *
 *  Description: See header file.
 */
#include <iostream>
#include "boost/foreach.hpp"
#include "utils/gdb/gdbUtils.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastaSeqSlurper.hh"

struct argsT{
  std::istream* fastaIstreamPtr1;
  std::istream* fastaIstreamPtr2;
} args;


namespace cbrc{
  void fastaPrintDuplicateSeqs2Streams(){

    FastaSeqSlurper fastaSlurper;

    while(  fastaSlurper.nextRecord( *args.fastaIstreamPtr1 )  ){

      const std::string seqFrom1     = fastaSlurper.curRecord().seq();
      const std::string seqFrom1name = fastaSlurper.curRecord().name();
      bool neverSeenMatchTo_seqFrom1 = true;

      // reset fastaIstream2 to beginning of stream.
      args.fastaIstreamPtr2->clear();
      args.fastaIstreamPtr2->seekg( 0, std::ios_base::beg );

      while(  fastaSlurper.nextRecord( *args.fastaIstreamPtr2 )  ){
	if( seqFrom1 == fastaSlurper.curRecord().seq() ){
	  if( neverSeenMatchTo_seqFrom1 ){
	    neverSeenMatchTo_seqFrom1 = false;
	    std::cout << seqFrom1name;
	  }
	  std::cout << "\t" << fastaSlurper.curRecord().name() << std::endl;
	}
      }
    }
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "fastaInputFile1 fastaInputFile2" );

  argP.setDoc( "-help|--help",
	       "This program prints any duplicate sequences found between two input streams" );

  argP.printDoc();

  args.fastaIstreamPtr1 = argP.getIstreamPtrOrDie( 1 );
  args.fastaIstreamPtr2 = argP.getIstreamPtrOrDie( 2 );

  DO_OR_DIEF( *args.fastaIstreamPtr1 != *args.fastaIstreamPtr2,
	      "The two input files must differ from each other" );

  argP.dieIfUnusedArgs();

  cbrc::fastaPrintDuplicateSeqs2Streams();
  return 1;
}

