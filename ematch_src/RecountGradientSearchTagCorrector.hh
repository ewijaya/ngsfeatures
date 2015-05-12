/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.22
 *  Last Modified: $Date: 2009/10/04 13:23:57 $
 *
 *  Warning: Mothballed 2009/10/04, because it is not clear that
 *           a gradient descent method is necessary.
 *
 *  Description: Class to correct sequence tag counts
 *               based gradient search. Not fully implemented.
 *
 *  Purpose: Created for the RECOUNT project.
 *
 */
#ifndef RECOUNTEXPECTATIONMATCHINGTAGCORRECTOR_HH_
#define RECOUNTEXPECTATIONMATCHINGTAGCORRECTOR_HH_
#include <iostream>
#include "RecountComputerForGraphOnDisk.hh"

namespace cbrc{

class RecountExpectationMatchingTagCorrector{
public:

  /* --------------- TYPEDEFS --------------- */

  /* --------------- CONSTRUCTORS --------------- */
  RecountExpectationMatchingTagCorrector
  (  const RecountComputerForGraphOnDisk&  expectationComputer,
     const RecountTagCounts&               observedCounts,
     const tagCountT                       maxCountDiff_convergenceCriterion  =  0.4,
     const double                          momentum                           =  0.1,
     const tagCountT                       initialLearningRate                =  1.0,
     const size_t                          numIterationsToWaitForImprovement  =  100,
     const tagCountT                       learningRateDeaccelarationFactor   =  0.9

     );


  /* --------------- ACCESSORS --------------- */
  const RecountTagCounts&  estimatedTrueCounts() const{
    return  estTrueCounts[ curIdx() ];
  }

  const tagCountT&  learningRate() const{
    return  _learningRate;
  }

  const RecountTagCounts&  bestEstCounts() const{
    return _bestEstCounts;
  }

  const tagCountT&         bestEstCountsTotalFoldChange() const{
    return _bestEstCountsTotalFoldChange;
  }

  const size_t&            bestEstCountsIteration() const{
    return _bestEstCountsIteration;
  }


  /* --------------- Current/Previous Index Methods --------------- */

  const int&  curIdx() const{
    return _curIdx;
  }

  int prevIdx() const{
    return  1 - curIdx();
  }

  void incrementIdx(){
    _curIdx = prevIdx();
  }


  void inferTrueTagCounts();

  /* Compute count correction using difference between observed and expected counts
     Input:   estTrueCounts[ prevIdx() ],  observedCounts,  expectedCounts
     Output:  countCorrections[ curIdx() ]
  */
  void computeCountCorrections_by1stDerivative();


  /* Compute sum over all tags i of  | expected(i) - observed(i) | / i  
   * Input: observedCounts, expectedCounts
   */
  tagCountT  totalFoldChangeError() const;

private:

  /* --------------- OBJECT DATA --------------- */
  const RecountComputerForGraphOnDisk&  expectationComputer;
  const RecountTagCounts&               observedCounts;

  /* ---------- Optimization Search Parameters ---------- */
  const tagCountT  maxCountDiff_convergenceCriterion;
  const tagCountT  momentum;
  const tagCountT  initialLearningRate;
  const tagCountT  numIterationsToWaitForImprovement;
  const tagCountT  learningRateDeaccelarationFactor;
  /***/ tagCountT  _learningRate;


  RecountTagCounts  expectedCounts;
  RecountTagCounts  estTrueCounts[2];
  RecountTagCounts  countCorrections[2];

  // bestEstCounts, with total fold change bestEstCountsTotalFoldChange,
  // found after bestEstCountsIteration steps of iterative optimization.
  RecountTagCounts  _bestEstCounts;
  tagCountT         _bestEstCountsTotalFoldChange;
  size_t            _bestEstCountsIteration;

  int  _curIdx;
};

} // end namespace cbrc
#endif // RECOUNTEXPECTATIONMATCHINGTAGCORRECTOR_HH_
