/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.3.5
 *  Last Modified: $Date: 2008/02/27 01:31:42 $
 *
 *  Description: Abstract representation of a weighted graph.
 *
 */
#ifndef _WEIGHTEDGRAPH_HH
#define _WEIGHTEDGRAPH_HH
#include <iostream>

namespace cbrc{

class WeightedGraph : public Graph{
  typedef double edgeWeightT;
public:
  virtual edgeWeightT edgeWeight( nodeIndexT n0, nodeIndexT n1 ) const = 0;
private:
};

}; // end namespace cbrc
#endif // _WEIGHTEDGRAPH_HH
