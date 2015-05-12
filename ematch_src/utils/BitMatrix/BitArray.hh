/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.5
 *  
 *  Purpose: Provide an efficient bit array.
 *
 *  Usage: The operator [] is overridden to access elements. BitArray[i] returns the bit at position i.
 *         However BitArray[] is NOT a valid lvalue. To asign values use BitArray.set( size_t i, bool val ) or
 *         just BitArray.set( i ) to set to 1 and BitArray.clear( i ) to set to 0.
 *         Normally the index i is checked for overflow. However there are two ways
 *         to avoid this small overhead -- one is to use the methods clearFast, and setFast. 
 *
 *         The more complete way to avoid this overhead however is to use the derived class BitArrayFast. This
 *         class does not check indices for overflow unless (for debugging) the macro 
 *         BITMATRIX_ALWAYS_CHECK_INDICES is defined.
 *
 *         You may iterator over true values using a BitArray::trueIterator iterator
 *         with trueBegin() and trueEnd().
 *
 *         This iterator should be relatively fast for sparse (few true values) arrays
 *         because it can move forward across long streches of 0s (false values) one byte
 *         (eight values) at a time rather than one value at a time.
 *
 *  Innards: There is an issue of where to place bits in relative to bytes. Below we show an
 *           example of how our implementation uses memory for a bit array of size (in bits) 10.
 *           The digits 0-9 show the positions of bits 0-9
 *           (as accessed with operator[] for example) and "x" marks unused bit positions.
 *
 *           In this case, a is of size two, the 16 bits starting at *a are: 7654321098xxxxxx
 *
 *           In otherwords indices count down within a byte and count up between bytes.
 *
 *  Initial Value: Constructor and setSize do not initialize contents.
 */
#ifndef _BITARRAY_HH_
#define _BITARRAY_HH_
#include <assert.h>
#include <string.h>
#include <iostream>
#define BITARRAY_BITS_PER_BYTE 8
#define IMPOSSIBLE_VALUE BITARRAY_BITS_PER_BYTE+1


namespace cbrc{

typedef unsigned char byte;

const byte BitArrayMask[] =        {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const byte BitArrayInverseMask[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
const byte BitArrayCumInverseMask[] = {0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80};


class BitArray{
public:
  /* ***** constructors ***** */
  BitArray(){ a = NULL; sizeInBytes = _size = 0; }

  BitArray( const size_t& size );

  BitArray( const BitArray& ba ){
    sizeInBytes = ba.sizeInBytes;
    a = new byte[sizeInBytes];
    memcpy( a, ba.a, sizeInBytes );
  }

  virtual ~BitArray(){ if( a ) delete []a; }

  void setSize( const size_t& size );

  BitArray& operator=( const BitArray& ba );

  void fill( const bool& fillVal ){
    if( fillVal )  memset( a, 255, sizeInBytes );
    else           memset( a,   0, sizeInBytes );
  }

  void zero(){  memset( a, 0, sizeInBytes );  }

  void fillWithByte( const int& fillByte ){
    memset( a, fillByte, sizeInBytes );
  }

  bool validIndex( const size_t& index ) const;

  bool operator==( const BitArray& ba ) const{
    if( ba.size() != size() )  return false;
    return(  !memcmp( a, ba.a, sizeInBytes )  );
  }

  virtual bool operator[] ( const size_t& index ) const{
    assert(  validIndex( index )  );
    size_t byteIndex = index >> 3;
    size_t bitIndex (index & static_cast<byte>(7));
    return( a[byteIndex] & BitArrayMask[bitIndex] );
  }

  virtual void set( const size_t& index, const bool& val ) const{
    assert(  validIndex( index )  );
    if( val ) setFast( index );
    else      clearFast( index );
  }

  virtual void set( const size_t& index ) const{
    assert(  validIndex( index )  );
    size_t byteIndex = index >> 3;
    size_t bitIndex = (index & static_cast<byte>(7));
    a[byteIndex] |= BitArrayMask[bitIndex];
  }

  void setFast( const size_t& index ) const{
    size_t byteIndex = index >> 3;
    size_t bitIndex = (index & static_cast<byte>(7));
    a[byteIndex] |= BitArrayMask[bitIndex];
  }

  virtual void clear( const size_t& index ) const{
    assert(  validIndex( index )  );
    size_t byteIndex = index >> 3;
    size_t bitIndex = (index & static_cast<byte>(7));
    a[byteIndex] &= BitArrayInverseMask[bitIndex];
  }

  void clearFast( const size_t& index ) const{
    size_t byteIndex = index >> 3;
    size_t bitIndex = (index & static_cast<byte>(7));
    a[byteIndex] &= BitArrayInverseMask[bitIndex];
  }


  /* ***** define sub-class trueIterator ***** */
  class trueIterator{
  public:
    trueIterator( const BitArray& ba ) : ba(ba){}
    operator size_t() const{ return bitIndex; }
    void operator++();
    bool operator!=( const trueIterator& it ) const{ return bitIndex != it.bitIndex; }
  public:  // really private.
    const BitArray& ba;
    size_t bitIndex;
    size_t byteIndex;
  };


  /* ***** methods for iterating with trueIterator ***** */
  trueIterator trueEnd() const{
    trueIterator it( *this ); 
    it.bitIndex = size();
    return it;
  }

  trueIterator trueBegin() const;

  // accessors
  size_t size() const{ return _size; }
  size_t getSizeInBytes() const{ return sizeInBytes; }
protected:
  // object data
  size_t _size; // size in bits.
  size_t sizeInBytes; // size in bytes.
  byte* a;
}; // end class BitArray


class BitArrayFast : public BitArray{
  BitArrayFast() : BitArray(){}
  BitArrayFast( const size_t sz ) : BitArray( sz ){}
  BitArrayFast( const BitArray& ba ) : BitArray( ba ){}
  virtual bool operator[] ( const size_t index ) const{
#ifdef BITARRAY_ALWAYS_CHECK_INDICES
    assert(  validIndex( index )  );
#endif // defined BITARRAY_ALWAYS_CHECK_INDICES
    size_t byteIndex = index >> 3;
    size_t bitIndex (index & static_cast<byte>(7));
    return( a[byteIndex] & BitArrayMask[bitIndex] );
  }
  virtual void set( const size_t index, bool val ) const{
#ifdef BITARRAY_ALWAYS_CHECK_INDICES
    assert(  validIndex( index )  );
#endif // defined BITARRAY_ALWAYS_CHECK_INDICES
    if( val ) setFast( index );
    else      clearFast( index );
  }
  virtual void set( const size_t index ) const{
#ifdef BITARRAY_ALWAYS_CHECK_INDICES
    assert(  validIndex( index )  );
#endif // defined BITARRAY_ALWAYS_CHECK_INDICES
    size_t byteIndex = index >> 3;
    size_t bitIndex = (index & static_cast<byte>(7));
    a[byteIndex] |= BitArrayMask[bitIndex];
  }
  virtual void clear( const size_t index ) const{
#ifdef BITARRAY_ALWAYS_CHECK_INDICES
    assert(  validIndex( index )  );
#endif // defined BITARRAY_ALWAYS_CHECK_INDICES
    size_t byteIndex = index >> 3;
    size_t bitIndex = (index & static_cast<byte>(7));
    a[byteIndex] &= BitArrayInverseMask[bitIndex];
  }
};


inline std::ostream &operator<<( std::ostream &os, const BitArray& ba ){
  for( size_t i = 0; i < ba.size(); ++i ){
    os  <<  ( ba[i] ? "1" : "0" );
  }
  os << std::endl;
  return os;
}


class BitArrayStack : public BitArray{
public:
  BitArrayStack() : BitArray(){ topIndex = 0; }
  BitArrayStack( const size_t sz ) : BitArray( sz ){ topIndex = 0; }
  BitArrayStack( const BitArrayStack& bas ) : BitArray( bas ){ topIndex = 0; }
  void push( bool val ){ set( topIndex++, val ); }
  bool empty() const{ return( topIndex == 0 ); }
  bool pop(){ return (*this)[--topIndex]; }
private:
  size_t topIndex;
};


} // end namespace cbrc
#endif // defined _BITARRAY_HH_

