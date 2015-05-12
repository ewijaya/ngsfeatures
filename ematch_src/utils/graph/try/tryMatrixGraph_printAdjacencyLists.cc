/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2008/02/27 01:43:32 $
 *
 *  Purpose: try code involving MatrixGraph_printAdjacencyLists
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../MatrixGraph.hh"


namespace cbrc{

void tryMatrixGraph_printAdjacencyLists(){
  //  nodeIndex n = 5;
  //  MatrixGraph mg( n );
//    mg.setEdge( 1, 3 );
//    mg.setEdge( 0, 4 );
//    mg.setEdge( 3, 2 );
  MatrixGraph mg( std::cin );
  
  mg.printAdjacencyLists( std::cout );
  //  std::cout << mg;
}

} // end namespace cbrc



int main( int argc, const char* argv[] ){

  cbrc::ArgvParser args( argc, argv, "No args. Pipe graph from stardard in" );

  args.dieIfUnusedArgs();

  cbrc::tryMatrixGraph_printAdjacencyLists();
  return 1;
}

