/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *  Last Modified: $Date: 2007/02/13 02:40:44 $
 *  
 *  Description: namespace including various curve smoothing functions.
 *
 *  Purpose: Originally created for use with WoLF to monotonically map example distance to 
 *           the probability the examples are equal.
 *           
 */
#ifndef _SMOOTHING_HH_
#define _SMOOTHING_HH_
#include <algorithm>
#include <vector>
#include <list>
#include <assert.h>
#include <iostream>
#include "utils/geometry/WeightedPoint2D.hh"

namespace cbrc{

namespace smoothing{

  typedef Point2D<double, double> POINT;
  typedef WeightedPoint2D<double, double, size_t> NPOINT; // for point observed n times.
  typedef WeightedPoint2D<double, double, double> WPOINT; // for weighted point.

  template <typename XT, typename YT, typename WT>
  void inline sortInPlaceAscendingValue( std::vector< WeightedPoint2D<XT,YT,WT> >& u ){
        std::sort( u.begin(),
		   u.end(),
		   WeightedPoint2D<XT,YT,WT>::xLess );
  }

  /* Merge adjacent points with identical x values.
   * The y values of the merged point is the weighted average of the y values of the merged points.
   * The weight of the merged point is the sum of the weights of the merged points.
   */
  std::vector<NPOINT> mergeIdenticalAdjacentX( const std::vector<NPOINT>& u );
  std::vector<WPOINT> mergeIdenticalAdjacentX( const std::vector<WPOINT>& u );


  /* return a smoothed version v of the points in u, such that
   *     1. There is exactly one point v for each unique x coordinate from u
   *     2. The y coordinate of each point is based on an average of at least
   *        minSampleSize points.
   *     3. The y coordinates are monotonic within a small tolerance.
   *
   * Note: * Modifies u by sorting elements in increasing x order. Otherwise u is unchanged.
   *       * The average value of the y coordinates are not preserved.
   *
   * Limitations: Error bar information on the estimates of y values is not given.
   */
  std::vector<POINT> monotonicSmoothY
  ( std::vector<NPOINT>& u,
    const bool ensureYIncreasing = true,  // set to false to ensure y coordinates decrease with X
    size_t minSampleSize = 32 );


  /* Achtung! 1. alters v, 
   *          2. assumes the x values of points in u are sorted in ascending order.
   *          3. assumes minSampleSize is less than the total weight of the points in v.
   *
   * Make v based on u, but changed so that each y point is based on at least minSampleSize samples.
   * The y values for x points with fewer than minSampleSize samples is calculated by
   * averaging in the y values of the points around it with the closest x values.

   * Note that upon completion, the weighted average of the Y coordinates of v will generally be
   * different than that of u.
   */
  void averageYCoordinatesWithNeighboringPointsUntilMinSampleSizeAttained
  ( const std::vector<NPOINT>& u, std::vector<POINT>& v, const size_t minSampleSize );

  void averageYCoordinateOfPointIWithNeighborPointUntilMinSampleSizeAttained
  ( const std::vector<NPOINT>& u, std::vector<POINT>& v, const size_t minSampleSize, const size_t index );

}; // end namespace smoothing
}; // end namespace cbrc
#endif // defined _SMOOTHING_HH_
