/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.10
 *  Last Modified: $Date: 2009/09/24 04:35:23 $
 *
 *  Purpose: try code involving RecountNeighborProbGraph
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "./RecountNeighborProbGraphOnDisk.hh"
#include "./graph/ConnectedComponentOnlineComputer.hh"

/* ********** PARAMETERS FROM COMMAND LINE ********** */
static std::ifstream  arg_tagSeqs_ifstream;
static std::ifstream  arg_neighborsProbGraphOnDisk_ifstream;


namespace cbrc{


void dumpNeighborList(  const TagSet&              tagID_to_seq,
			const RecountNeighborList  neighborList  ){

  std::cout  <<  tagID_to_seq.toString( neighborList.id() )  <<  " ";

  BOOST_FOREACH( const RecountTagIdProbPair& tagIdProbPair, neighborList ){
    std::cout  <<  "("  <<  tagID_to_seq.toString( tagIdProbPair.id() )
	       <<  ","  <<  tagIdProbPair.prob()  <<  ") ";

  }
  std::cout << std::endl;
}


void dumpRecountNeighborProbGraphOnDisk(){

    const TagSet  tagID_to_seq( arg_tagSeqs_ifstream );

    RecountNeighborProbGraphOnDisk
      neighborProbGraph( tagID_to_seq, arg_neighborsProbGraphOnDisk_ifstream );


    ConnectedComponentOnlineComputer ccoc(neighborProbGraph.size());

    neighborProbGraph.readFirstNode();


    do{

        const RecountNeighborList& neighborList = neighborProbGraph.curNeighborList();

        BOOST_FOREACH( const RecountTagIdProbPair& tagIdProbPair, neighborList ){

                    if (neighborList.id() >= neighborProbGraph.size() ) {
                        std::cout << "If 1\n";
                        std::cout << neighborList.id() << " " << tagIdProbPair.id() << " " << neighborProbGraph.size() <<  "\n";
                     
                    }
                    if (tagIdProbPair.id() >= neighborProbGraph.size() ) {
                        std::cout << "If 2\n";
                        std::cout << neighborList.id() << " " << tagIdProbPair.id() << " " << neighborProbGraph.size() <<  "\n";
                     
                    }
/*--------------------------------------------------
* 
*                     if (neighborList.id() == tagIdProbPair.id() ) {
*                         std::cout << "If 3\n";
*                         std::cout << neighborList.id() << " " << tagIdProbPair.id() << " " << neighborProbGraph.size() <<  "\n";
*                      
*                     }
* 
*--------------------------------------------------*/
                    if (neighborList.id() != tagIdProbPair.id()) {
                        
                        //std::cout << neighborList.id() << " " << tagIdProbPair.id() << " " << neighborProbGraph.size() <<  "\n";
                        ccoc.addEdge( neighborList.id(), tagIdProbPair.id());
                    }

        }

        
       //dumpNeighborList( tagID_to_seq, neighborProbGraph.curNeighborList() );
    } while(  neighborProbGraph.readNextNode()  );


   FLEArray<unsigned int> nodeComponents(  neighborProbGraph.size() );
    std::cout << "number of components: " << ccoc.getNodeComponents( nodeComponents ) << "\n";
    std::cout << nodeComponents << std::endl;




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

