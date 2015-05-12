/*
 *  Author: Paul B. Horton
 *
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.29
 *
 *  Last Modified: $Date: 2006/04/26 10:25:56 $
 *  
 *  Purpose: Provide a fixed matrix (2D array) class that checks for the validity of indices.
 *
 *  Description: FLMatrix is a fixed length 2D array that can also be efficiently accessed as a 1D array.
 *               The operator () is used to access as a 2D array.
 *               The operator [] is used to access as a 1D array.
 *               e.g. FLMatrix<double> fml( 2, 3 );
 *               fml( 1, 2 ) = 3.2;
 *               fml[ 2*1+2 ] = 3.2;  // these two lines are equivalent.
 *
 *               Another way to access elements is to get a column pointer with getColPtr( size_t index0 ).
 *
 *
 *               FLMatrixFast is a derived class that avoids a small overhead by not checking
 *               the index passed to operator [], operator (), and getColPtr for being too large. 
 *               However, when debugging FLARRAY_ALWAYS_CHECK_INDICES can be defined to force index 
                 checking with FLMatrixFast too.
 *
 *               FLNumMatrix is an extension of FLMatrix which assumes T can do arithmetic. It supplies
 *               methods such as sum() which returns the summation of the elements.
 *
 *               FLNumMatrixFast is exactly analogous to FLMatrixFast. In fact I tried to have it multiple
 *               inherit from FLNumMatrix and FLMatrixFast -- but I couldn't get that to work...
 *
 */

#ifndef _FLMATRIX_HH_
#define _FLMATRIX_HH_
#include <assert.h>
#include <vector>
#include <iostream>
#include "utils/FLArray/FLArray.hh"

namespace cbrc{

template <typename T>
class FLMatrix {
public:
  typedef T* iterator;
  typedef const T* const_iterator;
  FLMatrix(){ m = NULL; s = 0; }
  FLMatrix( const size_t sz0, const size_t sz1 ) : s0(sz0), s1(sz1){
    s = sz0*sz1;
    m = new T[s];
  }
  FLMatrix( const size_t sz0, const size_t sz1, const T fillVal ) : s0(sz0), s1(sz1){
    s = sz0*sz1;
    m = new T[s];
    fill( fillVal );
  }
  FLMatrix( const size_t sz0, const size_t sz1, const T** const b ) :  s0(sz0), s1(sz1){
    s = sz0*sz1;
    m = new T[s];
    for( size_t i = 0; i < s; ++i )  m[i] = b[i];
  }
  FLMatrix( const std::vector< std::vector<T> > v ){
    assert( validAsMatrix(v) );
    s0 = v.size();
    s1 = v[0].size();
    s = s0*s1;
    m = new T[s];
    T* n = m;
    for( size_t i = 0; i < s0; ++i ){
      for( size_t j = 0; j < s1; ++j ){
	*n = v[i][j];
	++n;
      }
    }
  }
  FLMatrix( const FLMatrix<T>& flm ){
    s = flm.s; s0 = flm.s0; s1 = flm.s1;
    m = new T[s];
    memcpy( m, flm.m, s*sizeof(T) );
  }
  virtual ~FLMatrix(){ if( m ) delete []m; }
  void setSize( size_t sz0, size_t sz1 ){
    assert( !m );
    s0 = sz0; s1 = sz1; s = s0*s1;
    m = new T[s];
  }
  // resize with no guarantee's about the contents of the array after resizing.
  void resizeDestructive( size_t sz0, size_t sz1 ){ // note: delete []a calls destructor for elements.
    if( m ){
      if( (s0==sz0) && (s1==sz1) ) return;
      delete[]m;
    }
    s0 = sz0; s1 = sz1; s = s0*s1;
    m = new T[s];
  }
  FLMatrix operator=( const FLMatrix& flm ){
    assert( flm.m );
    if( m ){
      assert( size() == flm.size() );
    }
    else{
      s0 = flm.s0; s1 = flm.s1; s = flm.s;
      m = new T[s];
    }
    memcpy( m, flm.m, s*sizeof(T) );
    return *this;
  }
  FLMatrix operator=( std::vector< std::vector<T> > v ){
    assert( validAsMatrix(v) );
    if( m ){
      assert( s0 = v.size() );
      assert( s1 = v[0].size() );
    }      
    else{
      s0 = v.size();
      s1 = v[0].size();
      s = s0*s1;
      m = new T[s];
    }
    T* n = m;
    for( size_t i = 0; i < s0; ++i ){
      for( size_t j = 0; j < s1; ++j ){
	*n = v[i][j];
	++n;
      }
    }
    return *this;
  }
  // getVal returns default value if indices are out of range. I first used this to eliminate boundary checking
  // code in a dynamic programming traceback routine.
  T getVal( size_t index0, size_t index1, T defValue ){
    return  validIndex( index0, index1 ) ? m[ index0 * s1 + index1 ] : defValue;
  }
  void zero(){
    memset( m, 0, sizeof(T) * s );
  }
  void fill( T fillVal ){
    for( size_t i = 0; i < s; ++i ) m[i] = fillVal;
  }
  void fillWithByte( int fillByte ){
    memset( m, fillByte, sizeof(T) * s );
  }
  bool validIndex( size_t index ) const{
    if( index >= s ){
      std::cerr << "index: " << index << " >= size: " << s << std::endl << std::flush;
      return false;
    }
    return true;
  }
  bool validIndex( size_t index0, size_t index1 ) const{
    if( (index0 >= s0) || (index1 >= s1) )  return false;
    return true;
  }
  bool operator==( const FLMatrix& flm ) const{
    if( flm.s != s ) return false;
    return(  !memcmp( m, flm.m, s*sizeof(T) )  );
  }
  virtual T& operator[]( const size_t index ) const{
    assert( validIndex( index ) );
    return( m[index] );
  }
  virtual T& operator()( const size_t index0, const size_t index1 ) const{
    if( !validIndex( index0, index1 ) ){
#ifdef CBRC_DEBUG
      for(;;);
#endif // defined CBRC_DEBUG
      assert(  validIndex( index0, index1 )  );
    }
    return(  m[ index0 * s1 + index1 ]  );
  }
  FLMatrix<T> submatrix( const size_t startRow, const size_t endRow, const size_t startCol, const size_t endCol ) const{
    // Return a copy of the submatrix from startRow to endRow and startCol to endCol (inclusive).
    assert( startRow <= endRow );
    assert( startCol <= endCol );
    assert(  validIndex( startRow, startCol )  );
    assert(  validIndex( endRow, endCol )  );
    FLMatrix<T> retVal( endRow - startRow + 1, endCol - startCol + 1 );
    for( size_t i = startRow; i <= endRow; ++i ){
      for( size_t j = startCol; j <= endCol; ++j ){
	retVal( i-startRow, j-startCol ) = this->operator()( i, j );
      }
    }
    return retVal;
  }
  void printColumnMajor( std::ostream& os ) const{
    if( size() == 0 ) return;
    for( size_t i = 0; i < size1(); ++i ){
      os << operator()( 0, i);
      for( size_t j = 1; j < size0(); ++j ){
	os << " " << operator()(j, i);
      }
      os << std::endl;
    }
  }
  void shiftLeft( FLArray<T> newCol ){
    assert( newCol.size() == s1 );
    memmove( m, m+s1, (s-s1)*sizeof(T) );
    for( unsigned int i = 0; i < s1; ++i )  m[i+s-s1] = newCol[i];
  }
  void shiftRight( FLArray<T> newCol ){
    assert( newCol.size() == s1 );
    memmove( m+s1, m, (s-s1)*sizeof(T) );
    for( unsigned int i = 0; i < s1; ++i )  m[i] = newCol[i];
  }
  virtual const T* getColPtr( const size_t index0 ) const{
    assert( index0 < s0 );
    return m+index0*s1;
  }
  size_t size() const{ return s; }
  size_t size0() const{ return s0; }
  size_t size1() const{ return s1; }
  iterator begin() const{ return m; }
  iterator end() const{ return m+s; }

  // get a pointer to the array. Convenient but exposes the contiguous memory implementation. I would use sparingly.
  T* getPtr() const{ return m; }
  void friend testFLMatrix();
protected:
  bool validAsMatrix( std::vector< std::vector<T> > v ) const{
    assert( v.size() > 0 );
    size_t sz1 = v[0].size();
    assert( sz1 > 0 );
    for( size_t i = v.size(); i != 0; --i ){
      if( v[i].size() != sz1 ) return false;
    }
    return true;
  }
  size_t s, s0, s1;
  T* m;
};


template <typename T>
class FLMatrixFast : public FLMatrix<T>{
public:
  FLMatrixFast() : FLMatrix<T>(){}
  FLMatrixFast( const size_t sz0, const size_t sz1 ) : FLMatrix<T>(sz0, sz1){}
  FLMatrixFast( const size_t sz0, const size_t sz1, const T fillVal ) : FLMatrix<T>(sz0, sz1, fillVal){}
  FLMatrixFast( const size_t sz0, const size_t sz1, const T** const b ) : FLMatrix<T>( sz0, sz1, b){}
  FLMatrixFast( const std::vector< std::vector<T> > v ) : FLMatrix<T>( v ){}
  FLMatrixFast( const FLMatrix<T>& flm ) : FLMatrix<T>( flm ){}
  FLMatrixFast operator=( const std::vector< std::vector<T> > v ){ 
    return(  *(dynamic_cast< FLMatrix<T>* >(this)) = v  );
  }
  FLMatrixFast operator=( const FLMatrixFast& flm ){ return(  *(dynamic_cast< FLMatrix<T>* >(this)) = flm  ); }
  T& operator[]( const size_t index ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( FLMatrix<T>::validIndex(index) );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( FLMatrix<T>::m[index] );
  }
  T& operator() ( const size_t index0, const size_t index1 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert(  FLMatrix<T>::validIndex( index0, index1 )  );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return(  FLMatrix<T>::m[ index0 * FLMatrix<T>::s1 + index1 ]  );
  };
  const T* getColPtr( const size_t index0 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( index0 < FLMatrix<T>::s0 );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( FLMatrix<T>::m 
	    + index0 * FLMatrix<T>::s1 );
  }
};

template <typename T>
class FLNumMatrix : public FLMatrix<T>{
public:
  FLNumMatrix() : FLMatrix<T>(){}
  FLNumMatrix( const size_t sz0, const size_t sz1 ) : FLMatrix<T>(sz0, sz1){}
  FLNumMatrix( const size_t sz0, const size_t sz1, const T fillVal ) : FLMatrix<T>(sz0, sz1, fillVal){}
  FLNumMatrix( const size_t sz0, const size_t sz1, const T** const b ) : FLMatrix<T>( sz0, sz1, b){}
  FLNumMatrix( const std::vector< std::vector<T> > v ) : FLMatrix<T>( v ){}
  FLNumMatrix( const FLMatrix<T>& flm ) : FLMatrix<T>( flm ){}
  FLNumMatrix operator=( const std::vector< std::vector<T> > v ){ 
    return(  *(dynamic_cast< FLMatrix<T>* >(this)) = v  );
  }
  FLNumMatrix operator=( const FLNumMatrix& flm ){ return(  *(dynamic_cast< FLMatrix<T>* >(this)) = flm  ); }
  FLNumMatrix& operator*=( T el ){
    for( size_t i = 0; i < FLMatrix<T>::s; ++i ) FLMatrix<T>::m[i] *= el;
    return( *this );
  }
  FLNumMatrix& operator/=( T el ){
    for( size_t i = 0; i < FLMatrix<T>::s; ++i ) FLMatrix<T>::m[i] /= el;
    return( *this );
  }
  FLNumMatrix& operator+=( T el ){
    for( size_t i = 0; i < FLMatrix<T>::s; ++i ) FLMatrix<T>::m[i] += el;
    return( *this );
  }
  FLNumMatrix& operator+=( const FLNumMatrix& flm ){
    assert( FLMatrix<T>::s0 == flm.s0 );
    assert( FLMatrix<T>::s1 == flm.s1 );
    for( size_t i = 0; i < FLMatrix<T>::s; ++i ) FLMatrix<T>::m[i] += flm.m[i];
    return( *this );
  }
  T sum(){
    T ans = 0;
    for( size_t i = 0; i < FLMatrix<T>::s; ++i ) ans += FLMatrix<T>::m[i];
    return ans;
  }

  // dimension is really enum {0, 1}, choosing to sum over the current
  // row or column.
  T sum( unsigned char dimension, unsigned int index ){
    assert( dimension < 2 );
    T ans = 0;
    if( dimension==0 ){
      unsigned int base = index;
      for( size_t i = 0; i < FLMatrix<T>::size0(); ++i ){
	ans += FLMatrix<T>::m[ base ];
	base += FLMatrix<T>::s1;
      }
      return ans;
    }else{
      unsigned int base = index * FLMatrix<T>::s1;
      for( size_t i = 0; i < FLMatrix<T>::size1(); ++i ){
	ans += FLMatrix<T>::m[ base + i ];
      }
    }
    return ans;
  }

  // dimension is really enum {0, 1}, choosing to divide from elements
  // of the current row or column.
  T divide( unsigned char dimension, unsigned int index, T divisor ){
    assert( dimension < 2 );
    T ans = 0;
    if( dimension==0 ){
      unsigned int base = index;
      for( size_t i = 0; i < FLMatrix<T>::size0(); ++i ){
	FLMatrix<T>::m[ base ] /= divisor;
	base += FLMatrix<T>::s1;
      }
      return ans;
    }else{
      unsigned int base = index * FLMatrix<T>::s1;
      for( size_t i = 0; i < FLMatrix<T>::size1(); ++i ){
	FLMatrix<T>::m[ base + i ] /= divisor;
      }
    }
    return ans;
  }
  void normalize( unsigned char dimension ){ // normalize row or column
    assert( dimension < 2 );
    size_t sumSize;
    if( dimension == 0 ) sumSize = FLMatrix<T>::size1();
    else                 sumSize = FLMatrix<T>::size0();
    for( unsigned int i = 0; i < sumSize; ++i ){
      divide(  dimension, i, sum( dimension, i )  );
    }
  }
};


template <typename T>
class FLNumMatrixFast : public FLNumMatrix<T>{
public:
  FLNumMatrixFast() : FLMatrix<T>(){}
  FLNumMatrixFast( const size_t sz0, const size_t sz1 ) : FLMatrix<T>(sz0, sz1){}
  FLNumMatrixFast( const size_t sz0, const size_t sz1, const T fillVal ) : FLMatrix<T>(sz0, sz1, fillVal){}
  FLNumMatrixFast( const size_t sz0, const size_t sz1, const T** const b ) : FLMatrix<T>( sz0, sz1, b){}
  FLNumMatrixFast( const std::vector< std::vector<T> > v ) : FLMatrix<T>( v ){}
  FLNumMatrixFast( const FLMatrix<T>& flm ) : FLMatrix<T>( flm ){}
  FLNumMatrixFast operator=( const std::vector< std::vector<T> > v ){ 
    return(  *(dynamic_cast< FLMatrix<T>* >(this)) = v  );
  }
  FLNumMatrixFast operator=( const FLNumMatrixFast& flm ){ return(  *(dynamic_cast< FLMatrix<T>* >(this)) = flm  ); }
  T& operator[]( const size_t index ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( FLMatrix<T>::validIndex(index) );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( FLMatrix<T>::m[index] );
  }
  T& operator() ( const size_t index0, const size_t index1 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert(  FLMatrix<T>::validIndex( index0, index1 )  );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return(  FLMatrix<T>::m[ index0 * FLMatrix<T>::s1 + index1 ]  );
  };
  const T* getColPtr( const size_t index0 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( index0 < FLMatrix<T>::s0 );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return FLMatrix<T>::m + index0 * FLMatrix<T>::s1;
  }
};

  

template <typename T>
std::ostream &operator<<( std::ostream &os, const FLMatrix<T>& flm ){
  if( flm.size() == 0 ) return os;
  for( size_t i = 0; i < flm.size0(); ++i ){
    os << flm( i, 0);
    for( size_t j = 1; j < flm.size1(); ++j ){
      os << " " << flm(i, j);
    }
    os << std::endl;
  }
  return os;
}

};  // end namespace cbrc
#endif // defined _FLMATRIX_HH_
