/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/09/20 11:48:26 $
 *
 *  Description: Class to write binary format neighbor graph from text files
 *
 */
#ifndef RECOUNTNEIGHBORPROBGRAPHWRITER_HH_
#define RECOUNTNEIGHBORPROBGRAPHWRITER_HH_
#include <iostream>
#include "TagSet.hh"
#include "RecountNeighborList.hh"

namespace cbrc{

class RecountNeighborProbGraphWriter{
public:

  /* ********** TYPEDEFS ********** */

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
  const TagSet  tagId_to_seq;
};

} // end namespace cbrc
#endif // RECOUNTNEIGHBORPROBGRAPHWRITER_HH_
