/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.28
 *  Last Modified: $Date: 2009/04/20 12:37:55 $
 *  
 *  Purpose: Provide a binary predicate for sorting an array of indices relative to another
 *           array.
 *
 *  Usage: FLEArray<size_t> perm = FLEArrayIdentityPermutation( b.size() );
 *         std::stable_sort( perm.begin(), perm.end(), FLEArrayByIndexSortingPredicate<int>(b) );
 *
 *         sorts perm such that b[perm[0]] <= b[perm[1]] <= ...
 *
 *  Note:  This file was created by simply copying FLArrayByIndexSortingPredicate, which predates
 *         the creation of FLEArray as a specialization of FLArray. At some point this should be
 *         restructured to allow cross talk between the two classes.
 *
 */

#ifndef _FLEARRAYBYINDEXSORTINGPREDICATE_HH_
#define _FLEARRAYBYINDEXSORTINGPREDICATE_HH_
#include <functional>
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

inline FLEArray<size_t> FLEArrayIdentityPermutation( size_t s ){
    FLEArray<size_t> ip( s );
    for( size_t i = 0; i < s; ++i ) ip[i] = i;
    return ip;
}

template < typename T, typename op=std::less<T> >
class FLEArrayByIndexSortingPredicate{
public:
  FLEArrayByIndexSortingPredicate( const FLEArray<T>& a ) : a(a){}
  bool operator()( const size_t index0, const size_t index1 ) const{ 
    return op()( a[index0], a[index1] );
  }
private:
  const FLEArray<T>& a;
};

template <typename T>
FLEArray<size_t> FLEArraySortedPermutation( FLEArray<T> a ){
  FLEArray<size_t> perm = FLEArrayIdentityPermutation( a.size() );
  std::stable_sort( perm.begin(), perm.end(), FLEArrayByIndexSortingPredicate<T>( a ) );
  return perm;
}

template <typename T, typename op>
FLEArray<size_t> FLEArraySortedPermutation( FLEArray<T> a ){
  FLEArray<size_t> perm = FLEArrayIdentityPermutation( a.size() );
  std::stable_sort( perm.begin(), perm.end(), FLEArrayByIndexSortingPredicate<T,op>( a ) );
  return perm;
}

#if 0 // this function doesn't need to be inlined. Move to a .cc file someday.
inline bool FLEArrayValidPermutation( FLEArray<size_t> perm ){
  FLEArray<bool> valuePresent( perm.size(), false );
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
#endif // defined _FLEARRAYBYINDEXSORTINGPREDICATE_HH_
