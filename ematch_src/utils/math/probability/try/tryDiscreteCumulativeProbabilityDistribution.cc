/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.22
 *  Last Modified: $Date: 2006/09/22 08:14:16 $
 *
 *  Purpose: try or run code involving DiscreteCumulativeProbabilityDistribution
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../DiscreteCumulativeProbabilityDistribution.hh"

namespace cbrc{
  void tryDiscreteCumulativeProbabilityDistribution(){
    double va[] = {0.5, 0.7, 0.9};
    FLENumArray<double> v( 3, va );
    DiscreteCumulativeProbabilityDistribution cmldist( v );
    std::cout << "cumulative distribution is: " << cmldist << std::endl;

    for( size_t i = 0; i < 9; ++i ){
      std::cout << "random int is: " << cmldist.randomInt() << std::endl;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  args.dieIfUnusedArgs();
  cbrc::tryDiscreteCumulativeProbabilityDistribution();
  return 1;
}

