/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.12.19
 *
 *  Last Modified: $Date: 2005/04/15 07:41:28 $
 *  
 *  Description: List of up to k best items of a series of items. Updated online.
 *
 *  Special Note: This class appears to have never been tested and used. Until it is
 *                finished try the similar class TopKSorted.hh
 *
 */

#ifndef _TOPKUNIQUE_HH_
#define _TOPKUNIQUE_HH_
#include <set>

namespace cbrc{

template<typename T, typename BinaryPredicate = std::greater<T> >
class TopKUnique{
public:
  TopKUnique( size_t k ){ assert( k > 0 ); this->k = k; }
  void insert( T val ){
    if( size() < k ){
      s.insert( val );
      if( size() == k ) minAcceptableVal = *( minPos() );
      return;
    }
    if( !cmp( val, minAcceptableVal ) ) return;
    s.insert( val );
    if( size() > k ){
      s.erase( minPos() );
      minAcceptableVal = *( minPos() );
    }
  }
  T maxVal() const{ return *s.begin(); }
  T minVal() const{ return *( minPos() ); }
  const std::set<T, BinaryPredicate>& getSet() const{ return s; }
  size_t size() const{ return s.size(); }
private:
  std::set<T, BinaryPredicate>::iterator minPos() const{
    std::set<T, BinaryPredicate>::iterator last = s.end();
    return( --last );
  }
  size_t k;
  T minAcceptableVal;
  BinaryPredicate cmp;
  std::set<T, BinaryPredicate> s;
};

}; // end namespace
#endif // defined _TOPKUNIQUE_HH_

