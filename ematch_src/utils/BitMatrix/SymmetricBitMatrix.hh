/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.30
 *  Last Modified: $Date: 2008/02/24 03:17:19 $
 *
 *  Description: Square Bit Matrix where the element (i, j) is always the same as (j, i).
 *
 *  Requirements: space: requires ceiling n(n-1)/16 bytes of storage for an n x n matrix.
 *
 *  Usage: A SymmetricBitMatrix object can be constructed with a starting size or as an empty matrix.
 *         An empty matrix can be made into a matrix with a given size with setSize( size_t n ).
 *         setSize() may only be called for an empty matrix.
 *
 *         The operator () is overridden to access elements. SymmetricBitMatrix( i, j ) returns the bit at i, j,
 *         or equivalently the bit at j, i.
 *
 *         Bits can be set or cleared with set(), and clear() methods.
 *         Normally indices are checked for validity, i.e. i, j < n. This small overhead can be avoided by using the
 *         methods clearFast, and setFast. 
 *
 *  Extensions: The current implementation does not provide space for diagonal elements, (i, i). An extention of this
 *              class could easily be added to do that, and in fact virtual declarations are made to facilitate
 *              that extension. This would require ceiling n(n+1)/16 bytes of storing.
 */
#ifndef _SYMMETRICBITMATRIX_HH_
#define _SYMMETRICBITMATRIX_HH_
#include <assert.h>
#include <iostream>
#define SYMMETRICBITMATRIX_BITS_PER_BYTE 8

namespace cbrc{

typedef unsigned char byte;

const byte SymmetricBitMatrixMask[] = {1, 2, 4, 8, 16, 32, 64, 128};
const byte SymmetricBitMatrixInverseMask[] = {254, 253, 251, 247, 239, 223, 191, 127};


class SymmetricBitMatrix {
public:
  SymmetricBitMatrix(){ m = NULL; _sizeInBytes = _width = 0; }

  SymmetricBitMatrix( const size_t& width );

  SymmetricBitMatrix( const SymmetricBitMatrix& sbm ){
    _width = sbm.width();
    _sizeInBytes = sbm.sizeInBytes();
    m = new byte[sizeInBytes()];
    memcpy( m, sbm.m, sizeInBytes() );
  }

  virtual ~SymmetricBitMatrix(){ delete m; }

  void setSize( const size_t& _width );

  SymmetricBitMatrix operator=( const SymmetricBitMatrix& sbm ){
    assert( sbm.m );
    if( m ){
      assert( width() == sbm.width() );
    }else{
      _width = sbm.width();
      _sizeInBytes = sbm.sizeInBytes();
      m = new byte[sizeInBytes()];
    }
    memcpy( m, sbm.m, sizeInBytes() );
    return *this;
  }
  void fill( const bool fillVal ){
    if( fillVal )  memset( m, 255, sizeInBytes() );
    else           memset( m,   0, sizeInBytes() );
  }
  void zero(){ memset( m, 0, sizeInBytes() ); }
  void fillWithByte( const int fillByte ){
    memset( m, fillByte, sizeInBytes() );
  }

  virtual bool validIndex( const size_t& index0,
			   const size_t& index1 ) const;

  bool operator==( const SymmetricBitMatrix& sbm ) const{
    if( sbm.width() != width() ) return false;
    if( sizeInBytes() == 0 ) return true;
    return(  !memcmp( m, sbm.m, sizeInBytes() )  );
  }

  bool operator() ( size_t index0, size_t index1 ) const{
    swapIfNecessary( index0, index1 );
    assert(  validIndex( index0, index1 )  );
    size_t bitIndex = index0 + rowStart( index1 );
    size_t byteIndex = bitIndex >> 3;
    bitIndex = (bitIndex & static_cast<byte>(7));
    return( m[byteIndex] & SymmetricBitMatrixMask[bitIndex] );
  }
  virtual void set( size_t index0, size_t index1, bool val ) const{
    swapIfNecessary( index0, index1 );
    assert(  validIndex( index0, index1 )  );
    if( val ) setFast( index0, index1 );
    else      clearFast( index0, index1 );
  }
  virtual void set( size_t index0, size_t index1 ) const{
    swapIfNecessary( index0, index1 );
    assert(  validIndex( index0, index1 )  );
    setFast( index0, index1 );
  }
  void setFast( const size_t index0, const size_t index1 ) const{
    size_t bitIndex = index0 + rowStart( index1 );
    size_t byteIndex = (bitIndex >> 3);
    bitIndex = (bitIndex & static_cast<byte>(7));
    m[byteIndex] |= SymmetricBitMatrixMask[bitIndex];
  }
  virtual void clear( size_t index0, size_t index1 ) const{
    swapIfNecessary( index0, index1 );
    assert(  validIndex( index0, index1 )  );
    size_t bitIndex = rowStart( index0 ) + index1;
    size_t byteIndex = bitIndex >> 3;
    bitIndex = (bitIndex & static_cast<byte>(7));
    m[byteIndex] &= SymmetricBitMatrixInverseMask[bitIndex];
  }
  void clearFast( const size_t index0, const size_t index1 ) const{
    size_t bitIndex = index0 + rowStart( index1 );
    size_t byteIndex = bitIndex >> 3;
    bitIndex = (bitIndex & static_cast<byte>(7));
    m[byteIndex] &= SymmetricBitMatrixInverseMask[bitIndex];
  }
  const size_t& sizeInBytes() const{ return _sizeInBytes; }
  size_t width() const{ return _width; }
protected:
  virtual size_t rowStart( const size_t row ) const{ return(  ( row*(row-1) ) / 2  ); }
  void swapIfNecessary( size_t& index0, size_t& index1 ) const{
    if( index1 < index0 ){
      size_t temp = index0;
      index0 = index1;
      index1 = temp;
    }
  }
  // object data.
  size_t _width; // same as height (symmetic matrix).
  size_t _sizeInBytes;
  byte* m;
};


inline std::ostream &operator<<( std::ostream &os, const SymmetricBitMatrix& sbm ){
  for( size_t i = 1; i < sbm.width(); ++i ){
    for( size_t j = 0; j < i; ++j ){
      if( sbm(j,i) )  os << "1";
      else            os << "0";
    }
    os << std::endl;
  }
  return os;
}


  // class SymmetricBitMatrixWithDiagonal : public SymmetricBitMatrix{   // possible future extension.

}; // end namespace
#endif // defined _SYMMETRICBITMATRIX_HH_
