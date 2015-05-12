/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Creation Date: 2003.5.30
 *  Last Modification Date: $Date: 2009/10/08 01:54:39 $
 *  Copyright: Paul B. Horton
 *  License: All rights reserved
 *  
 *  Purpose: Provide an efficient bit matrix. 
 *
 *  Usage: The operator () is overridden to access elements. BitMatrix( i, j ) returns the bit at i, j
 *         If just one argument is given BitMatrix( j ) returns the bit at the jth position of the current
 *         column. Thus BitMatrix.setCurCol( i ); BitMatrix( j ) returns the same bit as BitMatrix( i, j ).
 *
 *         Normally the indices i, j in these examples are checked for overflow. However there are two ways
 *         to avoid this small overhead. One is to use the methods clearFast, and setFast. 
 *
 *         The more complete way to avoid this overhead however is to use the derived class BitMatrixFast. This
 *         class does not check indices for overflow unless (for debugging) the macro 
 *         BITMATRIX_ALWAYS_CHECK_INDICES is defined.
 *
 *  Initial Value: After constructing a non-empty BitMatrix or using setSize(), all elements of the matrix are
 *                 initialized to false.
 *
 */
#ifndef _BITMATRIX_HH_
#define _BITMATRIX_HH_
#include <assert.h>
#include <iostream>
#include <cstring>
#define BITMATRIX_BITS_PER_BYTE 8

namespace cbrc{


typedef unsigned char byte;

// const byte BitMatrixMask[] = {1, 2, 4, 8, 16, 32, 64, 128};
// const byte BitMatrixInverseMask[] = {254, 253, 251, 247, 239, 223, 191, 127};
const byte BitMatrixMask[] = {128, 64, 32, 16, 8, 4, 2, 1};
const byte BitMatrixInverseMask[] = {127, 191, 223, 239, 247, 251, 253, 254};


class BitMatrix {
public:
  BitMatrix(){ m = NULL; _sizeInBytes = s0 = s1 = 0; }
  BitMatrix( const size_t sz0, const size_t sz1 ) : s0(sz0), s1(sz1){
    bytesPerCol = s1/BITMATRIX_BITS_PER_BYTE;
    if( s1 != bytesPerCol * BITMATRIX_BITS_PER_BYTE ) ++bytesPerCol;
    _sizeInBytes = bytesPerCol * s0;
    m = new byte[_sizeInBytes];
    colPtr = m;
    zero(); // this is important so that the use of memcmp for operator== will be correct.
  }
  BitMatrix( const BitMatrix& bm ){
    s0 = bm.s0; s1 = bm.s1; bytesPerCol = bm.bytesPerCol; _sizeInBytes = bm.sizeInBytes();
    m = new byte[_sizeInBytes];
    colPtr = m;
    memcpy( m, bm.m, _sizeInBytes );
  }
  virtual ~BitMatrix(){ delete m; }
  void setSize( size_t sz0, size_t sz1 ){
    s0 = sz0; s1 = sz1;
    assert( !m );
    bytesPerCol = s1/BITMATRIX_BITS_PER_BYTE;
    if( s1 % bytesPerCol*BITMATRIX_BITS_PER_BYTE ) ++bytesPerCol;
    _sizeInBytes = bytesPerCol * s0;
    m = new byte[_sizeInBytes];
    colPtr = m;
    zero(); // this is important so that the use of memcmp for operator== will be correct.
  }
  BitMatrix operator=( const BitMatrix& bm ){
    assert( bm.m );
    if( m ){
      assert( _sizeInBytes == bm.sizeInBytes() );
    }else{
      s0 = bm.s0; s1 = bm.s1; bytesPerCol = bm.bytesPerCol; _sizeInBytes = bm.sizeInBytes();
      m = new byte[_sizeInBytes];
      colPtr = m;
    }
    memcpy( m, bm.m, _sizeInBytes );
    return *this;
  }
  void fill( bool fillVal ){
    if( fillVal )  memset( m, 255, _sizeInBytes );
    else           memset( m,   0, _sizeInBytes );
  }
  void zero(){ memset( m, 0, _sizeInBytes ); }
  void fillWithByte( int fillByte ){
    memset( m, fillByte, _sizeInBytes );
  }
  bool validIndex( size_t index0, size_t index1 ) const{
    if( (index0 >= s0) || (index1 >= s1) ){
      if( index0 >= s0 ) std::cerr << "** index0: " << index0 << " >= size0: " << s0 << std::endl << std::flush;
      if( index1 >= s1 ) std::cerr << "** index1: " << index1 << " >= size1: " << s1 << std::endl << std::flush;
      return false;
    }
    return true;
  }
  bool operator==( const BitMatrix& bm ) const{
    if( bm.sizeInBytes() != _sizeInBytes ) return false;
    if( bm.s1 != s1 ) return false;
    if( _sizeInBytes == 0 ) return true; // both have zero size.
    return(  !memcmp( m, bm.m, _sizeInBytes )  );
  }
  virtual void setCurCol( const size_t index0 ){
    assert( index0 < s0 );
    colPtr = m+bytesPerCol*index0;
  }
  virtual bool operator() ( const size_t index1 ) const{
    assert( index1 < s1 );
    size_t byteIndex = index1 >> 3;
    size_t bitIndex = (index1 & static_cast<byte>(7));
    return( colPtr[byteIndex] & BitMatrixMask[bitIndex] );
  }
  virtual bool operator() ( const size_t index0, const size_t index1 ) const{
    assert(  validIndex( index0, index1 )  );
    size_t byteIndex = index1 >> 3;
    size_t bitIndex = (index1 & static_cast<byte>(7));
    return( m[bytesPerCol * index0 + byteIndex] & BitMatrixMask[bitIndex] );
  }
  virtual void set( const size_t index0, const size_t index1, bool val ) const{
    assert(  validIndex( index0, index1 )  );
    if( val ) setFast( index0, index1 );
    else      clearFast( index0, index1 );
  }
  virtual void set( const size_t index0, const size_t index1 ) const{
    if( !validIndex( index0, index1 ) ){
      std::cerr << "index0: " << index0 << " >= s0: " << s0 << std::endl;
      assert( false );
    }
    setFast( index0, index1 );
  }
  void setFast( const size_t index0, const size_t index1 ) const{
    size_t byteIndex = (index1 >> 3);
    size_t bitIndex = (index1 & static_cast<byte>(7));
    m[bytesPerCol * index0 + byteIndex] |= BitMatrixMask[bitIndex];
  }
  virtual void clear( const size_t index0, const size_t index1 ) const{
    assert(  validIndex( index0, index1 )  );
    size_t byteIndex = index1 >> 3;
    size_t bitIndex = (index1 & static_cast<byte>(7));
    m[bytesPerCol * index0 + byteIndex] &= BitMatrixInverseMask[bitIndex];
  }
  void clearFast( const size_t index0, const size_t index1 ) const{
    size_t byteIndex = index1 >> 3;
    size_t bitIndex = (index1 & static_cast<byte>(7));
    m[bytesPerCol * index0 + byteIndex] &= BitMatrixInverseMask[bitIndex];
  }
  void writeAsPbm( std::ostream& os );
  size_t sizeInBytes() const{ return _sizeInBytes; }
  size_t size0() const{ return s0; }
  size_t size1() const{ return s1; }
protected:
  size_t s0, s1, _sizeInBytes;
  size_t bytesPerCol;
  byte* m;
  byte* colPtr;
};


class BitMatrixFast : public BitMatrix{
public:
  BitMatrixFast() : BitMatrix(){}
  BitMatrixFast( const size_t sz0, const size_t sz1 ) : BitMatrix( sz0, sz1 ){}
  BitMatrixFast( const BitMatrix& bm ) : BitMatrix( bm ){}
  void setCurCol( const size_t index0 ){
#ifdef BITMATRIX_ALWAYS_CHECK_INDICES
    assert( index0 < s0 );
#endif // defined BITMATRIX_ALWAYS_CHECK_INDICES
    colPtr = m+bytesPerCol*index0;
  }
  bool operator() ( const size_t index1 ) const{
#ifdef BITMATRIX_ALWAYS_CHECK_INDICES
    assert( index1 < s1 );
#endif // defined BITMATRIX_ALWAYS_CHECK_INDICES
    size_t byteIndex = (index1 >> 3);
    size_t bitIndex = (index1 & static_cast<byte>(7));
    return( colPtr[byteIndex] & BitMatrixMask[bitIndex] );
  }
  bool operator() ( const size_t index0, const size_t index1 ) const{
#ifdef BITMATRIX_ALWAYS_CHECK_INDICES
    assert(  validIndex( index0, index1 )  );
#endif // defined BITMATRIX_ALWAYS_CHECK_INDICES
    size_t byteIndex = (index1 >> 3);
    size_t bitIndex = (index1 & static_cast<byte>(7));
    return( m[bytesPerCol * index0 + byteIndex] & BitMatrixMask[bitIndex] );
  }
  void set( const size_t index0, const size_t index1, bool val ) const{
#ifdef BITMATRIX_ALWAYS_CHECK_INDICES
    assert(  validIndex( index0, index1 )  );
#endif // defined BITMATRIX_ALWAYS_CHECK_INDICES
    if( val ) set( index0, index1 );
    else      clear( index0, index1 );
  }
  void set( const size_t index0, const size_t index1 ) const{
#ifdef BITMATRIX_ALWAYS_CHECK_INDICES
    if( !validIndex( index0, index1 ) ){
      std::cerr << "index0: " << index0 << " >= s0: " << s0 << std::endl;
      assert( false );
    }
#endif // defined BITMATRIX_ALWAYS_CHECK_INDICES
    size_t byteIndex = (index1 >> 3);
    size_t bitIndex = (index1 & static_cast<byte>(7));
    m[bytesPerCol * index0 + byteIndex] |= BitMatrixMask[bitIndex];
  }
  virtual void clear( const size_t index0, const size_t index1 ) const{
#ifdef BITMATRIX_ALWAYS_CHECK_INDICES
    assert(  validIndex( index0, index1 )  );
#endif // defined BITMATRIX_ALWAYS_CHECK_INDICES
    size_t byteIndex = (index1 >> 3);
    size_t bitIndex = (index1 & static_cast<byte>(7));
    m[bytesPerCol * index0 + byteIndex] &= BitMatrixInverseMask[bitIndex];
  }
};


inline std::ostream &operator<<( std::ostream &os, const BitMatrix& bm ){
  for( size_t i = 0; i < bm.size0(); ++i ){
    for( size_t j = 0; j < bm.size1(); ++j ){
      if( bm(i,j) )  os << "1";
      else           os << "0";
    }
    os << std::endl;
  }
  return os;
}


} // end namespace cbrc
#endif // defined _BITMATRIX_HH_
