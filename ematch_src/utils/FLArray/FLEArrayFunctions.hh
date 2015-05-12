/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2003-2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.7.22
 *  Last Modified: $Date: 2007/12/11 10:17:51 $
 *
 *  Description: Various functions which use FLEArray.
 *
 */

#ifndef _FLEARRAYFUNCTIONS_HH_
#define _FLEARRAYFUNCTIONS_HH_

#include "utils/gdb/gdbUtils.hh"
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

namespace FLEArrayFunctions {

  /* ********** Functions common to FLArrayFunctions ********** */

  // Pick random element by uniform distribution
  // If called with non-zero callerGivenSeed reseed with that value
  // Otherwise seed only on first call
  template<typename T>
  const T& randomElement( const FLEArray<T> a, long int callerGivenSeed = 0 ){
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



  /* ********** Functions unique to FLEArrayFunctions ********** */
  // upon return numCombinationsVector[i] holds n choose i, where n = numCombinationsVector.size() - 1.
  template<typename elemType>
  void setNumCombinationsVector( FLENumArray<elemType>& numCombinationsVector ){
  
    numCombinationsVector[0] = 1;
    unsigned int n = numCombinationsVector.size() - 1;
    for( unsigned int i = 1; i <= n; ++i ){
      numCombinationsVector[i] = numCombinationsVector[i-1] * (n-i+1) / i;
    }
  }

  // Construct a double type FLENumArray from a vector of strings and range.
  // Note: this may someday be found in toDouble()
  FLENumArray<double> doubleFLENumArrayFromStrings( std::vector<std::string> asStrings, size_t start, size_t end ){
    assert( end <= asStrings.size() );
    assert( end > start );
    FLENumArray<double> newArray( end - start );
    for( size_t i = start; i < end; ++i ){
      newArray[i-start] = strtod( asStrings[i].c_str(), NULL );
    }
    return newArray;
  }

  // Vector subtraction. Set a to b-c. 
  template<typename elemType>
  void diff( FLENumArray<elemType>& a, const FLENumArray<elemType>& b, const FLENumArray<elemType>c ){
    // assert( a.size() <= b.size(); assert( a.size() <= c.size() )
    for( size_t i = 0; a.size(); ++i ){
      a[i] = b[i] - c[i];
    }
  }

  // set a to cumulative sum of b.
  template <typename T>
  void cumulativeSum( FLENumArray<T>& a, const FLENumArray<T>& b ){
    if( !b.size() )  return;
    if( !a.size() )  a.setSize( b.size() );
    GDB_ASSERT( a.size() == b.size() );
    a[0] = b[0];
    for( size_t i = 1; i < b.size(); ++i )  a[i] = a[i-1] + b[i];
  }

  // normalize elements of a to add to sum.
  template <typename T>
  void normalizeSum( FLENumArray<T>& a, double sumAfterNormalization = 1.0 ){
    double sum = (double) a.sum();
    double ratio = sumAfterNormalization / sum;
    for( size_t i = 0; i < a.size(); ++i )  a[i] = a[i] * ratio;
  }
};

}; // end namespace
#endif // defined _FLEARRAYFUNCTIONS_HH_

