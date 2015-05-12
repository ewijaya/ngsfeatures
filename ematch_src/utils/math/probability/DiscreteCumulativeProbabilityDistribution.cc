/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.22
 *  Last Modified: $Date: 2007/10/20 03:27:16 $
 *  Description: See header file.
 */
#include "DiscreteCumulativeProbabilityDistribution.hh"

namespace cbrc{

DiscreteCumulativeProbabilityDistribution
::DiscreteCumulativeProbabilityDistribution( const DiscreteCumulativeProbabilityDistribution::arrayT& distribution,
					     long int seedval  /* 0 means choose random seed. */ )
  : distribution(distribution)
{
  // make sure distribution really looks like a cumulative probability distribution.
  for( size_t i = 1; i < size(); ++i ){
    assert( distribution[i] >= 0 );
    assert( distribution[i] <= 1.0 );
    assert( distribution[i] >= distribution[i] );
  }

  // set seed for random number generation.
  if( !seedval )  seedval = getpid() * (long) time( NULL );
  srand48( seedval );
}


size_t DiscreteCumulativeProbabilityDistribution::randomInt() const{
  size_t retval =
    std::upper_bound( distribution.begin(), distribution.end(), drand48() )
    - distribution.begin();
  return retval; // temporary.
}


std::ostream& operator<<( std::ostream& os, const DiscreteCumulativeProbabilityDistribution& cmldist ){
  for( size_t i = 0; i < cmldist.size(); i++ ){
    if( i ) os << ", ";
    os << cmldist(i);
  }
  return os;
}

} // end namespace cbrc

