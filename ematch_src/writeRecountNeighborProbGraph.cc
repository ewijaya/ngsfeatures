/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/09/20 11:44:33 $
 *
 *  Input:  sequence tags with their neighbors and probability of error
 *  Output: RecountNeighborProbGraphWriter object in binary
 *          format
 *
 *  Purpose: Created for RECOUNT project
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "./RecountNeighborProbGraphWriter.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */

static std::istream*   arg_tagSeqsIstreamPtr       =  NULL;
static std::istream*   arg_tagNeighborsIstreamPtr  =  NULL;
static std::ofstream   arg_outfile;

namespace cbrc{

  void writeRecountNeighborProbGraph(){

    RecountNeighborProbGraphWriter graphWriter( *arg_tagSeqsIstreamPtr );

    graphWriter.write( arg_outfile, *arg_tagNeighborsIstreamPtr );
    
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "tagSeqsFile tagNeighborsFile outputFile" );

  argvP.setDoc( "-h|--help|--man",
		"\
tagSeqsFile tagNeighborsFile\n\
\n\
ARGUMENTS\n\
\n\
tagSeqsFile\n\
    File holdings tag sequences, one per line\n\
\n\
neighborListFile\n\
    File holding neighboring tags and probabilities for each tag,\n\
    each line representing the neighborhood of a single tag sequence\n\
\n\
outputFile\n\
    File to output binary stream to"
		);

  argvP.printDoc();

  size_t curArg = 0;

  argvP.setOrDie( arg_tagSeqsIstreamPtr,      ++curArg );

  argvP.setOrDie( arg_tagNeighborsIstreamPtr, ++curArg );

  argvP.setCautiouslyOrDie( arg_outfile,      ++curArg );
  
  argvP.dieIfUnusedArgs();

  cbrc::writeRecountNeighborProbGraph();

  return 1;
}

