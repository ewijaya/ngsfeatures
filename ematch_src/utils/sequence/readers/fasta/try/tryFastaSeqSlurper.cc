/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/05/17 10:49:17 $
 *  
 *  Purpose: try out FastaSeqSlurper	.
 */
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/stringUtils.hh"
#include "../FastaSeqSlurper.hh"


static std::istream*  arg_fastaIstreamPtr  =  &std::cin;
static std::string    arg_delimiter("");


namespace cbrc{


  void tryFastaSeqSlurper	(){


    /* ***** set up FastaInputOptions object ***** */
    FastaInputOptions fastaInputOptions( defaultFastaInputOptions );

    if( arg_delimiter.size() ){
      fastaInputOptions.setBetweenLabelsDelimiterRegex( boost::regex(arg_delimiter) );
    }


    /* ***** construct FastaSeqSlurper	 and set parameters ***** */
    FastaSeqSlurper fastaRecordReader( *arg_fastaIstreamPtr,
				       fastaInputOptions
				       );

    std::vector<LabeledAsciiSeq> seqs = fastaRecordReader.slurpSeqs();

    BOOST_FOREACH( const LabeledAsciiSeq& seq, seqs ){
      std::cout  <<  seq  <<  std::endl;
    }
  } // end tryFastaSeqSlurper	


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
  $0 -d \'\\t+\' exampleData/tabSeparated.fasta"
		); /* end help message. */

  argvP.set( arg_delimiter, Q(DELIMITER_FLAG) );

  // replace "\t" with tab, since tab is hard to input from command line.
  cbrc::stringUtils::stringReplaceAll( arg_delimiter,
				       std::string("\\t"),
				       std::string("\t") );

  argvP.printDoc();

  argvP.set( arg_fastaIstreamPtr, 1 );

  argvP.dieIfUnusedArgs();

  cbrc::tryFastaSeqSlurper();

  return 1;
}

