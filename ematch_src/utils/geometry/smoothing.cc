/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.30
 *  Last Modified: $Date: 2007/07/11 06:38:13 $
 *  
 *  Description: See Header File.
 *
 */
#include <cmath>
#include "utils/geometry/smoothing.hh"

namespace cbrc{

namespace smoothing{

std::vector<NPOINT> mergeIdenticalAdjacentX( const std::vector<NPOINT>& u ){
  std::vector<NPOINT> v;
  
  for( size_t i = 0; i < u.size(); ){
    double ySum = 0.0;
    size_t j = i+1;
    size_t weightSum = 0;
    // at for loop termination, u[j-1] is the last element after u[i] such that u[j].y() == u[i].y()
    for( ; (j < u.size()) && (u[i].x() == u[j].x()); ++j ){
      ySum += u[j].y();
      weightSum += u[j].weight();
    }
    v.push_back(  NPOINT( u[i].x(),
			  (u[i].y() + ySum) / static_cast<double>(j-i),
			  u[i].weight() + weightSum )
		  );
    i = j;
  }
  return v;
}


std::vector<WPOINT> mergeIdenticalAdjacentX( const std::vector<WPOINT>& u ){
  std::vector<WPOINT> v;
  
  for( size_t i = 0; i < u.size(); ){
    double ySum = 0.0;
    size_t j = i+1;
    double weightSum = 0;
    // at for loop termination, u[j-1] is the last element after u[i] such that u[j].y() == u[i].y()
    for( ; (j < u.size()) && (u[i].x() == u[j].x()); ++j ){
      ySum += u[j].y();
      weightSum += u[j].weight();
    }
    v.push_back(  WPOINT( u[i].x(),
			  (u[i].y() + ySum) / static_cast<double>(j-i),
			  u[i].weight() + weightSum )
		  );
    i = j;
  }
  return v;
}



// Achtung! Modifies u by sorting elements in increasing x order.
std::vector<POINT> monotonicSmoothY( std::vector<NPOINT>& u,
				     const bool ensureYIncreasing, size_t minSampleSize ){
  assert( u.size() );

  { // error check for non-finite elements.
    std::vector<NPOINT>::iterator infiniteElement = 
      find_if( u.begin(), u.end(), NPOINT::nonFinite );
    if( infiniteElement != u.end() ){
      std::cout << __FILE__ << ":" << __LINE__ << " Error: cannot accept coordinates with infinite values\n u[" << distance( u.begin(), infiniteElement ) << "] = " << *infiniteElement << std::endl;
#ifdef CBRC_DEBUG
    for(;;);
#else
    exit( -1 );
#endif // debined CBRC_DEBUG
    }
  }


  // Calculate totalWeight of u and if necessary adjust minSampleSize so that
  // sqrt(totalWeight) <= minSampleSize <= totalWeight
  size_t totalWeight = 0;
  for( size_t i = 0; i < u.size(); ++i ) totalWeight += u[i].weight();
  if( minSampleSize > totalWeight ) minSampleSize = totalWeight;
  size_t rootSize = static_cast<size_t>( sqrt( totalWeight ) );
  if( minSampleSize < rootSize ) minSampleSize = rootSize;

  // The minus is because we have defined, disallowed condition in a negative sense rather than allowed condition.
  double tolerance = -(1.0 / ( static_cast<double>(totalWeight) ) );
		       
  POINT::TolerantYCompare yDisallowedCondition( tolerance, !ensureYIncreasing );

  std::sort( u.begin(), u.end(), NPOINT::xLess );

  std::vector<POINT> v;
  for( size_t i = 0; i < u.size(); ++i ){
    v.push_back( POINT( u[i].x(), u[i].y() ) );
  }

  averageYCoordinatesWithNeighboringPointsUntilMinSampleSizeAttained( u, v, minSampleSize );
  if( std::adjacent_find( v.begin(), v.end(), yDisallowedCondition ) == v.end() ){
    return v;
  }

  std::vector<size_t> minSampleSizes( v.size(), minSampleSize );
  size_t disallowedCount;
  while( std::adjacent_find( v.begin(), v.end(), yDisallowedCondition ) != v.end() ){
    disallowedCount = 0;
    for( size_t i = 0; i < v.size()-1; ++i ){
      if( yDisallowedCondition( v[i], v[i+1] ) ){
	++disallowedCount;
	if( minSampleSizes[i] < totalWeight ){
	  ++minSampleSizes[i];
	  averageYCoordinateOfPointIWithNeighborPointUntilMinSampleSizeAttained
	    ( u, v, minSampleSizes[i], i );
	}
	if( minSampleSizes[i+1] < totalWeight ){
	  ++minSampleSizes[i+1];
	  averageYCoordinateOfPointIWithNeighborPointUntilMinSampleSizeAttained
	    ( u, v, minSampleSizes[i+1], i+1 );
	}
      }
    }
  }

  return v;
}
    
// Achtung! 1. alters v, 
//          2. assumes the x values of points in u are sorted in ascending order.
//          3. assumes minSampleSize is less than the total weight of the points in v.
//
// Make v based on u, but changed so that each y point is based on at least minSampleSize samples.
// The y values for x points with few than minSampleSize samples the y value is calculated by
// averaging in the y values of the points around it with the closest x values.
//
// Note that upon completion, the weighted average of the Y coordinates of v will generally be
// different than that of u.
void averageYCoordinatesWithNeighboringPointsUntilMinSampleSizeAttained( const std::vector<NPOINT>& u, std::vector<POINT>& v, const size_t minSampleSize ){
  for( size_t i = 0; i < u.size(); ++i ){
    averageYCoordinateOfPointIWithNeighborPointUntilMinSampleSizeAttained( u, v, minSampleSize, i );
  }
}


void averageYCoordinateOfPointIWithNeighborPointUntilMinSampleSizeAttained
( const std::vector<NPOINT>& u,
  std::vector<POINT>& v,
  const size_t minSampleSize,
  const size_t i                   // v[i] is modified.
  ){
  assert( u.size() == v.size() );
  enum directionT { LEFT, RIGHT, BOTH, UNDETERMINED } takeFrom;

  size_t curNumSamples = u[i].weight();
  if( curNumSamples >= minSampleSize ){
    v[i].setY( u[i].y() );
    return;
  }
  double yTotal = u[i].weight() * u[i].y();


  size_t li = i-1;
  size_t ri = i+1;
  // Loop to pick up neighboring points closest to u[i] in the x direction.
  // In each iteration either --li, ++ri. or both should happen. This causes
  // further (in the x direction) points to be used.
  while( curNumSamples < minSampleSize ){
    bool lPossible = li < u.size()-1; // li is unsigned, in spirit this means li >= 0.
    bool rPossible = ri < u.size();
    takeFrom = UNDETERMINED;

    if( (!lPossible) && (!rPossible) ){
#ifdef CBRC_DEBUG
      for( ;; );
#else
      assert( lPossible || rPossible );
#endif // defined CBRC_DEBUG
    }
    if( lPossible && rPossible ){
      if(       (u[i].x() - u[li].x()) == (u[ri].x() - u[i].x())  )  takeFrom = BOTH;
      else if(  (u[i].x() - u[li].x()) <  (u[ri].x() - u[i].x())  )  takeFrom = LEFT;
      else                                                           takeFrom = RIGHT;
    }
    if( lPossible && !rPossible ) takeFrom = LEFT;
    if( rPossible && !lPossible ) takeFrom = RIGHT;

    size_t numToTake = 0; // silence compiler.
    switch( takeFrom ){
    case RIGHT:
      numToTake = u[ri].weight();
      if( numToTake + curNumSamples > minSampleSize ) numToTake = minSampleSize - curNumSamples;
      yTotal += numToTake * u[ri].y();
      ++ri;
      break;
    case LEFT:
      numToTake = u[li].weight();
      if( numToTake + curNumSamples > minSampleSize ) numToTake = minSampleSize - curNumSamples;
      yTotal += numToTake * u[li].y();
      --li;
      break;
    case BOTH:
      // nearest points to the left and right are equidistant. In this case take from both of them
      // equally until one of the other runs out of observations.
      // then continue to take from the one with more observations.
      // EXCEPT: stop the process at any point if continuing would exceed minSampleSize.
      {
	size_t numCommon = std::min( u[li].weight(), u[ri].weight() );
	numToTake = numCommon * 2;
	if( numToTake + curNumSamples >= minSampleSize ){
	  numToTake = minSampleSize - curNumSamples;
	}
	yTotal += (  ( numToTake * u[li].y() + numToTake * u[ri].y() ) / 2.0  );
	if( numToTake + curNumSamples < minSampleSize ){
	  size_t numExtraToTake = u[li].weight() + u[ri].weight() - numToTake;
	  size_t ei = ( u[li].weight() > u[ri].weight() ) ? li : ri;
	  if( numToTake + numExtraToTake + curNumSamples >= minSampleSize ){
	    numExtraToTake = minSampleSize - curNumSamples - numToTake;
	  }
	  yTotal += numExtraToTake * u[ei].y();
	  numToTake += numExtraToTake;
	}
      }
      --li; ++ri;
      break;
    case UNDETERMINED:
    default:
      std::cout << __FILE__ << ":" << __LINE__ << " Error: aborting\n";
    }
      
    curNumSamples += numToTake;
  } // end:  while( curNumSamples < minSampleSize )

  v[i].setY( yTotal / minSampleSize );
}



}; // end namespace smoothing

}; // end namespace cbrc
