/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.13
 *  Last Modified: $Date: 2009/09/23 09:32:47 $
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
#include "RecountNeighborProbGraphOnDisk.hh"
#include "RecountTagCounts.hh"

namespace cbrc{

class RecountComputerForGraphOnDisk{
public:
  /* ********** CONSTRUCTORS ********** */
  RecountComputerForGraphOnDisk( RecountNeighborProbGraphOnDisk& neighborProbGraph )
    : _neighborProbGraph( neighborProbGraph )
  {}

  /* ********** ACCESSORS ********** */
  const RecountNeighborProbGraphOnDisk&  neighborProbGraph() const
  {
    return _neighborProbGraph;
  }

  /* ********** METHODS ********** */
  // set observedCounts(i) to the mean number of counts of tag i
  void meanCountsFromTrue(  /***/ RecountTagCounts& observedCounts,
			    const RecountTagCounts& trueCounts  ) const;


  // set observedCounts(i) to the variance of the number of counts of tag i
  void varianceCountsFromTrue(  /***/ RecountTagCounts& variances,
				const RecountTagCounts& trueCounts  ) const;



private:
  // object data
  RecountNeighborProbGraphOnDisk&  _neighborProbGraph;
};

} // end namespace cbrc
#endif // RECOUNTCOMPUTERFORGRAPHONDISK_HH_
