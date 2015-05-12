/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/06/20 05:42:57 $
 *  Description: Compute and display global affine gap alignment.
 *
 *  Note: Uses full dynamic programming table for traceback and therefore requires l1 * l2 memory cells
 *        to align two sequences of length l1 and l2 respectively.
 *        It is possible to reduce that to O( Max( l1, l2 ) ) with the "Hirschberg trick" but this 
 *        implementation does not do that.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaRecordReader.hh"
#include "../SeqGlobalAffineAligner.hh"


/* ********** Global Variables ********** */

std::istream*  fastaIstreamPtr;

bool markColumns;  // if true, mark columns

// if true, mark columns containing different residues, otherwise mark the other columns
// has no effect when markColumns is false.
bool markDifferent;

bool outputHtml;

namespace cbrc{

  void printIfHtml( const std::string&  message ){
    if( outputHtml )  std::cout  <<  message;
  }

  void runGlobalAffineAlignment(){
    FastaRecordReader fr;
    AminoScorePam120 aScore;

    const FastaRecord* recordPtr = fr.nextRecordOrDie( *fastaIstreamPtr );

    ResidueIndexMap::arrayT s1(  aminoResidueIndexMap.toResidueIndicesVector( fr.seq() )  );

    SeqGlobalAffineAligner gas( aScore, s1 );

    recordPtr = fr.nextRecordOrDie( *fastaIstreamPtr );

    ResidueIndexMap::arrayT s2(  aminoResidueIndexMap.toResidueIndicesVector( fr.seq() )  );

    Alignment al = gas.alignment( s2 );

    printIfHtml( "<HTML><BODY>\n" );
    std::cout << "alignment score: " << al.score( aScore ) << std::endl;
    printIfHtml( "<BR>\n" );

    if( outputHtml ){
          if( markColumns )  al.outputHtmlMarkSame( std::cout, markDifferent );
	  else               al.outputHtml        ( std::cout );
    }
    else{ // output plain text
      if( markColumns )  al.outputTextMarkSame( std::cout, markDifferent );
      else               std::cout << al;
    }
    printIfHtml( "</BODY></HTML>\n" );
  }
} // end namescape cbrc



#define OUTPUT_HTML_FLAG    -h|--output-html
#define MARK_COLUMNS        -m|--mark-columns
#define MARK_COLUMN_STYLES  same|different

int main( int argc, const char** argv ){
  cbrc::ArgvParser args( argc, argv, "[-h] [-m same|different], [fastaFile]" );

  const std::string man(
			"\
NAME\n\
    $0\n\
SYNOPSIS\n\
        Scores alignment two sequences\n\
\n\
OPTIONS\n\
    "Q(OUTPUT_HTML_FLAG)"\n\
        Output in html format. Otherwise plain text is output.\n\
\n\
    "Q(MARK_COLUMNS)" "Q(MARK_COLUMN_STYLES)"\n\
        Mark columns whose residie is the SAME or DIFFERENT.\n\
\n\
    fastaFile\n\
        fasta format input sequence file. Defaults to std::cin if omitted\n"
			);

  args.setDoc( "-man|--man|-help|--help", man );

  args.printDoc();

  outputHtml  =  args.hasFlag( Q(OUTPUT_HTML_FLAG) );

  const std::string
    markColumnsArg(  args.getEnumOpt(  Q(MARK_COLUMNS), Q(MARK_COLUMN_STYLES)  )   );

  markColumns  =  markColumnsArg.size();

  markDifferent  =  ( markColumnsArg == "different" );

  fastaIstreamPtr  =  args.getIstreamPtr( 1 );

  args.dieIfUnusedArgs();

  cbrc::runGlobalAffineAlignment();

  return 1;
}


