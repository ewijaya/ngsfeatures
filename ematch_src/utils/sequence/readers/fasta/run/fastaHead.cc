/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.10.13
 *  Last Modified: $Date: 2009/05/21 12:32:38 $
 *
 *  Purpose: Like unix head, but acting on fasta records
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../FastaRecordReader.hh"
#include "../fastafmtFunctions.hh"

#define BYTE_FLAGS   -c|--bytes
#define RECORD_FLAGS -r|--records


struct argT{
  int            numToOutput;  // num bytes/records to output (or skip if numToOutput < 0)
  std::istream*  istreamPtr;
} args;



namespace cbrc{


// print head lines and bytes of fasta records according to value of args.numToOutput.
void fastaHeadBytes(){

  const int& numBytes  =  args.numToOutput;

  FastaRecordReader fr( *args.istreamPtr );

  while(  fr.nextRecord()  ){

    const std::string&  curSeq  =  fr.curRecord().seq();

    const int  seqLen  =  (int) curSeq.size();

    const std::string  seq  =
      ( numBytes   >= 0 )      ?  curSeq.substr( 0, numBytes )
      :( -numBytes < seqLen )  ?  curSeq.substr( -numBytes )
      :                              "";

    // print results
    std::cout  <<  fr.curRecord().headLine()  <<  std::endl
	       <<  seq                        <<  std::endl;
  }

} // end function fastaHeadBytes



void fastaHeadRecords(){


  if( args.numToOutput < 0 ){

    fastafmtFunctions::skipRecords( *args.istreamPtr, -args.numToOutput );

    fastafmtFunctions::printRecords( *args.istreamPtr );

    return;
  }


  // else:  args.numToOutput ≧ 0

  fastafmtFunctions::printRecords( *args.istreamPtr, args.numToOutput );

} // end fastaHeadRecords


} // end namescape cbrc



int main( int argc, const char* argv[] ){

  /* ******* Construct argv parser and set up documentation. ******* */
  cbrc::ArgvParser argvP( argc, argv, 
			 "\
Usage: $0 "Q(BYTE_FLAGS)"   numBytes [fastaFile]\n\
       $0 "Q(RECORD_FLAGS)" numRecords [fastaFile]\n"
			 );

  std::string man( "\
NAME\n\
        $0\n\
SYNOPSIS\n\
       $0 "Q(BYTE_FLAGS)"   numBytes [fastaFile]\n\
       $0 "Q(RECORD_FLAGS)" numRecords [fastaFile]\n\
\n\
DESCRIPTION\n\
        Print first part of a fasta format file.\n\
\n\
        "Q(BYTE_FLAGS)"\n\
\n\
            print fasta head line and first numBytes characters of each sequence,\n\
            except print all BUT the first -numBytes characters if numBytes < 0.\n\
\n\
        "Q(RECORD_FLAGS)"\n\
\n\
            print first numRecords records.\n\
            except print all BUT the first -numRecords characters if numRecords < 0.\n\
\n\
" );

  argvP.setDoc(  "-help", man );
  argvP.setDoc( "--help", man );
  argvP.setDoc(  "-man",  man );
  argvP.setDoc( "--man",  man );

  argvP.printDoc();
  

  /* ******* Process arguments ******* */
  bool countBytes   =  argvP.hasOpt(  Q(BYTE_FLAGS)    );
  bool countRecords =  argvP.hasOpt(  Q(RECORD_FLAGS)  );

  if(  countBytes ? countRecords : !countRecords  ){
    argvP.die( "please specify whether to work on bytes or records" );
  }

  argvP.setOrDie(  args.numToOutput, Q(BYTE_FLAGS)"|"Q(RECORD_FLAGS)  );
  args.istreamPtr  =  argvP.getIstreamPtr( 1 );


  // dispatch to appropriate function
  countBytes ? cbrc::fastaHeadBytes() : cbrc::fastaHeadRecords();
  
  return 1;
}

