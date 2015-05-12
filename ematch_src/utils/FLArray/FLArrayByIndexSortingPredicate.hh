/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.1.28
 *
 *  Last Modified: 2004.1.28
 *  
 *  Purpose: Provide a binary predicate for sorting an array of indices relative to another
 *           array.
 *
 *  Usage: FLArray<size_t> perm = FLArrayIdentityPermutation( b.size() );
 *         std::stable_sort( perm.begin(), perm.end(), FLArrayByIndexSortingPredicate<int>(b) );
 *
 *         sorts perm such that b[perm[0]] <= b[perm[1]] <= ...
 *
 */

#ifndef _FLARRAYBYINDEXSORTINGPREDICATE_HH_
#define _FLARRAYBYINDEXSORTINGPREDICATE_HH_
#include <functional>
#include "utils/FLArray/FLEArray.hh"
#include "utils/FLArray/FLArray.hh"

namespace cbrc{

inline FLEArray<size_t> FLArrayIdentityPermutation( size_t s ){
    FLEArray<size_t> ip( s );
    for( size_t i = 0; i < s; ++i ) ip[i] = i;
    return ip;
}

template < typename T, typename op=std::less<T> >
class FLArrayByIndexSortingPredicate{
public:
  FLArrayByIndexSortingPredicate( const FLArray<T>& a ) : a(a){}
  bool operator()( const size_t index0, const size_t index1 ) const{ 
    return op()( a[index0], a[index1] );
  }
private:
  const FLArray<T>& a;
};

template <typename T>
FLEArray<size_t> FLArraySortedPermutation( FLArray<T> a ){
  FLEArray<size_t> perm = FLArrayIdentityPermutation( a.size() );
  std::stable_sort( perm.begin(), perm.end(), FLArrayByIndexSortingPredicate<T>( a ) );
  return perm;
}

template <typename T, typename op>
FLEArray<size_t> FLArraySortedPermutation( FLArray<T> a ){
  FLEArray<size_t> perm = FLArrayIdentityPermutation( a.size() );
  std::stable_sort( perm.begin(), perm.end(), FLArrayByIndexSortingPredicate<T,op>( a ) );
  return perm;
}

#if 0 // this function doesn't need to be inlined. Move to a .cc file someday.
inline bool FLArrayValidPermutation( FLArray<size_t> perm ){
  FLArray<bool> valuePresent( perm.size(), false );
  for( size_t i = 0; i < perm.size(); ++i ){
    if( perm[i] >= perm.size() ) return false;
    valuePresent[i] = true;
  }
  for( size_t i = 0; i < perm.size(); ++i ){
    if( !valuePresent[i] ){
      std::cout << "value " << i << " missing\n";
      return false;
    }
  }
  return true;
}
#endif // unconditional

}; // end namespace
#endif // defined _FLARRAYBYINDEXSORTINGPREDICATE_HH_

