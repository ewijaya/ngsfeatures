/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003,2007 Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *  Last Modified: $Date: 2007/02/09 08:58:41 $
 *  
 *  Description: A weighted two-dimensional point.
 *
 */
#ifndef _WEIGHTEDPOINT2D_HH_
#define _WEIGHTEDPOINT2D_HH_
#include "utils/geometry/Point2D.hh"


namespace cbrc{

template<typename XT, typename YT, typename WT=double>
class WeightedPoint2D{
public:
  WeightedPoint2D() : w(1){}
  WeightedPoint2D( const Point2D<XT, YT>& p ): xVar( p.x() ), yVar( p.y() ), w(1){}
  WeightedPoint2D( const WeightedPoint2D<XT, YT, WT>& wp ): xVar( wp.x() ), yVar( wp.y() ), w(wp.w){}
  WeightedPoint2D( const XT x, const YT y, const WT w = 1 ) : xVar(x), yVar(y), w(w){}
  operator Point2D<XT, YT>() const{ return Point2D<XT, YT>( xVar, yVar ); }
  bool operator!=( const WeightedPoint2D<XT, YT, WT>& p ){
    if( x() != p.x() ) return true;
    return( y() != p.y() );
  }
  XT x() const{ return xVar; }
  YT y() const{ return yVar; }
  WT weight() const{ return w; }
  void set( XT x, YT y ){ xVar = x; yVar = y; }
  void swapXY(){ XT temp = xVar; xVar = yVar; yVar = temp; } // assumes one can assign between types XT and YT.
  static bool xLess( const WeightedPoint2D<XT, YT, WT>& p0, const WeightedPoint2D<XT, YT, WT>& p1 ){ 
    return p0.x() < p1.x();
  }
  static bool xAscending(  const WeightedPoint2D<XT, YT, WT>& p0, const WeightedPoint2D<XT, YT, WT>& p1 ){ return p1.x() >= p0.x(); }
  static bool xDescending(  const WeightedPoint2D<XT, YT, WT>& p0, const WeightedPoint2D<XT, YT, WT>& p1 ){ return p1.x() <= p0.x(); }
  static bool yAscending(  const WeightedPoint2D<XT, YT, WT>& p0, const WeightedPoint2D<XT, YT, WT>& p1 ){ return p1.y() >= p0.y(); }
  static bool yDescending( const WeightedPoint2D<XT, YT, WT>& p0, const WeightedPoint2D<XT, YT, WT>& p1 ){ return p1.y() <= p0.y(); }
  static bool nonFinite( const WeightedPoint2D<XT, YT, WT>& p ){
    if( moreNumericLimits::isInfinity<XT>( p.x() )  ){
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return true;
    }
    if( moreNumericLimits::isNegativeInfinity<XT>( p.x() )  ){
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return true;
    }
    if( moreNumericLimits::isInfinity<YT>( p.y() )  ){
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return true;
    }
    if( moreNumericLimits::isNegativeInfinity<YT>( p.y() )  ){
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return true;
    }
    if( moreNumericLimits::isInfinity<WT>( p.weight() )  ){
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return true;
    }
    if(  moreNumericLimits::isNegativeInfinity<WT>( p.weight() )  ){
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return true;
    }
    return false;
  }
  void setX( XT x ){ xVar = x; }
  void setY( YT y ){ yVar = y; }

  // after returning, the new y coordinate is the weighted average of
  // the old y coordinate and the point p. The new weight is the
  // sum of the old weight and the weight of p.
  void averageYCoordinateAndSumWeightsWith( const WeightedPoint2D<XT, YT, WT>& p ){
    double totalWeight = static_cast<double>(  w + p.weight()  );
    yVar = (w * yVar + p.weight() * p.y()) / totalWeight;
    w = totalWeight;
  }

  // after returning, new x,y coordinates are the weighted average of
  // the old x,y coordinates and the point p. The new weight is the
  // sum of the old weight and the weight of p.
  void averageCoordinatesAndSumWeightsWith( const WeightedPoint2D<XT, YT, WT>& p ){
    double totalWeight = static_cast<double>(  w + p.weight()  );
    xVar = (w * xVar + p.weight() * p.x()) / totalWeight;
    yVar = (w * yVar + p.weight() * p.y()) / totalWeight;
    w = totalWeight;
  }
  friend void testWeightedPoint2D();
private:
  XT xVar;
  YT yVar;
  WT w;
};

template<typename XT, typename YT, typename WT>
std::ostream& operator<<( std::ostream& os, const WeightedPoint2D<XT, YT, WT>& point ){
  os << point.weight() << ": " << point.x() << ", " << point.y() << " ";
  return os;
}

}; // end namespace cbrc
#endif // defined _WEIGHTEDPOINT2D_HH_

