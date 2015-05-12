/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.10
 *  Last Modified: $Date: 2009/09/24 04:26:23 $
 *
 *  Purpose: try code involving RecountNeighborProbGraph
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../RecountNeighborProbGraphOnDisk_SortedTagSet.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
static std::ifstream  arg_tagSeqs_ifstream;
static std::ifstream  arg_neighborsProbGraphOnDisk_ifstream;


namespace cbrc{


void dumpNeighborList(  const SortedTagSet&     tagId_to_seq,
			const RecountNeighborList  neighborList  ){

  const SortedTagSet::tagSeqT  tagSeq  =  tagId_to_seq( neighborList.id() );
  std::cout  <<  tagId_to_seq.toString( tagSeq )  <<  " ";

  BOOST_FOREACH( const RecountTagIdProbPair& tagIdProbPair, neighborList ){
    const SortedTagSet::tagSeqT  neighborTagSeq  =  tagId_to_seq( tagIdProbPair.id() );
    std::cout  <<  "("  <<  tagId_to_seq.toString( neighborTagSeq )
	       <<  ","  <<  tagIdProbPair.prob()  <<  ") ";
  }
  std::cout << std::endl;
}


void dumpRecountNeighborProbGraphOnDisk(){

    const SortedTagSet tagId_to_seq( arg_tagSeqs_ifstream );

    RecountNeighborProbGraphOnDisk
      neighborProbGraph( tagId_to_seq, arg_neighborsProbGraphOnDisk_ifstream );

    neighborProbGraph.readFirstNode();
    do{
      dumpNeighborList( tagId_to_seq, neighborProbGraph.curNeighborList() );
    } while(  neighborProbGraph.readNextNode()  );


} // end dumpRecountNeighborProbGraphOnDisk


} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "tagSeqsFile neighborsProbGraphOnDiskFile" );
  
  argvP.setOrDie( arg_tagSeqs_ifstream, 1 );

  argvP.setOrDie( arg_neighborsProbGraphOnDisk_ifstream, 2 );

  argvP.dieIfUnusedArgs();

  cbrc::dumpRecountNeighborProbGraphOnDisk();

  return 1;

}

