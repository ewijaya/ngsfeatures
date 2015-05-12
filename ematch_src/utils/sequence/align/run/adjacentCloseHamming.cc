/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.2
 *  Last Modified: $Date: 2009/06/09 02:18:24 $
 *
 *  Description: 
 *
 *  Purpose: Created for
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/BoundedLenFifo/BoundedLenFifoVector.hh"
#include "utils/sequence/LabeledAsciiSeq.hh"
#include "utils/sequence/readers/fasta/FastaRecordReader.hh"
#include "../HammingDistanceComputer.hh"
#define  DIRECTION_ENUM       flush-left|flush-right
#define  NUM_NEIGHBORS_FLAG  -n|--num-neighbors
#define  PERCENT_FLAG        -p|--percent
#define  USAGE \
" [-p]  -n numNeighbors  threshold  ("Q(DIRECTION_ENUM)")  [multifastaFile]"



std::istream* argFastaInputStreamPtr;
double        argThreshold;
bool          argByPercent;
bool          argFlushLeft;
size_t        argNumNeighbors;



namespace cbrc{

  void printCloseNeighbors( const BoundedLenFifoVector<LabeledAsciiSeq>& neighbors ){

    const LabeledAsciiSeq&  oldest  =  neighbors.oldestElem( 0 );
    
    for( size_t i = 1;  i < neighbors.size();  ++i  ){

      const LabeledAsciiSeq&  other  =  neighbors.oldestElem( i );

      double hammingDistanceThreshold;

      if( argByPercent ){
	const size_t shorterLen  =  std::min( oldest.seq().size(), other.seq().size() ); 

	hammingDistanceThreshold = argThreshold * shorterLen / 100;
      }else{
	hammingDistanceThreshold = argThreshold;
      }

      const size_t  distance  =
	(  argFlushLeft ?
	 HammingDistanceComputer
	   ::computeBoundedGapAtEnd( oldest.seq(), other.seq(), hammingDistanceThreshold )
	   : // else
	   HammingDistanceComputer
	   ::computeBoundedGapAtBeg( oldest.seq(), other.seq(), hammingDistanceThreshold )
	   );

      /* ** PROGRAM OUTPUT ** */
      if( distance < hammingDistanceThreshold ){
	std::cout  <<  oldest.name()  <<  "\t"  <<  other.name()  <<  std::endl;
      }

    } // end for neighbor loop

  } // end function printCloseNeighbors


  void adjacentCloseHamming(){

    /* ***** initiate local vars ***** */
    BoundedLenFifoVector<LabeledAsciiSeq>  curSeqs(  argNumNeighbors + 1  );

    FastaRecordReader fastaReader( *argFastaInputStreamPtr );

    const FastaRecord*  recordPtr;


    /* ***** fill up curSeqs ***** */
    while(  curSeqs.size() < curSeqs.capacity()
	    && (recordPtr = fastaReader.nextRecord())  ){
      curSeqs.push(  LabeledAsciiSeq( recordPtr->seq(), recordPtr->name() )  );
    }

    // process first line
    printCloseNeighbors( curSeqs );

    // process from 2nd line, up to number of lines in stream - curLines.capacity()
    while((  recordPtr = fastaReader.nextRecord()  )){
      curSeqs.push(  LabeledAsciiSeq( recordPtr->seq(), recordPtr->name() )  );
      printCloseNeighbors( curSeqs );
    }

    // process remaining lines
    for(  curSeqs.popOldest();
	  curSeqs.size();
	  curSeqs.popOldest()  ){
      printCloseNeighbors( curSeqs );
    }

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){

  cbrc::ArgvParser args( argc, argv, USAGE );

  /* ***** Documentation ***** */
  std::string man( "\
NAME\n\
    $0\n\
SYNOPSIS\n\
    "USAGE"\n\
\n\
\n\
DESCRIPTION\n\
    Print adjacent sequence pairs from multifastaFile whose Hamming distance\n\
    is less than THRESHOLD\n\
\n\
\n\
ARGUMENTS\n\
\n\
     "Q(NUM_NEIGHBORS_FLAG)" numNeighbors\n\
\n\
     Number of neighbors to compare to each sequence. set numNeighbors to one to\n\
     compare only immediately adjacent sequences. set numNeighbors to ∞ to compare\n\
     all sequence pairs\n\
\n\
     "Q(DIRECTION_ENUM)"\n\
\n\
     Mandatory argument which controls where gaps are placed when\n\
     one sequence is shorter than the other. If one \"flush-left\" is chosen\n\
     the gaps are placed at the end, while if \"flush-right\" is chosen, the\n\
     gaps are placed at the beginning of the alignment.\n\
\n\
\n\
OPTIONS\n\
\n\
    "Q(PERCENT_FLAGS)"\n\
\n\
        For each sequence pair comparison, interpreset THRESHOLD as percentage of\n\
        $l, the length of the shorter sequence. Output pair if the Hamming distance\n\
        is less than THRESHOLD * $l\n\
" ); // end manual

  args.setDoc(  "-help|--help|-man|--man", man );

  args.printDoc();


  /* ***** Argument Parsing ***** */
  argByPercent  =  args.hasFlag( Q(PERCENT_FLAG) );

  args.setOrDie( argNumNeighbors, Q(NUM_NEIGHBORS_FLAG) );

  argNumNeighbors || args.die( "argNumNeighbors must be positive integer" );

  size_t curArg = 0;


  args.setOrDie( argThreshold,++curArg );


  const std::string  flushDirection(  args.getEnumOrDie( ++curArg, Q(DIRECTION_ENUM) )  );

  argFlushLeft  =  ( flushDirection == "flush-left" );


  argFastaInputStreamPtr = args.getIstreamPtr( ++curArg );


  args.dieIfUnusedArgs();

  cbrc::adjacentCloseHamming();

  return 1;
}

