/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.22
 *  Last Modified: $Date: 2009/10/04 13:24:01 $
 *  Description: See header file.
 */
//#include <numeric_limits>
#include "RecountExpectationMatchingTagCorrector.hh"

namespace cbrc{


/* --------------- CONSTRUCTORS --------------- */

RecountExpectationMatchingTagCorrector::RecountExpectationMatchingTagCorrector
(  const RecountComputerForGraphOnDisk&  expectationComputer,
   const RecountTagCounts&               observedCounts,
   const tagCountT                       maxCountDiff_convergenceCriterion,
   const double                          momentum,
   const tagCountT                       initialLearningRate,
   const size_t                          numIterationsToWaitForImprovement,
   const tagCountT                       learningRateDeaccelarationFactor
   )
  : expectationComputer              ( expectationComputer                 ),
    observedCounts                   ( observedCounts                      ),
    maxCountDiff_convergenceCriterion(  maxCountDiff_convergenceCriterion  ),
    momentum                         ( momentum                            ),
    initialLearningRate              (  initialLearningRate                ),
    numIterationsToWaitForImprovement(  numIterationsToWaitForImprovement  ),
    learningRateDeaccelarationFactor (  learningRateDeaccelarationFactor   )  
{
  _curIdx = 0;

  expectedCounts.setSize( observedCounts.size() );
  
  estTrueCounts[ prevIdx() ].assign( observedCounts );
  estTrueCounts[  curIdx() ].setSize( observedCounts.size() );

  countCorrections[ prevIdx() ].setSize( observedCounts.size() );
  countCorrections[  curIdx() ].setSize( observedCounts.size() );

  countCorrections[ prevIdx() ].zero();
  countCorrections[  curIdx() ].zero();


  /* ---------- Initialize Current Best Solution Info ---------- */
  _bestEstCounts.setSize( observedCounts.size() );
  _bestEstCounts.zero();
  _bestEstCountsTotalFoldChange  =  std::numeric_limits<tagCountT>::max();
}



void RecountExpectationMatchingTagCorrector::inferTrueTagCounts(){

  tagCountT  curMaxCountDiff;

  size_t curIteration  =  1;
  size_t prevSearchResetIteration = 1;


  // ITERATIVE_SEARCH:
  for( curMaxCountDiff  =  maxCountDiff_convergenceCriterion + 1.0;
       curMaxCountDiff > maxCountDiff_convergenceCriterion;
       incrementIdx(), ++curIteration
	){

    expectationComputer.meanCountsFromTrue(  expectedCounts,
					     estTrueCounts[ prevIdx() ]  );

    computeCountCorrections_by1stDerivative();

    countCorrections[ prevIdx() ]  *=  momentum;

    RecountTagCounts::assignSum(  estTrueCounts[  curIdx() ],
				  estTrueCounts[ prevIdx() ],
				  countCorrections[  curIdx() ],
				  countCorrections[ prevIdx() ]  );

    const tagCountT  curError  =  totalFoldChangeError();


    if( 0 ){  // Change to if(1) when debugging.
      std::cout  <<  "--------------------------------------------------\n"
		 <<  "iteration: "                     <<  curIteration
		 <<  "estTrueCounts[ prvIdX() ]\n"     <<  estTrueCounts[ prevIdx() ]
		 <<  "expected counts:\n"              <<  expectedCounts
		 <<  "countCorrections[ curIdx() ]\n"  <<  countCorrections[ curIdx() ];
    }


    // Update if current estimated counts better than best previously seen
    if( curError  <  bestEstCountsTotalFoldChange()  ){
      _bestEstCountsTotalFoldChange  =  curError;
      _bestEstCountsIteration        =  curIteration;
      prevSearchResetIteration      =  curIteration; 
      _bestEstCounts.assign(  estTrueCounts[ curIdx() ]  );
    }


    // If no progress for too long, then go back to best previously seen
    // estimated counts and reduce learning rate.
    if(   ( curIteration - prevSearchResetIteration )  >  numIterationsToWaitForImprovement  ){
      estTrueCounts[ curIdx() ].assign(  bestEstCounts()  );
      countCorrections[ prevIdx() ].zero();
      countCorrections[  curIdx() ].zero();
      _learningRate  *=  learningRateDeaccelarationFactor;
      prevSearchResetIteration  =  curIteration;
      curMaxCountDiff  =  maxCountDiff_convergenceCriterion + 1.0;
      continue; // ITERATIVE_SEARCH;
    }

    curMaxCountDiff
      =  RecountTagCounts::maxAbsDiff(  estTrueCounts[ prevIdx() ],
					estTrueCounts[  curIdx() ]  );
      
  }


} // end inferTrueTagCounts



void RecountExpectationMatchingTagCorrector::computeCountCorrections_by1stDerivative
(){

  RecountTagCounts::const_iterator estTrue    =   estTrueCounts[ prevIdx() ].constBegin();
  RecountTagCounts::const_iterator observed   =   observedCounts.constBegin();
  RecountTagCounts::const_iterator expected   =   expectedCounts.constBegin();

  for(  RecountTagCounts::iterator correction  =  countCorrections[ curIdx() ].begin();
	correction  !=  countCorrections[ curIdx() ].end();
	++estTrue, ++observed, ++expected, ++correction  )
    {

      *correction   =   learningRate() *  (*observed - *expected);

    }

} // end computeCountCorrections_byExpectationMatching1stDerivation



tagCountT  RecountExpectationMatchingTagCorrector::totalFoldChangeError() const{
  tagCountT  curSum  =  0.0;
  
  RecountTagCounts::const_iterator observed   =   observedCounts.constBegin();
  RecountTagCounts::const_iterator expected   =   expectedCounts.constBegin();

  static const tagCountT  pseudoCount   =   5.0;

  for( ; /* noop */
       observed  !=  observedCounts.constEnd();
       ++observed, ++expected  ){
    assert(  *observed  >=  1.0  );
    curSum   +=   log(  (pseudoCount + fabs( *observed - *expected ))
			/  (pseudoCount + *observed)  );
  }

  return curSum;

} // end totalFoldChangeError



} // end namespace cbrc

