/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.11
 *  Last Modified: $Date: 2009/04/26 05:34:27 $
 *  Description: See header file.
 */
#include <string>
#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "MatrixGraph.hh"

namespace cbrc{


void MatrixGraph::readContinuousIntegerNodesFormat( std::istream& is ){

  std::string line;
  std::vector<std::string> fields;
  nodeIndexT startIndex = 0; // init to silence compiler warning.


  /* ***** read num nodes declaration line ***** */
  std::string numNodeDeclarationLine = perlish::slurpLine( is );
  assert( numNodeDeclarationLine.size() );

  fields = perlish::split( inputDelimiter(), numNodeDeclarationLine );

  assert( fields.size() == 2 );
  startIndex = atoi( fields[0].c_str() );
  _numNodes = atoi( fields[1].c_str() ) - startIndex + 1;

  // add node names as strings
  for( size_t i = 0; i < _numNodes; ++i ){
    std::string nodeName(   boost::lexical_cast<std::string>( startIndex + i )   );
    labels.registerNewSerialNumber( nodeName );
  }
    
  edges.setSize( _numNodes );
  edges.zero();

  
  /* ***** read in edges ***** */
  while( !is.eof() ){
    std::string curLine = perlish::slurpLine( is );

    fields = perlish::split( inputDelimiter(), curLine );

    if( fields.size() < 2 ){
      std::cerr << "skipping line: " << curLine << std::endl;
      continue;
    }

    nodeIndexT n0 = atoi( fields[0].c_str() ) - startIndex;
    for( size_t i = 1; i < fields.size(); ++i ){
      nodeIndexT n1 = atoi( fields[i].c_str() ) - startIndex;
      setEdge( n0, n1 );
    }
  }
}



void MatrixGraph::readEdgesOneEdgePerLineFormat( std::istream& is ){
  while( !is.eof() ){
    std::string line = perlish::slurpLine( is );
    if( line.size() == 0 ) break;

    std::vector<std::string> fields = perlish::split( inputDelimiter(), line );

    if( !fields.size() == 2 ){
      std::cout << "Format error. Expected two fields when reading line: " << line << std::endl;
      exit( -1 );
    }
    if( fields[0] == fields[1] ){
      std::cerr << "Warning. Skipping self edge from line: " << line << std::endl;
      continue;
    }
    nodeIndexT n0 = labels.getSerialNumber( fields[0] );
    nodeIndexT n1 = labels.getSerialNumber( fields[1] );
    setEdge( n0, n1 );
  }
}
    

void MatrixGraph::printAdjacencyLists( std::ostream& os ){
  
  std::vector<nodeIndexT> neighbors;

  for( nodeIndexT i = 0; i < numNodes(); ++i ){
    neighbors.clear();

    for( nodeIndexT j = i+1; j < numNodes(); ++j ){
      if( hasEdge( i, j ) ) neighbors.push_back( j );
    }

    if( neighbors.size() > 0 ){
      os << labels(i);
      for( nodeIndexT j = 0; j < neighbors.size(); ++j ){
	os << " " << labels( neighbors[j] );
      }
      os << std::endl;
    }

  }
}


std::ostream& operator<<( std::ostream& os, const MatrixGraph& mg ){
  os << mg.numNodes() << std::endl;
  for( nodeIndexT i = 0; i < mg.numNodes(); ++i ){
    for( nodeIndexT j = i+1; j < mg.numNodes(); ++j ){
      if( mg.hasEdge( i, j ) ){
	os << mg.nodeName(i) << " " << mg.nodeName(j) << std::endl;
      }
    }
  }
  return os;
}

}; // end namespace
