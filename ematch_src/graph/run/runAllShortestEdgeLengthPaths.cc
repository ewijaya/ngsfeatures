/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.30
 *  Last Modified: $Date: 2009/04/26 06:43:16 $
 *  
 *  Purpose: run AllShortestEdgeLengthPaths.
 */
#include <iostream>
#include "../NeighborListGraph.hh"
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"

namespace cbrc{
  void runAllShortestEdgeLengthPaths( bool summarizeOnly ){
    NeighborListGraph nlg;
    nlg.readOneEdgePerLineFormat( std::cin );
    std::cerr << "num nodes read: " << nlg.numNodes() << std::endl;
    //    nlg.checkConsistency();
    FLENumArray<nodeIndexT> histo( nlg.numNodes() );
    histo.zero();
    for( nodeIndexT i = 0; i < nlg.numNodes(); ++i ){
      FLENumArray<nodeIndexT> edgeLen = nlg.shortestPathByEdgeLength( i );
      FLEArray<size_t> perm = FLEArrayIdentityPermutation( edgeLen.size() );
      std::stable_sort( perm.begin(), perm.end(), FLEArrayByIndexSortingPredicate<nodeIndexT>(edgeLen) );

      if( !summarizeOnly ) std::cout << nlg.label(i);
      for( nodeIndexT j = 0; j < perm.size(); ++j ){
	nodeIndexT pathEnd = perm[j];
	if( edgeLen[pathEnd] == nlg.numNodes() ) continue; // node j not reachable from i
	if( pathEnd == i ) continue;
	++histo[ edgeLen[pathEnd] ];
	if( summarizeOnly ) continue;
	if( (j==0) || (edgeLen[pathEnd] != edgeLen[perm[j-1]]) ) std::cout << " " << edgeLen[pathEnd];
	std::cout << " " << nlg.label(pathEnd);
      }
      if( !summarizeOnly ) std::cout << std::endl;
    }
    nodeIndexT graphDiameter = 0;
    for( nodeIndexT i = 0; i < histo.size(); ++i ){
      if( histo[i] ) graphDiameter = i;
    }
    std::cout << "graphDiameter: " << graphDiameter << std::endl;
    if( graphDiameter == 0 ){
      std::cout << "graph has no edges!\n";
      return;
    }
    std::cout << histo[0];
    for( nodeIndexT i = 1; i <= graphDiameter; ++i ){
      std::cout << " " << histo[i];
    }
    std::cout << std::endl;
  }

}; // end namescape cbrc

int main( int argc, char** argv ){
  std::string usage( "Usage:" );
  usage += argv[0];
  usage += " [-s]\n";
  if( argc > 2 ){
    std::cout << usage;
    exit( -1 );
  }
  if( argc == 2 ){
    if( strcmp( argv[1], "-s" ) ){
      std::cout << usage;
      exit( -1 );
    }
    cbrc::runAllShortestEdgeLengthPaths( true );
  }else{
    cbrc::runAllShortestEdgeLengthPaths( false );
  }
  return 1;
}

