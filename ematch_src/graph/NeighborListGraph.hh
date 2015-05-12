/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.30
 *  Last Modified: $Date: 2009/04/26 05:28:19 $
 *  
 *  Description: Representation of a undirected, unweighted graph. Uses neighbor list
 *               representation. hasEdge( n0, n1 ) time complexity is log in min( degree(n0), degree(n1) ).
 *               Space complexity is O(V+E).
 *
 */

#ifndef _NEIGHBORLISTGRAPH_HH_
#define _NEIGHBORLISTGRAPH_HH_

#include "utils/graph/Graph.hh"
#include "utils/perlish/perlish.hh"
#include "utils/FLArray/FLEArray.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings.hh"

namespace cbrc{

class NeighborListGraph : public Graph{
public:
  void readOneEdgePerLineFormat( std::istream& );
  virtual ~NeighborListGraph(){}
  nodeIndexT numNodes() const{ return ne.size(); }

  bool hasEdge( const nodeIndexT& n0, const nodeIndexT& n1 ) const;

  const std::vector<nodeIndexT>& neighborList( nodeIndexT n ) const{ return ne[n]; }

  FLENumArray<nodeIndexT> shortestPathByEdgeLength( const nodeIndexT& n ) const;

  std::string nodeIndexTLabel( nodeIndexT n ) const{ return labels(n); }

  void checkConsistency() const;

  std::string label( nodeIndexT n ) const{ return labels(n); } // mostly for debugging.

  friend std::ostream& operator<<( std::ostream& os, const NeighborListGraph& nlg );
private:
  SerialNumbersForStrings<nodeIndexT> labels;
  std::vector< std::vector<nodeIndexT> > ne; // neighbor list.
};


}; // end namespace
#endif // defined _NEIGHBORLISTGRAPH_HH_

