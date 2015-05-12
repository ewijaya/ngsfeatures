/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.11
 *  Last Modified: $Date: 2009/09/20 08:19:05 $
 *
 *  Description: 
 *      Class to represent a graph in which nodes are tags
 *      and edges are the neighbors of each tag, weighted by
 *      probability of error. Neighbor list representation
 *      designed to be processed sequentially so that the
 *      only the neighbors of a single tag need to be held
 *      in memory at any given time.
 *
 *  Purpose: Created for RECOUNT project
 *
 */
#ifndef RECOUNTNEIGHBORPROBGRAPHONDISK_HH_
#define RECOUNTNEIGHBORPROBGRAPHONDISK_HH_
#include <iostream>
#include "SortedTagSet.hh"
#include "RecountNeighborList.hh"
#include "RecountNeighborProbGraphFormat.hh"

namespace cbrc{

class RecountNeighborProbGraphOnDisk{
public:

  /* ********** CONSTRUCTORS ********** */
  RecountNeighborProbGraphOnDisk(  const SortedTagSet& tagId_to_seq,
				   /***/ std::ifstream&   graphFile  )
    : _tagId_to_seq( tagId_to_seq ),
      graphFile   ( graphFile    )
  {
    init();
  }


  /* ********** ACCESSORS ********** */

  // number of nodes with neighbors
  const size_t& size() const{  return _size;  }

  const RecountNeighborList& curNeighborList() const{
    return _curNeighborList;
  }

  const SortedTagSet& tagId_to_seq() const{
    return _tagId_to_seq;
  }

  
  /* ***** Iterator-like methods ***** */
  void readFirstNode(){
    rewind();
    DO_OR_DIEF(  _curNeighborList.read( graphFile ),
		 "Binary input file error. Could not read first record"  );
  }

  bool readNextNode(){
    return  _curNeighborList.read( graphFile );
  }


private:
  void init();

  void rewind(){
    graphFile.clear();
    graphFile.seekg( RecountNeighborProbGraphFormat::headerSize() );
  }

  /* ***** Object Data ***** */
  RecountNeighborList  _curNeighborList;

  const SortedTagSet&  _tagId_to_seq;

  std::ifstream&  graphFile;

  size_t _size;

};

} // end namespace cbrc
#endif // RECOUNTNEIGHBORPROBGRAPHONDISK_HH_
