/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.4.30
 *  Last Modified: $Date: 2009/05/05 12:04:08 $
 *
 *  Description: 
 *
 *  Purpose: Created for
 */
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastaRecordReader.hh"


static std::string    arg_messageToAppend;
static std::istream*  arg_fastaIstreamPtr  =  &std::cin;

namespace cbrc{
  void fastafmtAppendToRecordHeads(){
    
    FastaRecordReader fastaReader( *arg_fastaIstreamPtr );

    while( const FastaRecord* const  recordPtr  =  fastaReader.nextRecord()  ){
      std::cout << recordPtr->headLine() << arg_messageToAppend << std::endl;
      BOOST_FOREACH( const std::string line, recordPtr->bodyLines() ){
	std::cout << line << std::endl;
      }
    }

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "messageToAppend [fastaFile]" );

  const std::string description( "Append messageToAppend to fasta stream.\n\
output goes to std::cout\n" );
  

  argvP.setDoc( "help", description );
  argvP.setDoc( "man",  description );

  argvP.setOrDie( arg_messageToAppend, 1 );

  argvP.set( arg_fastaIstreamPtr, 2 );

  argvP.dieIfUnusedArgs();
  cbrc::fastafmtAppendToRecordHeads();
  return 1;
}

