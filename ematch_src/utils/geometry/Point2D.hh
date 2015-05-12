/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *  Last Modified: $Date: 2007/02/09 07:08:37 $
 *  
 *  Description: A two-dimensional point.
 *
 */

#ifndef _POINT2D_HH_
#define _POINT2D_HH_
#include "utils/math/moreNumericLimits.hh"

namespace cbrc{

template<typename XT, typename YT>
class Point2D{
public:
  Point2D(){}
  Point2D( XT x, YT y ) : xVar(x), yVar(y){}
  XT x() const{ return xVar; }
  YT y() const{ return yVar; }
  void set( XT x, YT y ){ xVar = x; yVar = y; }

  void swapXY(){  // assumes one can assign between types XT and YT.
    XT temp = xVar; xVar = yVar; yVar = temp;
  }

  static bool xLess( const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p0.x() < p1.x(); }

  
  class TolerantYCompare{
  public:
    TolerantYCompare( YT tolerance, bool testForFirstArgYLess = true ) 
      : tolerance(tolerance), _testForFirstArgYLess(testForFirstArgYLess) {}
    bool operator()( const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){
      if( _testForFirstArgYLess )  return( p0.y() < p1.y() + tolerance );
      else                     return( p0.y() + tolerance > p1.y() );
    }
    void testForFirstArgYLess(){ _testForFirstArgYLess = true; }
    void testForFirstArgYMore(){ _testForFirstArgYLess = false; }
  private:
    YT tolerance;
    bool _testForFirstArgYLess;
  };
  static bool yLess( const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p0.y() < p1.y(); }
  static bool xGreater( const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p0.x() > p1.x(); }
  static bool yGreater( const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p0.y() > p1.y(); }
  static bool xAscending(  const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p1.x() >= p0.x(); }
  static bool xDescending(  const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p1.x() <= p0.x(); }
  static bool yDescending( const Point2D<XT, YT>& p0, const Point2D<XT, YT>& p1 ){ return p1.y() <= p0.y(); }
  static bool nonFinite( const Point2D<XT, YT>& p ){
    //    if( p.x() == -std::numeric_limits<YT>::infinity() ) return true;
    if( p.x() == std::numeric_limits<XT>::infinity() ) return true;
    if(  moreNumericLimits::isNegativeInfinity<XT>( p.x() )  )  return true;
    if( p.y() == std::numeric_limits<YT>::infinity() ) return true;
    //    if( p.y() == -std::numeric_limits<YT>::infinity() ) return true;
    if(  moreNumericLimits::isNegativeInfinity<YT>( p.y() )  )  return true;
    return false;
  }
  void setX( XT x ){ xVar = x; }
  void setY( YT y ){ yVar = y; }
  void averageWith( const Point2D<XT, YT>& p ){
    std::cout << "xVar = " << xVar << " p.xVar = " << p.x();
    xVar = (xVar + p.x()) / 2;
    yVar = (yVar + p.y()) / 2;
    std::cout << " xVar = " << xVar << std::endl;
  }

  // "+" operator denotes vector addition.
  Point2D<XT,YT>& operator+=( const Point2D<XT,YT>& p ){
    xVar += p.x();
    yVar += p.y();
    return( *this );
  }
  Point2D<XT,YT> operator+( const Point2D& p ) const{
    return(  Point2D<XT,YT>( x() + p.x(), y() + p.y() )  );
  }

  Point2D<XT,YT> operator*( const YT scalar ) const{
    return(  Point2D<XT,YT>( x() * static_cast<XT>( scalar ), y() * scalar )  );
  }
  Point2D<XT,YT> operator/( const YT scalar ) const{
    return(  Point2D<XT,YT>( x() / static_cast<XT>( scalar ), y() / scalar )  );
  }
  friend void testPoint2D();
private:
  XT xVar;
  YT yVar;
};

template<typename XT, typename YT>
std::ostream& operator<<( std::ostream& os, const Point2D<XT,YT>& point ){
  os << point.x() << ", " << point.y() << " ";
  return os;
}

}; // end namespace
#endif // defined _POINT2D_HH_

