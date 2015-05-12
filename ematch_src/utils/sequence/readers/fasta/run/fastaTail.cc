/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul Horton, All rights reserved.
 *  Creation Date: 2007.10.13
 *  Last Modified: $Date: 2009/05/21 12:47:14 $
 *
 *  Purpose: Like unix tail, but acting on fasta records
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../FastaRecordReader.hh"
#include "../fastafmtFunctions.hh"

#define BYTE_FLAGS   -c|--bytes
#define RECORD_FLAGS -r|--records


struct argT{
  int             numToOutput;  // num bytes/records to output (or skip if numToOutput < 0)
  std::ifstream   ifstream;
} args;



namespace cbrc{


// print tail lines and bytes of fasta records according to value of args.numToOutput.
void fastaTailBytes(){

  const int& numBytes  =  args.numToOutput;

  FastaRecordReader fr( args.ifstream );

  while(  fr.nextRecord()  ){

    const std::string&  curSeq  =  fr.curRecord().seq();

    const int  seqLen  =  (int) curSeq.size();

    const std::string seq  =
      ( numBytes >= 0 ) ? curSeq.substr(   std::max( 0, seqLen - numBytes )   )
      :  /* else */       curSeq.substr(   0,  std::max( 0, seqLen + numBytes )   );

    // print results
    std::cout  <<  fr.curRecord().headLine()  <<  std::endl
	       <<  seq                        <<  std::endl;
  }

} // end function fastaTailBytes



void fastaHeadRecords(){

  /* ***** count records and restore file pointer ***** */
  const int numRecords  =  (int)  fastafmtFunctions::countRecords( args.ifstream );
  args.ifstream.clear(); args.ifstream.seekg( 0 );


  if( args.numToOutput < 0 ){

    if( -args.numToOutput > numRecords )  return; // EXIT, nothing to do in this case.

    fastafmtFunctions::printRecords(  args.ifstream,  numRecords + args.numToOutput  );

    return;
  }

  // else:  args.numToOutput ≧ 0
  const size_t  numToOutput  =  std::min( numRecords, args.numToOutput );

  fastafmtFunctions::skipRecords( args.ifstream, numRecords - numToOutput );

  fastafmtFunctions::printRecords( args.ifstream );

} // end fastaHeadRecords


} // end namescape cbrc



int main( int argc, const char* argv[] ){

  /* ******* Construct argv parser and set up documentation. ******* */
  cbrc::ArgvParser argvP( argc, argv, 
			 "\
Usage: $0 "Q(BYTE_FLAGS)"   numBytes fastaFile\n\
       $0 "Q(RECORD_FLAGS)" numRecords fastaFile\n"
			 );

  std::string man( "\
NAME\n\
        $0\n\
SYNOPSIS\n\
       $0 "Q(BYTE_FLAGS)"   numBytes fastaFile\n\
       $0 "Q(RECORD_FLAGS)" numRecords fastaFile\n\
\n\
DESCRIPTION\n\
        Print first part of a fasta format file.\n\
\n\
        "Q(BYTE_FLAGS)"\n\
\n\
            print fasta head line and final numBytes characters of each sequence,\n\
            except print all BUT the final -numBytes characters, if numBytes < 0.\n\
\n\
        "Q(RECORD_FLAGS)"\n\
\n\
            print final numRecords records in fastaFile.\n\
            except print all BUT the final -numRecords records, if numRecords < 0.\n\
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

  argvP.setOrDie( args.ifstream, 1 );

  // dispatch to appropriate function
  countBytes ? cbrc::fastaTailBytes() : cbrc::fastaHeadRecords();
  
  return 1;
}

