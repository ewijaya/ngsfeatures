/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *
 *  Last Modified: $Date: 2007/02/09 08:16:56 $
 *
 *  Purpose: try or run code involving monotonic smoothing with smoothing.hh
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include "../smoothing.hh"


namespace cbrc{
  void tryMonotonicSmoother(){
    std::vector<smoothing::NPOINT> v;
    v.push_back( smoothing::NPOINT( 1, 1 ) );
    v.push_back( smoothing::NPOINT( 1, 0 ) );
    v.push_back( smoothing::NPOINT( 1, 1 ) );
    v.push_back( smoothing::NPOINT( 2, 1 ) );
    v.push_back( smoothing::NPOINT( 3, 0 ) );
    v.push_back( smoothing::NPOINT( 4, 1 ) );
    v.push_back( smoothing::NPOINT( 5, 0 ) );
    v.push_back( smoothing::NPOINT( 6, 0 ) );

    std::cout << "before smoothing\n";
    for( size_t i = 0; i < v.size(); ++i ){
      std::cout << v[i] << "  ";
    }
    std::cout << std::endl;

    
    std::vector<smoothing::NPOINT> vOrig( v );

    v = vOrig;
    v = smoothing::mergeIdenticalAdjacentX( v );
    std::cout << "after merging adjacent Y\n";
    for( size_t i = 0; i < v.size(); ++i ){
      std::cout << v[i] << "  ";
    }
    std::cout << std::endl;
    std::vector<smoothing::POINT> smoothed = smoothing::monotonicSmoothY( v, false, 2 );

    std::cout << "after smoothing Y\n";
    for( size_t i = 0; i < smoothed.size(); ++i ){
      std::cout << smoothed[i] << "  ";
    }
    std::cout << std::endl;
  }


}; // end namescape cbrc

int main(){
  cbrc::tryMonotonicSmoother();
  return 1;
}
