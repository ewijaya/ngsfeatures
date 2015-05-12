/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.22
 *  Last Modified: $Date: 2007/10/20 03:29:18 $
 *
 *  Description: Container to represent a cumulative probability distribution
 *               over the integers 0,...,size()-1.
 *
 */
#ifndef _DISCRETECUMULATIVEPROBABILITYDISTRIBUTION_HH
#define _DISCRETECUMULATIVEPROBABILITYDISTRIBUTION_HH
#include <iostream>
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

class DiscreteCumulativeProbabilityDistribution{
public:
  typedef FLENumArray<double> arrayT;

  // distribution[i] should represent the cumulative probability of 0...i
  //
  // to generate integers from 0..n, distribution should have size n.  NOT n+1.
  // for example distribution = {0.25, 0.5, 0.75} stipulates a uniform distribution over {0,1,2,3}.
  DiscreteCumulativeProbabilityDistribution( const arrayT& distribution,
					     long int seedval = 0 /* 0 means choose random seed. */  );

  const size_t& size() const{ return distribution.size(); }
  size_t randomInt() const; // pick integer from 0 to size()-1 according to distribution.
  const double& operator()( const size_t i ) const{ return distribution[i]; }
  private:
    const arrayT distribution;
};

std::ostream& operator<<( std::ostream& os, const DiscreteCumulativeProbabilityDistribution& cmldist );


} // end namespace cbrc
#endif // _DISCRETECUMULATIVEPROBABILITYDISTRIBUTION_HH
