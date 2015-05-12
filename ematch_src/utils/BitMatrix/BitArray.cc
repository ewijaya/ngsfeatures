/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.24
 *  Last Modified: $Date: 2008/02/24 01:43:00 $
 *  Description: See header file.
 */
#include "BitArray.hh"

namespace cbrc{


BitArray::BitArray( const size_t& _size ) : _size(_size){
  // compute size in bytes
  sizeInBytes = size() / BITARRAY_BITS_PER_BYTE;
  if( size() != sizeInBytes * BITARRAY_BITS_PER_BYTE ) ++sizeInBytes;

  // allocate memory
  a = new byte[sizeInBytes];
}



void BitArray::setSize( const size_t& _size ){

  this->_size = _size;

  // assert array has not be sized before
  assert( !a ); 
    
  // compute size in bytes.
  sizeInBytes = size() / BITARRAY_BITS_PER_BYTE;
  if( size() != sizeInBytes * BITARRAY_BITS_PER_BYTE ) ++sizeInBytes;

  // allocate memory
  a = new byte[sizeInBytes];
}



BitArray& BitArray::operator=( const BitArray& ba ){
  assert( ba.a );
  if( a ){
    assert( sizeInBytes = ba.sizeInBytes );
  }else{
    sizeInBytes = ba.sizeInBytes;
    a = new byte[sizeInBytes];
  }
  memcpy( a, ba.a, sizeInBytes );
  return *this;
}



bool BitArray::validIndex( const size_t& index ) const{
  if( index >= size() ){
    std::cerr << "** index: " << index << " >= size: " << size() << std::endl << std::flush;
    return false;
  }
  return true;
}


void BitArray::trueIterator::operator++(){
      byte inByteBitIndex = bitIndex & static_cast<byte>(7);
      if(  (inByteBitIndex == 7) || !(ba.a[byteIndex] & BitArrayCumInverseMask[inByteBitIndex] )  ){
	for( ++byteIndex; (byteIndex < ba.sizeInBytes) && (!ba.a[byteIndex]); ++byteIndex );
	if( byteIndex >= ba.sizeInBytes ){ bitIndex = ba.size(); return; }
	inByteBitIndex = 0;
      }else{
	++inByteBitIndex;
      }
      switch( inByteBitIndex ){ // Purposefully falling through all conditions.
      case 0:
	if( ba.a[byteIndex] & 0x01 ){ inByteBitIndex = 0; break; }
      case 1:
	if( ba.a[byteIndex] & 0x02 ){ inByteBitIndex = 1; break; }
      case 2:
	if( ba.a[byteIndex] & 0x04 ){ inByteBitIndex = 2; break; }
      case 3:
	if( ba.a[byteIndex] & 0x08 ){ inByteBitIndex = 3; break; }
      case 4:
	if( ba.a[byteIndex] & 0x10 ){ inByteBitIndex = 4; break; }
      case 5:
	if( ba.a[byteIndex] & 0x20 ){ inByteBitIndex = 5; break; }
      case 6: 
	if( ba.a[byteIndex] & 0x40 ){ inByteBitIndex = 6; break; }
      case 7: 
	if( ba.a[byteIndex] & 0x80 ){ inByteBitIndex = 7; break; }
      default:
	std::cout << "*** TROUBLE *** " << "a[byteIndex] = " << (int) ba.a[byteIndex] << " inByteBitIndex " << (int) inByteBitIndex << " byteIndex: " << byteIndex << std::endl;
      }
      bitIndex = byteIndex*BITARRAY_BITS_PER_BYTE + inByteBitIndex;
      if( bitIndex > ba.size() ) bitIndex = ba.size();

} // end BitArray::trueIterator::operator++



BitArray::trueIterator BitArray::trueBegin() const{
  trueIterator iter( *this );
  for( iter.byteIndex = 0;
       (iter.byteIndex < sizeInBytes) && !a[iter.byteIndex];
       ++iter.byteIndex );

  if( iter.byteIndex >= sizeInBytes )  return trueEnd();

  byte curByte = a[iter.byteIndex];
  byte inByteBitIndex =
    curByte & 0x01 ? 0 :
    curByte & 0x02 ? 1 :
    curByte & 0x04 ? 2 :
    curByte & 0x08 ? 3 :
    curByte & 0x10 ? 4 :
    curByte & 0x20 ? 5 :
    curByte & 0x40 ? 6 :
    curByte & 0x80 ? 7 :
    /* default */ IMPOSSIBLE_VALUE;
  assert( inByteBitIndex != IMPOSSIBLE_VALUE );

  iter.bitIndex = iter.byteIndex * BITARRAY_BITS_PER_BYTE + inByteBitIndex;
  if( iter.bitIndex > size() ) return trueEnd();
  return iter;
} // method trueBegin


} // end namespace cbrc

