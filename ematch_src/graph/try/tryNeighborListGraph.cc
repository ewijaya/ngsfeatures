/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.30
 *  Last Modified: $Date: 2008/02/24 03:40:21 $
 *
 *  Purpose: try code involving NeighborListGraph
 */
#include <iostream>
#include "../NeighborListGraph.hh"

namespace cbrc{
  void tryNeighborListGraph(){
    std::string line = perlish::slurpLine( std::cin ); // skip num of nodes line.
    NeighborListGraph nlg;
    nlg.readOneEdgePerLineFormat( std::cin );
    std::cout << nlg;
  }
}; // end namescape cbrc

int main(){
  cbrc::tryNeighborListGraph();
  return 1;
}

