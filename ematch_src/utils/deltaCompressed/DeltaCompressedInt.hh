/*
 *  Author: Paul B. Horton
 *
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.8
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: Compress a sorted list of ints into a more compact list using delta encoding.
 *
 *  Caveat: Although defined as a template class this will only work as is with 4 byte ints (int & unsigned int)
 *
 *  Data Representation: The first int is simply stored. For the following ints the difference is stored.
 *                       i.e. for input v[0...], v[1]-v[0] is stored instead of explicitly storing v[1].
 *                       The difference is stored as a variable length record similar to euc encoding.
 *                       If the difference is < 2^7 one byte is used. If the difference is less than 2^14
 *                       two bytes are used, etc. up to a maximum of five bytes.
 *
 *  Usage: DelaCompressedInt<int> dc( std::vector<int> v ) // v must be sorted in ascending order
 *         int val = dc.first();
 *	   for( size_t dcArrayIndex = 0; ; val += dc.nextDelta( dcArrayIndex ) ){
 *           if( dcArrayIndex > 0 ) os << " ";
 *           // Do something with val.
 *           // ...
 *           if( dcArrayIndex >= dc.arraySize() ) break;  // Achtung! Do not forget this!
 *        }
 *
 *
 */
#ifndef _DELTACOMPRESSEDINT_HH_
#define _DELTACOMPRESSEDINT_HH_
#include "motifDiscovery/utilities/FLArray/FLArray.hh"
#include <assert.h>
#include <vector>

namespace cbrc{

template <typename intType>
class DeltaCompressedInt{
public:
  // input vector must be in ascending sorted order. Cannot call with empy vector.
  DeltaCompressedInt<intType>( const std::vector<intType>& v ){
    sizeVar = v.size();
    arraySizeVar = computeArraySize( v );
    a.setSize(arraySizeVar);
    size_t s = 0;
    firstVar = v[0];
    for( size_t i = 1; i < sizeVar; ++i, ++s ){
      unsigned int diff = v[i] - v[i-1];
      a[s] = diff & 0x7F;

      if( diff <= 0x7F ) continue;
      a[s++] |= 0x80;
      diff = diff >> 7;
      a[s] = diff & 0x7F;

      if( diff <= 0x7F ) continue;
      a[s++] |= 0x80;
      diff = diff >> 7;
      a[s] = diff & 0x7F;

      if( diff <= 0x7F ) continue;
      a[s++] |= 0x80;
      diff = diff >> 7;
      a[s] = diff & 0x7F;

      if( diff <= 0x7F ) continue;
      a[s++] |= 0x80;
      diff = diff >> 7;
      a[s] = diff;
    }
  }
  intType first() const{ return firstVar; }

  // next modifies argument! call with 0 to get first delta. returns arraySize() if no more values are left.
  intType nextDelta( size_t& arrayIndex ) const{
    intType delta = a[arrayIndex] & 0x7F; 
    if( !(a[arrayIndex++] & 0x80) ) return delta;

    
    delta |= ( (a[arrayIndex] & 0x7F) << 7 );
    if( !(a[arrayIndex++] & 0x80) ) return delta;

    delta |= ( (a[arrayIndex] & 0x7F) << 14 );
    if( !(a[arrayIndex++] & 0x80) ) return delta;

    delta |= ( (a[arrayIndex] & 0x7F) << 21 );
    if( !(a[arrayIndex++] & 0x80) ) return delta;

    delta |= ( (a[arrayIndex] & 0x7F) << 28 );
    ++arrayIndex;
    return delta;
  }
  bool operator==( const std::vector<intType>& v ){
    if( v.size() != sizeVar ) return false;
    intType val = first();
    size_t vIndex = 0;
    for( size_t dcArrayIndex = 0; ; val += nextDelta( dcArrayIndex ) ){
      if( val != v[vIndex++] ) return false;
      if( dcArrayIndex >= arraySize() ) break;
    }
    return true;
  }

  size_t size() const{ return sizeVar; }
  size_t arraySize() const{ return arraySizeVar; }
private:
  size_t computeArraySize( const std::vector<intType>& v ){
    assert( v.size() > 0 );
    size_t s = 0;
    for( size_t i = 1; i < v.size(); ++i ){
      assert( v[i] > v[i-1] );
      unsigned int diff = v[i] - v[i-1];
      ++s;
      if( diff > 0x7F ) ++s;
      if( diff > 0x3FFF ) ++s;
      if( diff > 0x1FFFFF ) ++s;
      if( diff > 0xFFFFFFF ) ++s;
    }
    return s;
  }
  size_t sizeVar;
  size_t arraySizeVar;
  intType firstVar;
  FLArray<unsigned char> a;
};


template <typename intType>
std::ostream &operator<<( std::ostream &os, const DeltaCompressedInt<intType>& dc ){
  intType val = dc.first();
  for( size_t dcArrayIndex = 0; ; val += dc.nextDelta( dcArrayIndex ) ){
    if( dcArrayIndex > 0 ) os << " ";
    os << val;
    if( dcArrayIndex >= dc.arraySize() ) break;
  }
  os << std::endl;
  return os;
}
}; // end namespace
#endif // defined _DELTACOMPRESSEDINT_HH_

