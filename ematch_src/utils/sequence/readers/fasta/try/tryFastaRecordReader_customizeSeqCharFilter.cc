/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/05/06 10:00:20 $
 *  
 *  Purpose: try out FastaRecordReader with seqCharFilter customized.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/stringUtils.hh"
#include "../FastaRecordReader.hh"
#define QQ(x) #x
#define Q(x) QQ(x)
#define DELIMITER_FLAG  -d|--delimiter


static std::istream*  arg_fastaIstreamPtr  =  &std::cin;

namespace cbrc{
  void tryFastaRecordReader_customizeSeqCharFilter(){

    /* ***** set up FastaInputOptions object ***** */
    FastaInputOptions fastaInputOptions( defaultFastaInputOptions );

    fastaInputOptions.setSeqCharFilter( std::isupper );

    
    // construct FastaRecordReader and set parameters
    FastaRecordReader recordReader( *arg_fastaIstreamPtr, fastaInputOptions );


    // read and dump records
    while(   const FastaRecord* const  curRecord   =  recordReader.nextRecord()   ){
      std::cout << "name: " << curRecord->name()     << std::endl;

      std::cout << "labels:";
      for( size_t i = 0; i < curRecord->labels().size(); ++i ){
	std::cout << " |" << curRecord->labels()[i] << "|";
      }
      std::cout << std::endl;

      std::cout << "seq: "  << curRecord->seq() << std::endl;
    }
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "Usage: $0 [fastafile]\n\
\n\
input taken from std::cin if fastafile not given\n" );

  argvP.set( arg_fastaIstreamPtr, 1 );
  
  argvP.dieIfUnusedArgs();

  cbrc::tryFastaRecordReader_customizeSeqCharFilter();
  return 1;
}

