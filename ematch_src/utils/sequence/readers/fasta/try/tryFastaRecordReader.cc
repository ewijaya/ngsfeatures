/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/05/06 09:50:39 $
 *  
 *  Purpose: try out FastaRecordReader.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/stringUtils.hh"
#include "../FastaRecordReader.hh"
#define QQ(x) #x
#define Q(x) QQ(x)


static std::istream*  arg_fastaIstreamPtr  =  &std::cin;
static std::string    arg_delimiter("");


namespace cbrc{

  void tryFastaRecordReader(){


    /* ***** set up FastaInputOptions object ***** */
    FastaInputOptions fastaInputOptions( defaultFastaInputOptions );

    if( arg_delimiter.size() ){
      fastaInputOptions.setBetweenLabelsDelimiterRegex( boost::regex(arg_delimiter) );
    }


    /* ***** construct FastaRecordReader and set parameters ***** */
    FastaRecordReader fastaRecordReader( *arg_fastaIstreamPtr,
					 fastaInputOptions
					 );

    // read and dump records
    while(   const FastaRecord* const  record  =  fastaRecordReader.nextRecord()  ){
      std::cout << "name: " << record->name()     << std::endl;

      std::cout << "labels:";
      for(  size_t i = 0;  i < record->labels().size();  ++i  ){
	std::cout << " |" << record->labels()[i] << "|";
      }
      std::cout << std::endl;

      std::cout << "seq: "  << record->seq() << std::endl;
    }
  } // end tryFastaRecordReader


}; // end namescape cbrc



#define DELIMITER_FLAG  -d|--delimiter
#define USAGE           Usage: $0 [-d delimiter] [fastafile]

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, Q(USAGE) );

  argvP.setDoc( "-h|--help", "\
"Q(USAGE)"\n\
\n\
input taken from std::cin if fastafile not given\n\
\n\
EXAMPLE:\n\
\n\
  $0 -d \'\\t+\' ../exampleData/tabSeparated.fasta"
		); /* end help message. */

  argvP.set( arg_delimiter, Q(DELIMITER_FLAG) );

  // replace "\t" with tab, since tab is hard to input from command line.
  cbrc::stringUtils::stringReplaceAll( arg_delimiter,
				       std::string("\\t"),
				       std::string("\t") );

  argvP.printDoc();

  argvP.set( arg_fastaIstreamPtr, 1 );

  argvP.dieIfUnusedArgs();

  cbrc::tryFastaRecordReader();

  return 1;
}

