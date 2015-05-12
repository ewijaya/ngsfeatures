/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.7.23
 *
 *  Last Modified: 2004.7.23
 *  
 *  Description: A really simple linear time search map type class.
 *
 *  Historical Purpose: Provide a temporary replacement for a hash based map class
 *                      that was found to be buggy.
 *
 */

#ifndef _VECTORMAP_HH_
#define _VECTORMAP_HH_
#include <vector>
#include <algorithm>

namespace cbrc{

template <typename KT, typename VT>
class VectorMap{
public:
  bool has( const KT& key ) const{
    for( size_t i = 0; i < v.size(); ++i ){
      if( v[i].first == key ) return true;
    }
    return false;
  }
  size_t size() const{ return v.size(); }
  size_t getIndex( const KT& key ) const{
    size_t i;
    for( i = 0; i < v.size(); ++i ){
      if( v[i].first == key ) return i;
    }
    return i; // returning v.size() indicates the key was not found.
  }
  VT& operator[]( KT key ){ // adds element as necessary like map.
    size_t keyIndex = getIndex( key );
    if( keyIndex == size() ){
      VT dummyValue; // using default constructor.
      v.push_back(  std::make_pair( key, dummyValue )  );
    }
    return v[keyIndex].second;
  }
  std::vector< std::pair<KT, VT> > getVector() const{ return v; }
private:
  std::vector< std::pair<KT, VT> > v;
};

}; // end namespace cbrc
#endif // defined _VECTORMAP_HH_

