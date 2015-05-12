/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: Copied from listConnectedComponents, created 2003.6.10
 *  Last Modified: $Date: 2009/04/26 06:09:51 $
 *  
 *  Description: Randomly choose one representative from each connected component.
 *
 *  Purpose: Originally created to construct non-redundant dataset from WoLF PSORT
 *           taboo list.
 *
 */
#include <fstream>
#include <iostream>
#include "utils/graph/OneEdgePerLineFormat.hh"
#include "utils/perlish/perlish.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings.hh"
#include "utils/FLArray/FLEArrayFunctions.hh"
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "../ConnectedComponentOnlineComputer.hh"

namespace cbrc{

void printRandomRepresentativeFromEachConnectedComponent( std::string filename ){
  std::ifstream is;
  is.open( filename.c_str() );
  const nodeIndexT numNodes = OneEdgePerLineFormat::numNodesInStream( is ); // first count nodes.
  is.clear(); is.seekg( 0, std::ios::beg ); // reset the filestream to be reread from the beginning.
  
  ConnectedComponentOnlineComputer ccoc( numNodes );
  FLEArray<nodeIndexT> nodeComponents(numNodes);
  SerialNumbersForStrings<nodeIndexT> labels;

  // Read in graph and add edges.
  while( !is.eof() ){
    std::string line = perlish::slurpLine( is );
    if( line.size() == 0 ) break;

    const std::vector<std::string> fields
      = perlish::split( OneEdgePerLineFormat::inputDelimiter(), line );

    if( !fields.size() == 2 ){
      std::cout << "Format error. Expected two fields when reading line: " << line << std::endl;
      exit( -1 );
    }
    nodeIndexT n0 = labels.getSerialNumber( fields[0] );
    nodeIndexT n1 = labels.getSerialNumber( fields[1] );
    if( fields[0] == fields[1] ){
      // In order for labels to have the same size as numNodes it is important that this
      // if statement comes *after* the node name is added to labels. The case in which this is important
      // is a singleton node with a self-edge.
      std::cerr << "Warning. Skipping self edge from line: " << line << std::endl;
      continue;
    }
    ccoc.addEdge( n0, n1 );
  }

  // create permutation array which indirectly sorts the elements of nodeComponents
  nodeIndexT numComponents = ccoc.getNodeComponents( nodeComponents );
  FLEArray<size_t> perm = FLEArrayIdentityPermutation( nodeComponents.size() );
  std::stable_sort( perm.begin(), perm.end(), FLEArrayByIndexSortingPredicate<nodeIndexT>(nodeComponents) );

  /* ***** print out representative for each component ***** */
  for( nodeIndexT i = 0; i < numComponents; ++i ){
    nodeIndexT selectedNodeIndex = FLEArrayFunctions::randomElement( nodeComponents );
    std::cout << labels( perm[selectedNodeIndex] ) << std::endl;
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
  cbrc::printRandomRepresentativeFromEachConnectedComponent( filename );
  return 1;
}

