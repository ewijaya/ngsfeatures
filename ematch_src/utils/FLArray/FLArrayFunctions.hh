/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.12.11
 *  Last Modified: $Date: 2007/12/11 10:03:36 $
 *
 *  Description: Various functions which use FLArray.
 *
 *               Many of these could have been made into methods for FLArray,
 *               but were placed here to avoid the overhead linking them to
 *               every program which includes FLArray.hh
 *
 */

#ifndef _FLARRAYFUNCTIONS_HH_
#define _FLARRAYFUNCTIONS_HH_

#include "utils/FLArray/FLArray.hh"

namespace cbrc{

namespace FLArrayFunctions {


  // Pick random element by uniform distribution
  // If called with non-zero callerGivenSeed reseed with that value
  // Otherwise seed only on first call
  template<typename T>
  const T& randomElement( const FLArray<T> a, long int callerGivenSeed = 0 ){
    static long int seed = 0;

    /* ***** Seed if necessary ***** */
    if( callerGivenSeed ){
      seed = callerGivenSeed;
      srand48( seed );
    }else if( !seed ){
      seed = getpid() * (long) time( NULL );
      srand48( seed );
    }

    size_t randomIndex = static_cast<long int>( drand48() * a.size() );
    return(    a[ randomIndex ]   );
  };


}; // end namespace FLArrayFunctions

}; // end namespace cbrc
#endif // defined _FLARRAYFUNCTIONS_HH_

