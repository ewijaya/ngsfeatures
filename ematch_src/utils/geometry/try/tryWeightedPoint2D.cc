/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *  Last Modified: $Date: 2007/02/07 01:58:36 $
 *  
 *  Purpose: try or run code involving WeightedPoint2D
 */
#include <iostream>
#include <vector>
#include "../WeightedPoint2D.hh"

namespace cbrc{
  void tryWeightedPoint2D(){
    WeightedPoint2D<double, double> point( 3.4, 5.7 );
    std::cout << "point is: " << point << std::endl;

    point.swapXY();
    std::cout << "after swapping x,y coordinates, point is: " << point << std::endl;

    WeightedPoint2D<double, double> p2( -12, 3.142 );
    std::cout << "p2 is: " << p2 << std::endl;

    std::vector< WeightedPoint2D<double, double> > v;
    v.push_back( WeightedPoint2D<double, double>( 8.5, 9.6 ) );
    v.push_back( WeightedPoint2D<double, double>( 7.3, 4.6 ) );
    v.push_back( WeightedPoint2D<double, double>( 2.3, 5.6 ) );
    v.push_back( WeightedPoint2D<double, double>( 3.3, 5.7 ) );

    std::sort( v.begin(), v.end(), WeightedPoint2D<double, double>::xAscending );
    std::cout << "sorted by x\n";
    for( size_t i = 0; i < v.size(); ++i ){
      std::cout << v[i] << "; ";
    }
    std::cout << std::endl;

    WeightedPoint2D<double, double> p3( 14, -3.5 );
    std::cout << "\np2 is " << p2 << std::endl;
    p2.averageCoordinatesAndSumWeightsWith( p3 );
    std::cout << "weighted average of p2 with " << p3 << " is " << p2 << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::tryWeightedPoint2D();
  return 1;
}

