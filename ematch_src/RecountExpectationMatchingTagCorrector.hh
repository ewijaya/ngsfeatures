/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.22
 *  Last Modified: $Date: 2009/10/07 10:25:31 $
 *
 *  Description: Class to correct sequence tag counts
 *               based on the method of searching for an estimated set of
 *               true counts, whose estimated counts based on the error model
 *               matches the real observed counts as closely as possible
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
     const size_t                          numRoundsToWaitForBetter  =  10
     );


  /* --------------- ACCESSORS --------------- */
  const size_t&  numRoundsToWaitForBetter() const{
    return _numRoundsToWaitForBetter;
  }

  const RecountTagCounts&  estimatedTrueCounts() const{
    return  estTrueCounts[ curIdx() ];
  }

  const tagCountT&  bestEstError() const{
    return _bestEstError;
  }

  const RecountTagCounts&  bestEstCounts() const{
    return _bestEstCounts;
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

  // Compute count correction using difference between observed and expected counts
  void computeCountCorrections();


private:

  /* --------------- OBJECT DATA --------------- */
  const RecountComputerForGraphOnDisk&  expectationComputer;
  const RecountTagCounts&               observedCounts;

  /* ---------- Optimization Search Parameters ---------- */
  const size_t  _numRoundsToWaitForBetter;


  RecountTagCounts  expectedCounts;
  RecountTagCounts  estTrueCounts[2];

  // bestEstCounts, with error bestEstError
  // found after bestEstCountsIteration steps of iterative optimization.
  tagCountT         _bestEstError;
  RecountTagCounts  _bestEstCounts;
  size_t            _bestEstCountsIteration;

  int  _curIdx;
};

} // end namespace cbrc
#endif // RECOUNTEXPECTATIONMATCHINGTAGCORRECTOR_HH_
