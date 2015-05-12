/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2008/08/25 12:22:18 $
 *  
 *  Description: See header files.
 */

#include "utils/graph/ConnectedComponentOnlineComputer.hh"

namespace cbrc{



nodeIndexT ConnectedComponentOnlineComputer::_getComponent( nodeIndexT& n /* modifies n */ ){
  stackTop = 0;

  while( nodeToComponent[n] <= 0 ){
    compressionStack[stackTop++] = n;
    n = -nodeToComponent[n];
  }
  nodeIndexT answer = nodeToComponent[n];

  // do path compression.
  for( nodeIndexT i = 0; i < stackTop; ++i ){
    nodeToComponent[ compressionStack[i] ] = -n;
  }

  return answer;
}


void ConnectedComponentOnlineComputer::addEdge( nodeIndexT n0, nodeIndexT n1 ){
  
  unsigned int c0 = _getComponent( n0 /* modifies n0 */ );
  unsigned int c1 = _getComponent( n1 /* modifies n1 */ );

  if( c0 == c1 ){
    if( c0 == unassignedComponentIndexValue() ){
      nodeToComponent[n0] = ++curComponent;
      nodeToComponent[n1] = -n0;
    }
    return;
  }
  
  if( c0 < c1 ){
    nodeToComponent[n1] = -n0;
  }else{
    nodeToComponent[n0] = -n1;
  }

}


unsigned int
ConnectedComponentOnlineComputer::getNodeComponents( FLEArray<unsigned int>& nodeComponents ){
  // std::cout << "at getNodeComponents, nodeComponents: " << nodeComponents << std::endl;
  assert( nodeComponents.size() == nodeToComponent.size() );
  compressPaths();
  unsigned int maxComponent = 0;

  for( unsigned int i = 0; i < numNodes; ++i ){
    if(  isUnassignedComponentIndex( nodeToComponent[i] )  ){
      continue;
    }

    if( nodeToComponent[i] <= 0 ){
      // subtract 1 to number components from 0.
      nodeComponents[i] = nodeToComponent[ -nodeToComponent[i] ] - 1; 
    }

    if( nodeToComponent[i] > 0 ){
      nodeComponents[i] = nodeToComponent[i] - 1;  // subtract 1 to number components from 0.
      if( maxComponent < nodeComponents[i] ) maxComponent = nodeComponents[i];
    }

  }

  for( unsigned int i = 0; i < numNodes; ++i ){
    if(  isUnassignedComponentIndex( nodeToComponent[i] )  ){
      nodeComponents[i] = ++maxComponent;
    }
  }

  // at this point the list is correct but some integers are skipped,
  // so renumber the components to 0, 1, 2, ...
  return renumberComponents( nodeComponents );
}


unsigned int
ConnectedComponentOnlineComputer::renumberComponents( FLEArray<unsigned int>& nodeComponents ){

  unsigned int componentCount = 0;

  FLEArray<unsigned int> toUniqueComponentNumber( numNodes, unassignedComponentIndexValue() );

  for( unsigned int i = 0; i < numNodes; ++i ){
    if(  toUniqueComponentNumber[ nodeComponents[i] ] == unassignedComponentIndexValue() ){
      toUniqueComponentNumber[ nodeComponents[i] ] = componentCount++;
    }
    nodeComponents[i] = toUniqueComponentNumber[ nodeComponents[i] ];
  }

  return componentCount;
}  

  

void ConnectedComponentOnlineComputer::compressPaths(){
  for( unsigned int i = 0; i < numNodes; ++i ){
    unsigned int n = i;
    stackTop = 0;
    while( nodeToComponent[n] <= 0 ){
      compressionStack[stackTop++] = n;
      n = -nodeToComponent[n];
    }
    for( unsigned int j = 0; j < stackTop; ++j ){
      nodeToComponent[ compressionStack[j] ] = -n;
    }
  }
}



void ConnectedComponentOnlineComputer::addGraph( const Graph& g ){

  if( g.numNodes() != numNodes ){
    std::cerr << "** in addGraph: expected graph with: " << numNodes << " but got graph with: " << g.numNodes()
	      << std::endl;
    assert( false );
  }

  for( nodeIndexT i = 1; i < numNodes; ++i ){
    for( nodeIndexT j = 0; j < i; ++j ){
      if( g.hasEdge( i, j ) ) addEdge( i, j );
    }
  }
}



}; // end namespace
