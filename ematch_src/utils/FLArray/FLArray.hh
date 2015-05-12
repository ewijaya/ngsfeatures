/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2008 Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.19
 *  Last Modified: $Date: 2008/07/07 23:57:34 $
 *  
 *  Purpose: Provide a fixed length array class that checks for the validity of indices.
 *
 *  Description: FLArray is a fixed length array that checks the unsigned int index for being
 *               too large when using the operator [].
 *
 *               FLArrayFast is a derived class that avoids that small overhead by not checking
 *               the index passed to operator [] for being too large. However, when debugging
 *               FLARRAY_ALWAYS_CHECK_INDICES can be defined to force index checking with
 *               FLArrayFast too.
 *
 *               FLNumArray is an extension of FLArray which assumes T can do arithmetic.
 *               It supplies methods such as sum(), which returns the summation of the elements.
 *
 *               FLNumArrayFast is exactly analogous to FLArrayFast.
 *               In fact I tried to have it multiple inherit from FLNumArray and FLArrayFast
 *               -- but I couldn't get that to work...
 */

#ifndef _FLARRAY_HH_
#define _FLARRAY_HH_
#ifdef CBRC_DEBUG
#define FLARRAY_ALWAYS_CHECK_INDICES
#endif // defined CBRC_DEBUG

#include <assert.h>
#include <vector>
#include <iostream>

namespace cbrc{

template <typename T>
class FLArray{
public:
  typedef T* iterator;
  typedef const T* const_iterator;

  /* ********** CONSTRUCTORS ********** */
  FLArray(){ a = NULL; s = 0;}

  explicit FLArray( const size_t& sz ) : s(sz){
    a = new T[size()];
  }

  FLArray( const size_t& sz, const T& fillVal ) : s(sz){
    a = new T[size()];
    fill( fillVal );
  }

  FLArray( const size_t& sz, const T*& b ) : s(sz) {
    a = new T[size()];
    for( size_t i = 0; i < size(); ++i ) a[i] = b[i];
  }

  FLArray( const std::vector<T>& v ){
    s = v.size();
    a = new T[size()];
    for( size_t i = 0; i < size(); ++i ) a[i] = v[i];
  }

  FLArray( const std::string& v ){ // only makes sense if T is char
    s = v.size();
    a = new T[size()];
    for( size_t i = 0; i < size(); ++i ) a[i] = v[i];
  }

  FLArray( const FLArray<T>& fla ) : a(NULL){
    assign( fla.begin(), fla.end() );
  }

  virtual ~FLArray(){ delete []a; /* may delete null but that is a noop anyway. */ }

  operator std::vector<T>() const{
    std::vector<T> v( size() );
    v.assign( begin(), end() );
    return v;
  } 
  void setSize( size_t sz ){ // set size for the first time.
    assert( !a );
    s = sz;
    a = new T[size()];
  }
  void resize( size_t sz ){ // note: delete []a calls destructor for elements.
    size_t minSize = ( s < sz ) ? s : sz;
    if( a ){
      if( s == sz ) return;
      T* b = new T[sz];
      for( size_t i = 0; i < minSize; ++i ) b[i] = a[i];
      delete []a;
      a = b;
      s = sz;
    }else{
      setSize( sz );
    }
  }
  FLArray operator=( const FLArray& fla ){
    assign( fla.begin(), fla.end() );
    return *this;
  }
  FLArray operator=( std::vector<T> v ){ 
    assign( v.begin(), v.end() );
    return *this;
  }
  void assign( const typename FLArray<T>::const_iterator& begin,
	       const typename FLArray<T>::const_iterator& end ){
    assert( end >= begin );
    resize( end-begin );
    for( typename FLArray<T>::const_iterator it = begin;
	 it != end; ++it ){
      a[it-begin] = *it;
    }
  }
  void assign( const typename std::vector<T>::const_iterator& begin,
	       const typename std::vector<T>::const_iterator& end ){
    assert( end >= begin );
    resize( end-begin );
    for( typename std::vector<T>::const_iterator it = begin;
	 it != end; ++it ){
      a[it-begin] = *it;
    }
  }
  void zero(){ fill( 0 ); }
  void fill( const T& fillVal ){
    for( size_t i = 0; i < s; ++i )  a[i] = fillVal;
  }
  void fillWithByte( const int& fillByte ){
    memset( a, fillByte, sizeof(T) * s );
  }
  bool validIndex( const size_t& index ) const{
    if( index >= s ){
      std::cerr << __FILE__ << ":" <<__LINE__ 
		<< ": Out of Range Error; FLArray index: " << index << " >= size: " << s << std::endl << std::flush;
#ifdef GDB_DEBUG
      for( ;; );
#endif // defined GDB_DEBUG
      return false;
    }
    return true;
  }

  bool operator==( const FLArray& fla ) const{
    if( fla.size() != size() ) return false;
    return(  !memcmp( a, fla.a, size()*sizeof(T) )  );
  }

  virtual const T& operator()( const size_t& index ) const{
    assert( validIndex( index ) );
    return( a[index] );
  }

  virtual /***/ T& operator[]( const size_t& index ) const{
    assert( validIndex( index ) );
    return( a[index] );
  }

  void copyTo( const T& elem, size_t index ){ // copy by value.
    assert( validIndex( index ) );
    a[index] = elem;
  }

  virtual void printNumbered( std::ostream& os = std::cout, unsigned startLabel=0 ) const{
    for( size_t i = 0;; ++i ){
      os << i+startLabel << ":" << a[i];
      if( i >= size() - 1 ) break;
      os << ", ";
    }
    os << std::endl;
  }
  const size_t& size() const{ return s; }
  bool empty() const{ return s==0; }
  iterator begin() const{ return a; }
  iterator end() const{ return a+s; }
#ifndef CBRC_DEBUG
protected:      // convenient to comment this out when debugging with printf.
#endif // not defined CBRC_DEBUG
  size_t s;
  T* a;
};


template <typename T>
class FLArrayFast : public FLArray<T>{
public:
  FLArrayFast() : FLArray<T>(){}
  FLArrayFast( const size_t sz ) : FLArray<T>(sz){}
  FLArrayFast( size_t sz, T fillVal ) : FLArray<T>(sz, fillVal){}
  FLArrayFast( size_t sz, T* b ) : FLArray<T>( sz, b){}
  FLArrayFast( std::vector<T> v ) : FLArray<T>( v ){}
  FLArrayFast( T v[], unsigned int numElems ) : FLArray<T>(v, numElems){}
  FLArrayFast( const FLArray<T>& fla ) : FLArray<T>( fla ){}
  FLArrayFast operator=( std::vector<T> v ){
    return(  *(dynamic_cast< FLArray<T>* >(this)) = v  ); }
  FLArrayFast operator=( const FLArrayFast& fla ){ 
    return(  *(dynamic_cast< FLArray<T>* >(this)) = fla  ); }
  T& operator[]( const size_t index ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
    assert( FLArray<T>::validIndex(index) );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( this->a[index] );
  }
};


template <typename T>
class FLNumArray : virtual public FLArray<T>{
public:
  FLNumArray() : FLArray<T>(){}
  FLNumArray( const size_t sz ) : FLArray<T>(sz){}
  FLNumArray( size_t sz, T fillVal ) : FLArray<T>(sz, fillVal){}
  FLNumArray( size_t sz, T* b ) : FLArray<T>( sz, b){}
  FLNumArray( std::vector<T> v ) : FLArray<T>( v ){}
  FLNumArray( T v[], unsigned int numElems ) : FLArray<T>(v, numElems){}
  FLNumArray( const FLArray<T>& fla ) : FLArray<T>( fla ){}
  FLNumArray operator=( std::vector<T> v ){ 
    return(  *(dynamic_cast< FLArray<T>* >(this)) = v  );
  }
  FLNumArray operator=( const FLNumArray& fla ){
    assign( fla.begin(), fla.end() );
    return *this;
  }
  FLNumArray& operator*=( T el ){
    for( size_t i = 0; i < this->size(); ++i ) this->a[i] *= el;
    return( *this );
  }
  FLNumArray& operator/=( T el ){
    for( size_t i = 0; i < this->size(); ++i ) this->a[i] /= el;
    return( *this );
  }
  T sum() const{
    T retVal = 0;
    for( size_t i = 0; i < this->size(); ++i ) retVal += this->a[i];
    return retVal;
  }
  T max() const{
    assert( this->size() > 0 );
    T retVal = this->a[0];
    for( size_t i = 1; i < this->size(); ++i ) if( retVal < this->a[i] ) retVal = this->a[i];
    return retVal;
  }
  size_t maxIndex() const{ // returns first index in case of ties.
#ifdef GDB_DEBUG
    if( FLArray<T>::size() == 0 ){
      std::cout << __FILE__ << ":" << __LINE__ << " Error size() == 0!\n In gdb hit cntl-c...\n";
      for(;;);
    }
#endif // defined GDB_DEBUG
    assert( this->size() > 0 );
    size_t retIndex = 0;
    T maxVal = this->a[0];
    for( size_t i = 1; i < this->size(); ++i ){
      if( maxVal < this->a[i] ){ maxVal = this->a[i]; retIndex = i; }
    }
    return retIndex;
  }
  void squareElements(){
    for( size_t i = 0; i < this->size(); i++ ) this->a[i] = this->a[i] * this->a[i];
  }
};




template <typename T>
class FLNumArrayFast : public FLNumArray<T>{
public:
  FLNumArrayFast() : FLArray<T>(){}
  FLNumArrayFast( const size_t sz ) : FLArray<T>(sz){}
  FLNumArrayFast( size_t sz, T fillVal ) : FLArray<T>(sz, fillVal){}
  FLNumArrayFast( size_t sz, T* b ) : FLArray<T>( sz, b){}
  FLNumArrayFast( std::vector<T> v ) : FLArray<T>( v ){}
  FLNumArrayFast( T v[], unsigned int numElems ) : FLArray<T>(v, numElems){}
  FLNumArrayFast( const FLArray<T>& fla ) : FLArray<T>( fla ){}
  FLNumArrayFast operator=( const FLNumArrayFast& fla ){ 
    return(  *(dynamic_cast< FLNumArray<T>* >(this)) = fla  );
  }
  T& operator[]( const size_t index ) const{
#ifdef FLARRAY_ALWAYS_CHECK_INDICES
#ifdef GDB_DEBUG
    if( !FLArray<T>::validIndex( index ) ){
      for( ;; );
    }
#endif // defined GDB_DEBUG
    assert( FLArray<T>::validIndex(index) );
#endif // defined FLARRAY_ALWAYS_CHECK_INDICES
    return( this->a[index] );
  }
};


inline std::ostream& operator<<( std::ostream &os, const FLNumArray<unsigned char>& fla ){
  if( fla.size() == 0 ) return os;
  os << static_cast<unsigned int>(fla[0]);
  for( size_t i = 1; i < fla.size(); ++i ){
    os << " " << static_cast<unsigned int>(fla[i]);
  }
  return os;
}

template <typename T>
std::ostream& operator<<( std::ostream &os, const FLArray<T>& fla ){
  if( fla.size() == 0 ) return os;
  os << fla[0];
  for( size_t i = 1; i < fla.size(); ++i ){
    os << " " << fla[i];
  }
  return os;
}

};
#endif // defined _FLARRAY_HH_
