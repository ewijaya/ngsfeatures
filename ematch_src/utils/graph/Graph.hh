/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.9
 *  Last Modified: $Date: 2009/04/26 05:35:39 $
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: Abstract representation of a graph.
 */
#ifndef _GRAPH_HH_
#define _GRAPH_HH_
#include <iostream>
#include <vector>
#include "utils/graph/graphTypes.hh"

namespace cbrc{
  
class Graph {
public:
  virtual nodeIndexT numNodes() const = 0;
  virtual bool hasEdge( const nodeIndexT& n0, const nodeIndexT& n1 ) const = 0;
  virtual ~Graph() {}
private:
};


}; // end namespace
#endif // defined _GRAPH_HH_

