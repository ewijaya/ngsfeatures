/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.10
 *  Last Modified: $Date: 2008/02/24 08:58:57 $
 *
 *  Purpose: try code involving ConnectedComponentOnlineComputer
 */
#include <iostream>
#include "../ConnectedComponentOnlineComputer.hh"

namespace cbrc{

void tryConnectedComponentOnlineComputer(){
  unsigned int n = 9;
  ConnectedComponentOnlineComputer ccoc( n );
  FLEArray<unsigned int> nodeComponents(n);
  ccoc.addEdge( 0, 8 );
  ccoc.addEdge( 1, 7 );
  ccoc.addEdge( 2, 5 );
  ccoc.addEdge( 2, 6 );
  ccoc.addEdge( 3, 5 );
  ccoc.addEdge( 3, 8 );

  std::cout << "number of components: " << ccoc.getNodeComponents( nodeComponents ) << "\n";
  std::cout << nodeComponents << std::endl;
}

} // end namespace cbrc


int main(){
  cbrc::tryConnectedComponentOnlineComputer();
  return 1;
}

