/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2009/04/26 05:27:26 $
 *
 *  Purpose: try code involving MatrixGraph_readEdgesOneEdgePerLineFormat
 */
#include <iostream>
#include <stdlib.h>
#include "../MatrixGraph.hh"

namespace cbrc{

void tryReadOneEdgePerLineFormat(){

  const std::string line = perlish::slurpLine( std::cin );
  const nodeIndexT numNodes = atoi( line.c_str() );

  MatrixGraph mg( numNodes );

  mg.readEdgesOneEdgePerLineFormat( std::cin );
  std::cout << mg;
}

}; // end namespace cbrc

int main(){
  cbrc::tryReadOneEdgePerLineFormat();
  return 1;
}

