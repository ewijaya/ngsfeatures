/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.10.17
 *
 *  Last Modified: $Date: 2005/10/17 09:32:38 $
 *  
 *  Description: Container for a hash with a size_t type key and variable type value.
 *
 *  Purpose: Created to test and demonstrate the use of ChainHashtable.
 *
 */

#ifndef _HASHABLESIZETSIZET_HH_
#define _HASHABLESIZETSIZET_HH_

namespace cbrc{

template<typename T>
class HashableSizeTKey{
public:
  HashableSizeTKey(){}
  HashableSizeTKey( const size_t first ) : first(first) {}
  HashableSizeTKey( const size_t first, const T second ) : first(first), second(second) {}
  ~HashableSizeTKey(){}
  size_t hashVal() const{ return first; }
  bool keyEquals( const HashableSizeTKey& p ) const{
    return( p.first == first );
  }
  friend void testHashableSizeTKey();
  size_t first;
  T second;
};

}; // end namespace cbrc
#endif // defined _HASHABLESIZETSIZET_HH_

