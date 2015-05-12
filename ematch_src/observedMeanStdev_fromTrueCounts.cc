/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.12
 *  Last Modified: $Date: 2009/09/28 02:44:54 $
 *
 *  Input:   Sequence tags counts and tag neighbors
 *  Output:  mean and variance of expected observed counts,
 *           if the input counts were true counts.
 *
 *  Purpose: Created for RECOUNT project.
 *
 */
#include <iostream>
#include <cmath>
#include "utils/argvParsing/ArgvParser.hh"
#include "../RecountComputerForGraphOnDisk.hh"
#include "../RecountTagCounts.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
static std::ifstream  arg_tagSeqsFile;
static std::ifstream  arg_tagNeighborProbGraphFile;
static std::ifstream  arg_tagCountsFile;



namespace cbrc{


  void observedMeanStdev_fromTrueCounts(){


    /* ********** Construct Datastructures ********** */

    const TagSet  tagId_to_seq( arg_tagSeqsFile );

    RecountNeighborProbGraphOnDisk
      neighborGraph( tagId_to_seq, arg_tagNeighborProbGraphFile );

    RecountComputerForGraphOnDisk recountComputer( neighborGraph );

    RecountTagCounts trueTagCounts( neighborGraph.tagID_to_seq(), arg_tagCountsFile );

    RecountTagCounts expectedCounts( trueTagCounts.size() );

    RecountTagCounts variances( trueTagCounts.size() );


    /* ********** Compute Statistics ********** */

    recountComputer.meanCountsFromTrue    ( expectedCounts, trueTagCounts );

    recountComputer.varianceCountsFromTrue( variances, trueTagCounts );


    /* --------------- Output Results --------------- */
    std::cout  <<    "sum of real counts: "        <<  trueTagCounts.sum()
	       <<  "  sum of expected counts: "    <<  expectedCounts.sum()
	       <<  "\n--------------------\n";

    for(  size_t i = 0;  i < trueTagCounts.size();  ++i  ){
      std::cout  <<  tagId_to_seq.toString(i)
		 <<  "\t"  <<  expectedCounts(i)
		 <<  "\t"  <<  sqrt( variances(i) )
		 <<  std::endl;
    }
    
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){

  cbrc::ArgvParser argvP( argc, argv, "tagSeqsFile tagNeighborProbGraphFile tagCountsFile" );

  argvP.setDoc( "-h|-help|--help",
		"\
$0 tagSeqsFile tagNeighborProbFile tagCountsFile\n\
\n\
\n\
tagSeqsFile\n\
    text file holding unique tag sequences, one per line\n\
\n\
tagNeighborProbGraphFile\n\
    binary file holding information on the neighbor tags which may be\n\
    mistakenly generated for each tag, and the probability of such error.\n\
"
		);  /* end setDoc help */

  argvP.printDoc();



  argvP.setOrDie( arg_tagSeqsFile             , 1 );
  argvP.setOrDie( arg_tagNeighborProbGraphFile, 2 );
  argvP.setOrDie( arg_tagCountsFile           , 3 );

  
  argvP.dieIfUnusedArgs();

  cbrc::observedMeanStdev_fromTrueCounts();
  return 1;
}

