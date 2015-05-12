/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.23
 *  Last Modified: $Date: 2009/10/07 10:26:04 $
 *
 *  Purpose: try code involving RecountExpectationMatchingTagCorrector
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "./RecountExpectationMatchingTagCorrector.hh"
#define  USAGE                  [OPTIONS] tagSeqsFile tagNeighborProbGraphFile tagCountsFile
#define  ROUNDS_TO_WAIT_FLAG    -r|--rounds-to-wait


/* --------------- PARAMETERS FROM COMMAND LINE --------------- */
static std::ifstream  arg_tagSeqsFile;
static std::ifstream  arg_tagNeighborProbGraphFile;
static std::ifstream  arg_tagCountsFile;
size_t                arg_roundsToWait;


namespace cbrc{

  void runRecountExpectationMatchingTagCorrector(){

    const TagSet  tagID_to_seq( arg_tagSeqsFile );

    RecountNeighborProbGraphOnDisk neighborGraph( tagID_to_seq, arg_tagNeighborProbGraphFile );

    RecountComputerForGraphOnDisk recountComputer( neighborGraph );

    RecountTagCounts observedCounts( neighborGraph.tagID_to_seq(), arg_tagCountsFile );

    assert(   observedCounts.min()  >=  1.0   );


    RecountExpectationMatchingTagCorrector
      tagCorrector( recountComputer, observedCounts, arg_roundsToWait );

    tagCorrector.inferTrueTagCounts();

    //std::cout  <<  "Converged after: "  <<  tagCorrector.bestEstCountsIteration()
	//       <<  " iterations\n"
	//       <<  "# Corrected counts:\n";
    tagCorrector.bestEstCounts().print( tagID_to_seq );
							
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, Q(USAGE) );

    argvP.setDoc( "-h|-help|--help",
		"\
$0 "Q(USAGE)"\n\
\n\
\n\
tagSeqsFile\n\
    text file holding unique header line then tag sequences one per line\n\
    header line holds 2 integers:  number_of_sequences and total_sequence_length\n\
\n\
tagNeighborProbGraphFile\n\
    binary file holding information on the neighbor tags which may be\n\
    mistakenly generated for each tag, and the probability of such error.\n\
\n\
tagCountsFile\n\
   text file consisting of lines, each line holding a tag sequence and its count\n\
\n\
\n\
OPTIONS\n\
\n\
    "Q(ROUNDS_TO_WAIT_FLAG)"\n\
        Number of iterations to wait for improvement before terminating.\n\
\n\
"
		);  /* end setDoc help */

  argvP.printDoc();

  /* ----- Default values ----- */
  arg_roundsToWait  =  10;

  argvP.set( arg_roundsToWait, Q(ROUNDS_TO_WAIT_FLAG) );

  argvP.setOrDie( arg_tagSeqsFile             , 1 );
  argvP.setOrDie( arg_tagNeighborProbGraphFile, 2 );
  argvP.setOrDie( arg_tagCountsFile           , 3 );
  
  argvP.dieIfUnusedArgs();
  cbrc::runRecountExpectationMatchingTagCorrector();
  return 1;
}

