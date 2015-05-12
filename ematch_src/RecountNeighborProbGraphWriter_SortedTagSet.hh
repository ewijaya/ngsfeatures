/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/09/20 07:13:21 $
 *
 *  Description: Class to write binary format neighbor graph from text files
 *
 */
#ifndef RECOUNTNEIGHBORPROBGRAPHWRITER_SORTEDTAGSET_HH_
#define RECOUNTNEIGHBORPROBGRAPHWRITER_SORTEDTAGSET_HH_
#include <iostream>
#include "SortedTagSet.hh"
#include "RecountNeighborList.hh"

namespace cbrc{

class RecountNeighborProbGraphWriter{
public:

  /* ********** TYPEDEFS ********** */
  typedef  SortedTagSet::tagSeqT  tagSeqT;

  /* ********** CONSTRUCTORS ********** */

  RecountNeighborProbGraphWriter(  std::istream& tagSeqsIstream  )
    : tagId_to_seq( tagSeqsIstream )
  {}

  // returns number of neighbor lists written.
  size_t write( std::ofstream& ofStream,
                std::istream&  tagNeighborlistIstream  );

  /* ********** ACCESSORS ********** */
private:

  // object data
  const SortedTagSet  tagId_to_seq;
};

} // end namespace cbrc
#endif // RECOUNTNEIGHBORPROBGRAPHWRITER_SORTEDTAGSET_HH_
