/*
 *  Author: Please set CBRC_AUTHOR environment variable
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.6.28
 *
 *  Last Modified: 2004.6.28
 *  
 *  Purpose: test StayFiniteLog.
 */
#include <iostream>
#include "../StayFiniteLog.hh"

namespace cbrc{
  void testStayFiniteLog(){
    StayFiniteLog fl;
    for( double x = 0.0001; x > 0; x /= 2.0 ){
      std::cout << "x: " << x << " log(x): " << log(x) << std::endl;
    }
    std::cout << "log(x) : " << log(0) << " stayFiniteLog(0): " << fl(0) << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::testStayFiniteLog();
  return 1;
}

