/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *  Last Modified: $Date: 2007/02/07 01:30:09 $
 *  
 *  Purpose: try or run code involving Point2D
 */
#include <iostream>
#include <vector>
#include "../Point2D.hh"

namespace cbrc{
  void tryPoint2D(){
    Point2D<double, double> pA( 3.4, 5.7 );
    std::cout << "pA is: " << pA << std::endl;

    Point2D<double, double> pB( 3.4, 4.4 );
    std::cout << "pB is: " << pB << std::endl;
    std::cout << "pA + pB is: " << pA + pB << std::endl;

    std::cout << "pA * 3 = " << pA * 3 << std::endl;
    std::cout << "pA / 5 = " << pA / 5 << std::endl;


    pA.swapXY();
    std::cout << "after swapping x,y coordinates, point is: " << pA << std::endl;

    Point2D<int, double> p2( -12, 3.1412 );
    std::cout << "p2 is: " << p2 << std::endl;

#warning "Code Author: It is normal for the compiler to warn about this swapXY between int and double types"
    p2.swapXY();
    std::cout << "after swapping p2 is: " << p2 << std::endl;

    std::vector< Point2D<double, double> > v;
    v.push_back( Point2D<double, double>( 8.5, 9.6 ) );
    v.push_back( Point2D<double, double>( 7.3, 4.6 ) );
    v.push_back( Point2D<double, double>( 2.3, 5.6 ) );
    v.push_back( Point2D<double, double>( 3.3, 5.7 ) );

    std::sort( v.begin(), v.end(), Point2D<double, double>::xAscending );
    std::cout << "sorted by x\n";
    for( size_t i = 0; i < v.size(); ++i ){
      std::cout << v[i] << "; ";
    }
    std::cout << std::endl;

    Point2D<int, double> p3( 14, -3.5 );
    p2.averageWith( p3 );
    std::cout << "average of p2 with " << p3 << " is " << p2 << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::tryPoint2D();
  return 1;
}

