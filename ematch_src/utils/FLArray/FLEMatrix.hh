/*
 *  Author: Paul B. Horton
 *
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.29
 *
 *  Last Modified: $Date: 2009/09/12 02:32:13 $
 *  
 *  Purpose: Provide a fixed matrix (2D array) class that checks for the validity of indices.
 *
 *
 *  Description: FLEMatrix is a fixed length 2D array that can also be efficiently accessed as a 1D array. *
 *               Just as FLEArray and FLArray, FLEMatrix was derived from FLMatrix. 
 *               The FLEMatrix family of classes can correctly hold fixed length elements. It
 *               is *not* correct for variable length type elements or pointer containing types
 *               such as std::string. It also does not guarantee calling constructors or 
 *               destructors on the contained elements. The FLMatrix family of classes is provided
 *               for variable length types such as std::string
 *
 *               The operator () is used to access as a 2D array.
 *               The operator [] is used to access as a 1D array.
 *               e.g. FLEMatrix<double> fml( 2, 3 );
 *               fml( 1, 2 ) = 3.2;
 *               fml[ 2*1+2 ] = 3.2;  // these two lines are equivalent.
 *
 *               Another way to access elements is to get a column pointer with getColPtr( size_t index0 ).
 *
 *
 *               FLEMatrixFast is a derived class that avoids a small overhead by not checking
 *               the index passed to operator [], operator (), and getColPtr for being too large. 
 *               However, when debugging FLARRAY_ALWAYS_CHECK_INDICES can be defined to force index 
                 checking with FLEMatrixFast too.
 *
 *               FLENumMatrix is an extension of FLEMatrix which assumes elemType can do arithmetic. It supplies
 *               methods such as sum() which returns the summation of the elements.
 *
 *               FLENumMatrixFast is exactly analogous to FLEMatrixFast. In fact I tried to have it multiple
 *               inherit from FLENumMatrix and FLEMatrixFast -- but I couldn't get that to work...
 *
 */

#ifndef _FLEMATRIX_HH_
#define _FLEMATRIX_HH_
#ifdef CBRC_DEBUG
#define FLARRAY_ALWAYS_CHECK_INDICES
#endif // defined CBRC_DEBUG

#include <assert.h>
#include <vector>
#include <iostream>
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

template <typename T>
class FLEMatrix {
public:
  typedef T* iterator;
  typedef const T* const_iterator;
  FLEMatrix(){ m = NULL; s = 0; }
  FLEMatrix( const size_t sz0, const size_t sz1 ) : s0(sz0), s1(sz1){
    s = sz0*sz1;
    m = new T[s];
  }
  FLEMatrix( const size_t sz0, const size_t sz1, const T fillVal ) : s0(sz0), s1(sz1){
    s = sz0*sz1;
    m = new T[s];
    fill( fillVal );
  }
  FLEMatrix( const size_t sz0, const size_t sz1, const T** const b ) :  s0(sz0), s1(sz1){
    s = sz0*sz1;
    m = new T[s];
    for( size_t i = 0; i < s; ++i )  m[i] = b[i];
  }
  FLEMatrix( const std::vector< std::vector<T> >& v ){
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
  FLEMatrix( const FLEMatrix<T>& flm ){
    s = flm.s; s0 = flm.s0; s1 = flm.s1;
    m = new T[s];
    memcpy( m, flm.m, s*sizeof(T) );
  }
  virtual ~FLEMatrix(){ if( m ) delete []m; }
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
  FLEMatrix operator=( const FLEMatrix& flm ){
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
  FLEMatrix operator=( std::vector< std::vector<T> > v ){
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
  bool operator==( const FLEMatrix& flm ) const{
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
  FLEMatrix<T> submatrix( const size_t startRow, const size_t endRow, const size_t startCol, const size_t endCol ) const{
    // Return a copy of the submatrix from startRow to endRow and startCol to endCol (inclusive).
    assert( startRow <= endRow );
    assert( startCol <= endCol );
    assert(  validIndex( startRow, startCol )  );
    assert(  validIndex( endRow, endCol )  );
    FLEMatrix<T> retVal( endRow - startRow + 1, endCol - startCol + 1 );
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
  void shiftLeft( FLEArray<T> newCol ){
    assert( newCol.size() == s1 );
    memmove( m, m+s1, (s-s1)*sizeof(T) );
    for( unsigned int i = 0; i < s1; ++i )  m[i+s-s1] = newCol[i];
  }
  void shiftRight( FLEArray<T> newCol ){
    assert( newCol.size() == s1 );
    memmove( m+s1, m, (s-s1)*sizeof(T) );
    for( unsigned int i = 0; i < s1; ++i )  m[i] = newCol[i];
  }
  virtual const T* getColPtr( const size_t index0 ) const{
    assert( index0 < s0 );
    return m+index0*s1;
  }

  const size_t& size()  const {  return s;   }
  const size_t& size0() const {  return s0;  }
  const size_t& size1() const {  return s1;  }

  iterator begin() const {  return m;    }
  iterator end()   const {  return m+s;  }

  // get a pointer to the array. Convenient but exposes the contiguous memory implementation. I would use sparingly.
  T* getPtr() const{ return m; }
  void friend testFLEMatrix();
protected:
  bool validAsMatrix( const std::vector< std::vector<T> >& v ) const{
    assert( v.size() > 0 );
    size_t sz1 = v[0].size();
    assert( sz1 > 0 );
    for( size_t i = v.size()-1; i != 0; --i ){
      if( v[i].size() != sz1 ){
	std::cout << __FILE__ << ":" << __LINE__
		  << " v[0].size() = " << v[0].size()
		  << " != " << "V[" << i << "].size() = "
		  << v[i].size() << std::endl;
	return false;
      }
    }
    return true;
  }
  size_t s, s0, s1;
  T* m;
};


template <typename T>
class FLEMatrixFast : public FLEMatrix<T>{
public:
  FLEMatrixFast() : FLEMatrix<T>(){}
  FLEMatrixFast( const size_t sz0, const size_t sz1 ) : FLEMatrix<T>(sz0, sz1){}
  FLEMatrixFast( const size_t sz0, const size_t sz1, const T fillVal ) : FLEMatrix<T>(sz0, sz1, fillVal){}
  FLEMatrixFast( const size_t sz0, const size_t sz1, const T** const b ) : FLEMatrix<T>( sz0, sz1, b){}
  FLEMatrixFast( const std::vector< std::vector<T> > v ) : FLEMatrix<T>( v ){}
  FLEMatrixFast( const FLEMatrix<T>& flm ) : FLEMatrix<T>( flm ){}
  FLEMatrixFast operator=( const std::vector< std::vector<T> > v ){ 
    return(  *(dynamic_cast< FLEMatrix<T>* >(this)) = v  );
  }
  FLEMatrixFast operator=( const FLEMatrixFast& flm ){ return(  *(dynamic_cast< FLEMatrix<T>* >(this)) = flm  ); }
  T& operator[]( const size_t index ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( FLEMatrix<T>::validIndex(index) );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( FLEMatrix<T>::m[index] );
  }
  T& operator() ( const size_t index0, const size_t index1 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert(  FLEMatrix<T>::validIndex( index0, index1 )  );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return(  FLEMatrix<T>::m[ index0 * FLEMatrix<T>::s1 + index1 ]  );
  };
  const T* getColPtr( const size_t index0 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( index0 < FLEMatrix<T>::s0 );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return FLEMatrix<T>::m + index0 * FLEMatrix<T>::s1;
  }
};

template <typename T>
class FLENumMatrix : virtual public FLEMatrix<T>{
public:
  FLENumMatrix() : FLEMatrix<T>(){}
  FLENumMatrix( const size_t sz0, const size_t sz1 ) : FLEMatrix<T>(sz0, sz1){}
  FLENumMatrix( const size_t sz0, const size_t sz1, const T fillVal ) : FLEMatrix<T>(sz0, sz1, fillVal){}
  FLENumMatrix( const size_t sz0, const size_t sz1, const T** const b ) : FLEMatrix<T>( sz0, sz1, b){}
  FLENumMatrix( const std::vector< std::vector<T> > v ) : FLEMatrix<T>( v ){}
  FLENumMatrix( const FLEMatrix<T>& flm ) : FLEMatrix<T>( flm ){}
  FLENumMatrix operator=( const std::vector< std::vector<T> > v ){ 
    return(  *(dynamic_cast< FLEMatrix<T>* >(this)) = v  );
  }
  FLENumMatrix operator=( const FLENumMatrix& flm ){ return(  *(dynamic_cast< FLEMatrix<T>* >(this)) = flm  ); }
  FLENumMatrix& operator*=( T el ){
    for( size_t i = 0; i < FLEMatrix<T>::s; ++i ) FLEMatrix<T>::m[i] *= el;
    return( *this );
  }
  FLENumMatrix& operator/=( T el ){
    for( size_t i = 0; i < FLEMatrix<T>::s; ++i ) FLEMatrix<T>::m[i] /= el;
    return( *this );
  }
  FLENumMatrix& operator+=( T el ){
    for( size_t i = 0; i < FLEMatrix<T>::s; ++i ) FLEMatrix<T>::m[i] += el;
    return( *this );
  }
  FLENumMatrix& operator+=( const FLENumMatrix& flm ){
    assert( FLEMatrix<T>::s0 == flm.s0 );
    assert( FLEMatrix<T>::s1 == flm.s1 );
    for( size_t i = 0; i < FLEMatrix<T>::s; ++i ) FLEMatrix<T>::m[i] += flm.m[i];
    return( *this );
  }
  T sum(){
    T ans = 0;
    for( size_t i = 0; i < FLEMatrix<T>::s; ++i ) ans += FLEMatrix<T>::m[i];
    return ans;
  }

  enum dimensionT { ROW, COLUMN };
  // dimension is really enum {0, 1}, choosing to sum over the current
  // row or column.
  T sum( dimensionT dimension, unsigned int index ){
    assert( dimension < 2 );
    assert( FLEMatrix<T>::m );
    T ans = 0;
    if( dimension==0 ){
      unsigned int base = index;
      for( size_t i = 0; i < FLEMatrix<T>::size0(); ++i ){
	ans += FLEMatrix<T>::m[ base ];
	base += FLEMatrix<T>::s1;
      }
      return ans;
    }else{
      unsigned int base = index * FLEMatrix<T>::s1;
      for( size_t i = 0; i < FLEMatrix<T>::size1(); ++i ){
	ans += FLEMatrix<T>::m[ base + i ];
      }
    }
    return ans;
  }

  // dimension is really enum {0, 1}, choosing to divide from elements
  // of the current row or column.
  T divide( unsigned char dimension, unsigned int index, T divisor ){  // divide row or column.
    assert( dimension < 2 );
    T ans = 0;
    if( dimension==0 ){
      unsigned int base = index;
      for( size_t i = 0; i < FLEMatrix<T>::size0(); ++i ){
	FLEMatrix<T>::m[ base ] /= divisor;
	base += FLEMatrix<T>::s1;
      }
      return ans;
    }else{
      unsigned int base = index * FLEMatrix<T>::s1;
      for( size_t i = 0; i < FLEMatrix<T>::size1(); ++i ){
	FLEMatrix<T>::m[ base + i ] /= divisor;
      }
    }
    return ans;
  }
  void normalize( unsigned char dimension ){ // normalize row or column
    assert( dimension < 2 );
    size_t sumSize;
    if( dimension == 0 ) sumSize = FLEMatrix<T>::size1();
    else                 sumSize = FLEMatrix<T>::size0();
    for( unsigned int i = 0; i < sumSize; ++i ){
      divide(  dimension, i, sum( dimension, i )  );
    }
  }
};


template <typename T>
class FLENumMatrixFast : public FLENumMatrix<T>{
public:
  FLENumMatrixFast() : FLEMatrix<T>(){}
  FLENumMatrixFast( const size_t sz0, const size_t sz1 ) : FLEMatrix<T>(sz0, sz1){}
  FLENumMatrixFast( const size_t sz0, const size_t sz1, const T fillVal ) : FLEMatrix<T>(sz0, sz1, fillVal){}
  FLENumMatrixFast( const size_t sz0, const size_t sz1, const T** const b ) : FLEMatrix<T>( sz0, sz1, b){}
  FLENumMatrixFast( const std::vector< std::vector<T> > v ) : FLEMatrix<T>( v ){}
  FLENumMatrixFast( const FLEMatrix<T>& flm ) : FLEMatrix<T>( flm ){}
  FLENumMatrixFast operator=( const std::vector< std::vector<T> > v ){ 
    return(  *(dynamic_cast< FLEMatrix<T>* >(this)) = v  );
  }
  FLENumMatrixFast operator=( const FLENumMatrixFast& flm ){ return(  *(dynamic_cast< FLENumMatrix<T>* >(this)) = flm  ); }
  T& operator[]( const size_t index ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( FLENumMatrixFast<T>::validIndex(index) );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( FLEMatrix<T>::m[index] );
  }
  T& operator() ( const size_t index0, const size_t index1 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert(  FLEMatrix<T>::validIndex( index0, index1 )  );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return(  FLEMatrix<T>::m[ index0 * FLEMatrix<T>::s1 + index1 ]  );
  };
  const T* getColPtr( const size_t index0 ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( index0 < FLEMatrix<T>::s0 );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return FLEMatrix<T>::m + index0 * FLEMatrix<T>::s1;
  }
};


template <typename T>
std::ostream &operator<<( std::ostream &os, const FLEMatrix<T>& flm ){
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
#endif // defined _FLEMATRIX_HH_
