/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2009/04/26 05:39:36 $
 *
 *  Description: simple test of ConnectedComponentOnlineComputer.
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ConnectedComponentOnlineComputer.hh"
#include "../MatrixGraph.hh"

namespace cbrc{

void testConnectedComponentOnlineComputer( const bool& verify ){
  MatrixGraph mg( std::cin );
  std::cout << "num nodes is " << mg.numNodes() << std::endl;
  ConnectedComponentOnlineComputer ccoc( mg.numNodes() );

  if( verify ){
    // Very primitive v^4 algorithm for transitive closure follows:
    bool done = false;
    while( !done ){
      done = true;
      for( nodeIndexT k = 2; k < mg.numNodes(); ++k ){
	for( nodeIndexT j = 1; j < k; ++j ){
	  if( !mg.hasEdge( j, k ) ) continue;
	  for( nodeIndexT i = 0; i < j; ++i ){
	    if( mg.hasEdge(i, j) ){
	      if( mg.hasEdge(j, k) ){
		if( !mg.hasEdge(i, k) ){
		  done = false;
		  mg.setEdge( i, k );
		}
	      }
	    }
	  }
	}
      }
    }
  }
	  
  ccoc.addGraph( mg );
  FLEArray<nodeIndexT> nodeComponents( mg.numNodes() );

  unsigned int numComponents = ccoc.getNodeComponents( nodeComponents );
  std::cout << "number of components: " << numComponents << "\n";
  //  std::cout << "components: " << nodeComponents << std::endl;

  if( verify ){
    // Check correctness. This only works if the transitive closure has been taken.
    std::cout << "verifying...\n";
    for( nodeIndexT j = 1; j < mg.numNodes(); ++j ){
      for( nodeIndexT i = 0; i < j; ++i ){
	if( mg.hasEdge(i, j) ){
	  if( nodeComponents[i] != nodeComponents[j] ){
	    std::cout << "node: " << i << " component: " << nodeComponents[i] << " != " 
		      << "node: " << j << " component: " << nodeComponents[j] << std::endl;
	  }
	}
      }
    }
  }

  std::cout << "component: followed by node list\n";
  // This part is O(v^2) time. May need to be rewritten.
  for( unsigned int i = 0; i < numComponents; ++i ){
    std::cout << i << ":";
    for( unsigned int j = 0; j < nodeComponents.size(); ++j ){
      if( nodeComponents[j] == i ){
	std::cout << " " << j;
      }
    }
    std::cout << std::endl;
  }
  
      
}

}; // end namespace cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage: $0 [--verify]\n\
\n\
pipe MatrixGraph format input file from stdin\n"
			 );

  bool verify = args.hasFlag( "--verify" );
  args.dieIfUnusedArgs();
  cbrc::testConnectedComponentOnlineComputer( verify );
  return 1;
}
