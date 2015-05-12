/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.13
 *  Last Modified: $Date: 2009/09/23 09:32:55 $
 *  Description: See header file.
 */
#include <boost/foreach.hpp>
#include "RecountComputerForGraphOnDisk.hh"


namespace cbrc{


void
RecountComputerForGraphOnDisk::
meanCountsFromTrue(  /***/ RecountTagCounts& observedCounts,
		     const RecountTagCounts& trueCounts  ) const{

  GDB_ASSERTF(  observedCounts.size() == trueCounts.size(),
	        "Expected equal sizes but got observedCounts:%zu, trueCounts:%zu",
	        observedCounts.size(), trueCounts.size()  );

  observedCounts.zero();

  _neighborProbGraph.readFirstNode();
  do{

    const RecountNeighborList&  curNeighborList  =  neighborProbGraph().curNeighborList();

    const tagIdT&               trueTagId        =  curNeighborList.id();

    const tagCountT&            trueTagCount     =  trueCounts(trueTagId);


    BOOST_FOREACH( const RecountTagIdProbPair& tagIdProbPair, curNeighborList ){
      observedCounts[ tagIdProbPair.id() ]  +=  trueTagCount * tagIdProbPair.prob();
    }


  } while(  _neighborProbGraph.readNextNode()  );

}



void
RecountComputerForGraphOnDisk::
varianceCountsFromTrue(  /***/ RecountTagCounts& variances,
			 const RecountTagCounts& trueCounts  ) const{

  GDB_ASSERTF(  variances.size() == trueCounts.size(),
	        "Expected equal sizes but got variances:%zu, trueCounts:%zu",
	        variances.size(), trueCounts.size()  );

  variances.zero();

  _neighborProbGraph.readFirstNode();
  do{

    const RecountNeighborList&  curNeighborList  =  neighborProbGraph().curNeighborList();

    const tagIdT&               trueTagId        =  curNeighborList.id();

    const tagCountT&            trueTagCount     =  trueCounts(trueTagId);


    BOOST_FOREACH( const RecountTagIdProbPair& tagIdProbPair, curNeighborList ){
      const probT&  prob  =  tagIdProbPair.prob();
      variances[ tagIdProbPair.id() ]  +=  trueTagCount *  (1 - prob) * prob;
    }


  } while(  _neighborProbGraph.readNextNode()  );

}







} // end namespace cbrc

