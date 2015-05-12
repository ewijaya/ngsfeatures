/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.22
 *  Last Modified: $Date: 2010/07/16 05:12:44 $
 *  Description: See header file.
 */
#include "RecountExpectationMatchingTagCorrector.hh"
#include <limits>

namespace cbrc{


/* --------------- CONSTRUCTORS --------------- */

RecountExpectationMatchingTagCorrector::RecountExpectationMatchingTagCorrector
(  const RecountComputerForGraphOnDisk&  expectationComputer,
   const RecountTagCounts&               observedCounts,
   const size_t                          numRoundsToWaitForBetter
   )
  : expectationComputer        (  expectationComputer       ),
    observedCounts             (  observedCounts            ),
    _numRoundsToWaitForBetter  (  numRoundsToWaitForBetter  )
{
  _curIdx = 0;

  expectedCounts.setSize( observedCounts.size() );
  
  estTrueCounts[ prevIdx() ].assign( observedCounts );
  estTrueCounts[  curIdx() ].setSize( observedCounts.size() );


  /* ---------- Initialize Current Best Solution Info ---------- */
  _bestEstError  =  std::numeric_limits<tagCountT>::max();
  _bestEstCounts.setSize( observedCounts.size() );
  _bestEstCounts.zero();
  _bestEstCountsIteration  =  0;
}



void RecountExpectationMatchingTagCorrector::inferTrueTagCounts(){


  // ITERATIVE_SEARCH:
  for( tagCountT curIteration = 0;
       curIteration - bestEstCountsIteration()  <=  numRoundsToWaitForBetter();
       incrementIdx(), ++curIteration
	){

    expectationComputer.meanCountsFromTrue(  expectedCounts,
					     estTrueCounts[ prevIdx() ]  );

    computeCountCorrections();


    const tagCountT  curMaxCountDiff
      =  RecountTagCounts::maxAbsDiff(  estTrueCounts[ prevIdx() ],
					estTrueCounts[  curIdx() ]  );

    if( curMaxCountDiff < bestEstError() ){
      _bestEstError  =  curMaxCountDiff;
      _bestEstCountsIteration  =  curIteration;
      _bestEstCounts.assign(  estTrueCounts[  curIdx() ]  );
    }
  }


} // end inferTrueTagCounts



void RecountExpectationMatchingTagCorrector::computeCountCorrections
(){

  RecountTagCounts::const_iterator  estTruePrev   =   estTrueCounts[ prevIdx() ].constBegin();
  RecountTagCounts::const_iterator  observed      =               observedCounts.constBegin();
  RecountTagCounts::const_iterator  expected      =               expectedCounts.constBegin();

  for(  RecountTagCounts::iterator estTrueCur  =  estTrueCounts[ curIdx() ].begin();
	estTrueCur  !=  estTrueCounts[ curIdx() ].end();
	++estTruePrev, ++estTrueCur, ++observed, ++expected  )
    {
      //      *estTrueCur   =   std::max(  0.0,  *estTruePrev + *observed - *expected  );
      *estTrueCur   =   *estTruePrev + *observed - *expected;

    }

} // end computeCountCorrections



} // end namespace cbrc

