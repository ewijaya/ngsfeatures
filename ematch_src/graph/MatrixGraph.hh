/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2009/04/26 05:34:12 $
 *
 *  Description: Matrix representation of graph
 *
 */
#ifndef _MATRIXGRAPH_HH_
#define _MATRIXGRAPH_HH_

#include "utils/graph/Graph.hh"
#include "utils/BitMatrix/SymmetricBitMatrix.hh"
#include "utils/perlish/perlish.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings.hh"

namespace cbrc{

class MatrixGraph : public Graph {
public:
  /* ***** Constructors ***** */
  MatrixGraph( nodeIndexT numNodes ) : _numNodes(numNodes){
    edges.setSize( _numNodes );
    edges.zero();
  }

  MatrixGraph( std::istream& is ){
    readContinuousIntegerNodesFormat( is );
  }

  virtual ~MatrixGraph(){}


  /* ***** accessors ***** */
  nodeIndexT numNodes() const{ return _numNodes; }

  // return name of node with serial number nodeIndexT
  std::string nodeName( const nodeIndexT& nodeIndex ) const{
    return  labels(nodeIndex );
  }

  bool hasEdge( const nodeIndexT& n0, const nodeIndexT& n1 ) const{ return edges( n0, n1 ); }

  void setEdge( nodeIndexT n0, nodeIndexT n1 ){ edges.set( n0, n1 ); }
  void setEdgesFromAdjacencyListStream( std::istream& is );

  /* ***** I/O methods ***** */
  void readContinuousIntegerNodesFormat( std::istream& is );
  void readEdgesOneEdgePerLineFormat( std::istream& is );

  void printAdjacencyLists( std::ostream& os );
private:
  static const boost::regex& inputDelimiter(){
    static boost::regex delim( "\t+" );
    return delim;
  }

  SerialNumbersForStrings<nodeIndexT> labels;
  nodeIndexT _numNodes;
  SymmetricBitMatrix edges;
};


std::ostream& operator<<( std::ostream& os, const MatrixGraph& mg );


}; // end namespace
#endif // defined _MATRIXGRAPH_HH_
