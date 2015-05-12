/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.5
 *  Last Modified: $Date: 2008/03/05 21:37:42 $
 *
 *  Description: try using FLENumArray in conjunction with stlUtil transformAll function
 */
#include <cmath>
#include <iostream>
#include <algorithm>
#include <functional>
#include "../FLEArray.hh"
#include "utils/stl/stlUtils.hh"

namespace cbrc{
  void FLENumArray_sqrtTransform(){
    FLENumArray<double> a( 10, 6 );
    std::cout << "a is:\n" << a << std::endl;

    stlUtils::transformAll( a, sqrt );
    std::cout << "now a is:\n" << a << std::endl;
  }
} // end namescape cbrc


int main(){
  cbrc::FLENumArray_sqrtTransform();
  return 1;
}

