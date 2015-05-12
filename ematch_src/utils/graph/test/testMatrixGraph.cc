/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.11
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test MatrixGraph.
 */
#include <iostream>
#include "../MatrixGraph.hh"

void cbrc::testMatrixGraph(){
  //  nodeIndex n = 5;
  //  MatrixGraph mg( n );
//    mg.setEdge( 1, 3 );
//    mg.setEdge( 0, 4 );
//    mg.setEdge( 3, 2 );
  MatrixGraph mg( std::cin );
  
  mg.printAdjacencyLists( std::cout );
  //  std::cout << mg;
}

int main(){
  cbrc::testMatrixGraph();
  return 1;
}

