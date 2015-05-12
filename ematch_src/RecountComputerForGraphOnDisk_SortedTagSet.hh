/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.13
 *  Last Modified: $Date: 2009/09/25 02:51:57 $
 *
 *  Description: Class to compute various quantaties using
 *               RecountNeighborProbGraphOnDisk as representation
 *               of neighbor graph.
 *
 *  Purpose: Created for RECOUNT project.
 *
 */
#ifndef RECOUNTCOMPUTERFORGRAPHONDISK_HH_
#define RECOUNTCOMPUTERFORGRAPHONDISK_HH_
#include <iostream>
#include "RecountNeighborProbGraphOnDisk_SortedTagSet.hh"
#include "RecountTagCounts_SortedTagSet.hh"

namespace cbrc{

class RecountComputerForGraphOnDisk{
public:


  /* --------------- CONSTRUCTORS --------------- */

  RecountComputerForGraphOnDisk( RecountNeighborProbGraphOnDisk& neighborProbGraph )
    : _neighborProbGraph( neighborProbGraph )
  {}


  /* --------------- ACCESSORS --------------- */
  const RecountNeighborProbGraphOnDisk&  neighborProbGraph() const
  {
    return _neighborProbGraph;
  }


  /* --------------- GENERAL METHODS --------------- */
  // set observedCounts(i) to the mean number of counts of tag i
  void meanCountsFromTrue(  /***/ RecountTagCounts& observedCounts,
			    const RecountTagCounts& trueCounts  );


  // set observedCounts(i) to the variance of the number of counts of tag i
  void varianceCountsFromTrue(  /***/ RecountTagCounts& variances,
				const RecountTagCounts& trueCounts  );



private:
  // object data
  RecountNeighborProbGraphOnDisk&  _neighborProbGraph;
};

} // end namespace cbrc
#endif // RECOUNTCOMPUTERFORGRAPHONDISK_HH_
