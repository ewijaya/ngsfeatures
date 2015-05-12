/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.10
 *  Copyright: All rights reserved
 *  
 *  Purpose: List connected components of a graph.
 *
 */
#include <fstream>
#include <iostream>
#include "utils/graph/OneEdgePerLineFormat.hh"
#include "utils/perlish/perlish.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings.hh"
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "../ConnectedComponentOnlineComputer.hh"

namespace cbrc{

void listConnectedComponents( std::string filename ){
  std::ifstream is;
  is.open( filename.c_str() );
  nodeIndexT numNodes = OneEdgePerLineFormat::numNodesInStream( is ); // first count nodes.
  is.clear(); is.seekg( 0, std::ios::beg ); // reset the filestream to be reread from the beginning.
  
  ConnectedComponentOnlineComputer ccoc( numNodes );
  SerialNumbersForStrings<nodeIndexT> labels;

  // Read in graph and add edges.
 INPUT_LINE:
  while( !is.eof() ){
    std::string line = perlish::slurpLine( is );
    if( line.size() == 0 ) break;

    std::vector<std::string> fields
      = perlish::split( OneEdgePerLineFormat::inputDelimiter(), line );

    if( !fields.size() || (fields.size() > 2) ){
      std::cout << "Error: expected 1 or 2 fields per line: " << line << std::endl;
      exit( -1 );
    }

    if( fields.size() == 1 ){
      nodeIndexT n0 = labels.getSerialNumber( fields[0] );
      ccoc.addNode( n0 );
      goto INPUT_LINE;
    }

    nodeIndexT n0 = labels.getSerialNumber( fields[0] );
    nodeIndexT n1 = labels.getSerialNumber( fields[1] );
    if( fields[0] == fields[1] ){
      // In order for labels to have the same size as numNodes it is important that this
      // if statement comes *after* the node name is added to labels. The case in which this is important
      // is a singleton node with a self-edge.
      std::cerr << "Warning. Skipping self edge from line: " << line << std::endl;
      goto INPUT_LINE;
    }

    ccoc.addEdge( n0, n1 );
  }

  // create permutation array which indirectly sorts the elements of nodeComponents
  FLEArray<nodeIndexT> nodeComponents(numNodes);
  nodeIndexT numComponents = ccoc.getNodeComponents( nodeComponents );
  FLEArray<size_t> perm = FLEArrayIdentityPermutation( nodeComponents.size() );
  std::stable_sort( perm.begin(), perm.end(), FLEArrayByIndexSortingPredicate<nodeIndexT>(nodeComponents) );

  // print out the nodes sorted by component.
  nodeIndexT curNodeIndex = 0;
  for( nodeIndexT i = 0; i < numComponents; ++i ){
    for( ; // no initialization
	 (curNodeIndex < nodeComponents.size()) && nodeComponents[perm[curNodeIndex]] == i;
	 ++curNodeIndex ){
      std::cout << labels( perm[curNodeIndex] ) << "\t" << i << std::endl;
    }
  }
}

}; // end namespace cbrc

int main( int argc, char** argv ){
  std::string usage( "Usage: " );
  usage += argv[0];
  usage += " graphFile";
  if( argc != 2 ){
    std::cout << usage << std::endl;
    exit( -1 );
  }
  std::string filename( argv[1] );
  if( !cbrc::perlish::canStatFile( filename ) ){
    std::cout << " could not open file: " << filename << std::endl;
    exit( -1 );
  }
  cbrc::listConnectedComponents( filename );
  return 1;
}

