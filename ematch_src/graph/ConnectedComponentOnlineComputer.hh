/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.10
 *  Last Modified: $Date: 2008/08/25 12:20:28 $
 *  
 *  Purpose: Compute the connected components of a graph.
 *
 *  Usage: Construct with size of graph. Then add edges one at a time with addEdge() or
 *         add a whole graph with addGraph(). addEdge() has the advantage that the connected
 *         components can be computed even if all the edges don't fit in memory at the same time.
 *
 *  Output: getNodeComponents(  FLEArray<nodeIndexT>& nodeComponents );
 *          assigns nodeComponents[i] with a component number such that all nodes in the
 *          same component as i have the same component number.
 *
 *          The order of the componenent numbers is arbitrary but assigned
 *          so that the numbers 0,1,2,.. are used up to one minus the total number of
 *          connected components.
 *
 *  Requirements: space: linear in the number of vertices v. (not edges!)
 *                time:  O( vLogv ) I think. Could be made almost linear if disjoint set was
 *                more carefully implemented. In particular "path compression" is used but
 *                "union by rank" is not.
 *
 *  Reference: Uses Disjoint-set forests algorithm.
 *             For example see "Introduction to Algorithms" by C, L, & R.
 */
#ifndef _CONNECTEDCOMPONENTONLINECOMPUTER_HH_
#define _CONNECTEDCOMPONENTONLINECOMPUTER_HH_
#include "utils/graph/Graph.hh"
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

class ConnectedComponentOnlineComputer {
public:
  ConnectedComponentOnlineComputer( nodeIndexT numNodes ) : numNodes(numNodes) {
    nodeToComponent.setSize( numNodes );
    nodeToComponent.fill( unassignedComponentIndexValue() );
    compressionStack.setSize( numNodes );
    curComponent = 0;
  }

  void addNode( nodeIndexT n0 ){
    if(  isUnassignedComponentIndex( nodeToComponent[n0] )  ){
      nodeToComponent[n0] = ++curComponent;
    }
  }
  
  void addEdge( nodeIndexT n0, nodeIndexT n1 );

  void addGraph( const Graph& g );

  // return component index of component containing node N
  nodeIndexT getComponent( const nodeIndexT& n ){
    nodeIndexT tempN = n;
    return _getComponent( tempN );
  }

  nodeIndexT getNodeComponents( FLEArray<nodeIndexT>& nodeComponents ); // returns number of components.
private:
  nodeIndexT unassignedComponentIndexValue() const{
    return numNodes;
  }

  bool isUnassignedComponentIndex( const nodeIndexT& index ){
    return(  index == numNodes  );
  }

  void compressPaths();

  // returns number of components.
  nodeIndexT renumberComponents( FLEArray<nodeIndexT>& nodeComponents ); 

  nodeIndexT _getComponent( nodeIndexT& n /* modifies n */ );

  // object data.

  // if   nodeToComponent[i] = k > 0, i is the representative of cluster k,
  // else nodeToComponent[i] = -p, where p is the parent of node i
  FLEArray<int>        nodeToComponent;

  // stack used for path-compression
  FLEArray<nodeIndexT> compressionStack;
  nodeIndexT           stackTop;

  const nodeIndexT numNodes;
  nodeIndexT curComponent;
};

}; // end namespace
#endif // defined _CONNECTEDCOMPONENTONLINECOMPUTER_HH_

