/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.30
 *  Last Modified: $Date: 2009/04/26 05:33:44 $
 *  
 *  Description: See header files.
 */
#include "utils/gdb/gdbUtils.hh"
#include "NeighborListGraph.hh"
#include "OneEdgePerLineFormat.hh"
#include "utils/FLArray/FLStack.hh"
#include "utils/BitMatrix/BitArray.hh"

namespace cbrc{


void NeighborListGraph::readOneEdgePerLineFormat( std::istream& is ){

  // read in graph
  while( !is.eof() ){
    std::string line = perlish::slurpLine( is );
    if( line.size() == 0 ) break;

    std::vector<std::string> fields
      = perlish::split( OneEdgePerLineFormat::inputDelimiter(), line );

    GDB_ASSERTF( fields.size() == 2,
		 "Expected two fields when reading line: %s", line.c_str() );

    if( fields[0] == fields[1] ){
      std::cerr << "Warning. Skipping self edge from line: " << line << std::endl;
      // okay to skip here because adding a singleton node does not change the desired output.
      continue;
    }

    const nodeIndexT n0 = labels.getSerialNumber( fields[0] );
    if( n0 >= ne.size() )  ne.push_back( std::vector<nodeIndexT>() );

    const nodeIndexT n1 = labels.getSerialNumber( fields[1] );
    if( n1 >= ne.size() )  ne.push_back( std::vector<nodeIndexT>() );

    ne[n0].push_back( n1 );
    ne[n1].push_back( n0 );
  }

  // sort neighbor lists.
  for( size_t i = 0; i < ne.size(); ++i ){
    std::sort( ne[i].begin(), ne[i].end() );
  }
}


void NeighborListGraph::checkConsistency() const{
  for( size_t i = 0; i < numNodes(); ++i ){
    for( size_t j = 0; j < numNodes(); ++j ){
      if( i == j ) continue;
      if( hasEdge( i,j ) != hasEdge( j,i ) ){
	std::cout << "Error: hasEdge( " << i << "," << j << " ) != hasEdge( " << j << "," << i << ")\n";
	exit( -1 );
      }
    }
  }
}



bool NeighborListGraph::hasEdge( const nodeIndexT& n0, const nodeIndexT& n1 ) const{
  // assert( std::binary_search( ne[n0].begin(), ne[n0].end(), n1 ) ==
  //         std::binary_search( ne[n1].begin(), ne[n1].end(), n0 ) );
  if( ne[n0].size() <= ne[n1].size() ){
    return std::binary_search( ne[n0].begin(), ne[n0].end(), n1 );
  }else{
    return std::binary_search( ne[n1].begin(), ne[n1].end(), n0 );
  }
}


// Returns the number of edges on the shortest path from each node to n.
// The number is set to numNodes() for nodes which are unreachable from n.
FLENumArray<nodeIndexT>
NeighborListGraph::shortestPathByEdgeLength( const nodeIndexT& n ) const{

  assert( n < numNodes() );

  FLENumArray<nodeIndexT> retVal( numNodes(), numNodes() );

  BitArray reached( numNodes() );
  reached.zero();
  retVal[n] = 0;
  reached.set( n );

  FLStack<nodeIndexT> stack0( numNodes() );
  FLStack<nodeIndexT> stack1( numNodes() );
  
  std::vector< std::vector<nodeIndexT> > nodeStack( numNodes()-1 );
  for( size_t i = 0; i < ne[n].size(); ++i ){
    nodeIndexT curPathEnd = (ne[n])[i];
    stack0.push( curPathEnd );
    retVal[ curPathEnd ] = 1;
    reached.set( curPathEnd );
  }

  bool done = true; // initialized to silence compiler warning.
  for( size_t depth = 0; depth < nodeStack.size()-1; ++depth ){
    done = true;
    for( size_t i = 0; i < stack0.size(); ++i ){
      nodeIndexT expandingNode = stack0[i];
      for( size_t j = 0; j < ne[expandingNode].size(); ++j ){
	nodeIndexT curPathEnd = (ne[expandingNode])[j];
	if( !reached[ curPathEnd ] ){
	  stack1.push( curPathEnd );
	  retVal[curPathEnd] = depth+2;
	  reached.set( curPathEnd );
	  done = false;
	}
      }
    }
    if( done ) break;
    done = true;
    ++depth;
    if( depth >= nodeStack.size() ) break;
    stack0.zero();
    for( size_t i = 0; i < stack1.size(); ++i ){
      nodeIndexT expandingNode = stack1[i];
      for( size_t j = 0; j < ne[expandingNode].size(); ++j ){
	nodeIndexT curPathEnd = (ne[expandingNode])[j];
	if( !reached[ curPathEnd ] ){
	  stack0.push( curPathEnd );
	  retVal[curPathEnd] = depth+2;
	  reached.set( curPathEnd );
	  done = false;
	}
      }
    }
    if( done ) break;
    stack1.zero();
  }
  return retVal;
}


std::ostream& operator<<( std::ostream& os, const NeighborListGraph& nlg ){
  os << nlg.numNodes() << std::endl;
  for( size_t i = 0; i < nlg.numNodes(); ++i ){
    if( nlg.ne[i].empty() ) continue;
    for( size_t j = 0; (j < nlg.ne[i].size()) && (nlg.ne[i][j] <= i); ++j ){
      os << nlg.labels(i) << " " << nlg.labels( nlg.ne[i][j] ) << std::endl;
    }
  }
  return os;
}

}; // end namespace
