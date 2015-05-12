/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.19
 *  
 *  Last Modified: 2003.12.12
 *  
 *  Purpose: Provide a class similar to FLArray in that it does not autmotically expand -- but
 *           that provides a way to manually shrink or grow.
 *
 *  Description: VLArray is a fixed length array that checks the unsigned int index for being
 *               too large when using the operator [].
 *
 *               VLArrayFast is a derived class that avoids that small overhead by not checking
 *               the index passed to operator [] for being too large. However, when debugging
 *               FLARRAY_ALWAYS_CHECK_INDICES can be defined to force index checking with VLArrayFast too.
 *
 *               FLNumArray is an extension of VLArray which assumes elemT can do arithmetic. It supplies
 *               methods such as sum() which returns the summation of the elements.
 *
 *               FLNumArrayFast is exactly analogous to VLArrayFast. In fact I tried to have it multiple
 *               inherit from FLNumArray and VLArrayFast -- but I couldn't get that to work...
 */

#ifndef _VLARRAY_HH_
#define _VLARRAY_HH_

#include <assert.h>
#include <vector>
#include <iostream>
#include "utils/FLArray/FLArray.hh"


namespace cbrc{

template <typename elemT>
class VLArray : public FLArray{
public:
  typedef elemT* iterator;
  typedef const elemT* const_iterator;
  VLArray(){ a = NULL; s = 0; cap = 0; }
  explicit VLArray( const size_t sz ) : s(sz), cap(sz){ 
    a = new elemT[size()];
  }
  VLArray( size_t sz, elemT fillVal ) : s(sz), cap(sz){
    a = new elemT[capacity()];
    fill( fillVal );
  }
  VLArray( size_t sz, elemT* b ) : s(sz), cap(sz){
    a = new elemT[capacity()];
    for( size_t i = 0; i < size(); ++i ) a[i] = b[i];
  }
  VLArray( std::vector<elemT> v ){
    s = v.size();
    cap = v.capacity();
    a = new elemT[capacity()];
    for( size_t i = 0; i < size(); ++i ) a[i] = v[i];
  }
  VLArray( std::string v ){ // only makes sense if elemT is char
    s = v.size();
    cap = v.capacity();
    a = new elemT[cap];
    for( size_t i = 0; i < size(); ++i ) a[i] = v[i];
  }
  VLArray( elemT v[], unsigned int numElems ) : s(numElems), cap(numElems){
    a = new elemT[size()];
    memcpy( a, v, s*sizeof(elemT) );
  }
  VLArray( const VLArray<elemT>& fla ){
    s = fla.size();
    cap = fla.capacity();
    if( !s ){  a = NULL; return;  }
    a = new elemT[size()];
    memcpy( a, fla.a, fla.size()*sizeof(elemT) );
  }
  virtual ~VLArray(){ if( a ) delete []a; }
  operator std::vector<elemT>() const{
    std::vector<elemT> v( size() );
    memcpy( &v[0], a, size()*sizeof(elemT) );
    return v;
  } 
  void setSize( size_t sz ){
    assert( !a );
    s = sz;
    cap = sz;
    a = new elemT[capacity()];
  }
  void setSize( size_t sz, size_t cap ){
    assert( !a );
    s = sz;
    this->cap = cap;
    a = new elemT[capacity()];
  }
  // recycle set new size and capacity. Array contents destroyed if new size is bigger than the old one.
  //   In which case the new contents are undefined.
  void recycle( size_t newSize, size_t leeway = 0 ){
    if( !a ){
      setSize( newSize );
      return;
    }
    if( newSize <= size() ){
      s = newSize;
      return;
    }
    delete []a;
    s = newSize;
    cap = newSize + leeway;
    a = new elemT[capacity()];
  }
  VLArray operator=( const VLArray& fla ){
    if( capacity() != fla.capacity() ){
      if( a ) delete []a;
      cap = fla.capacity();
      a = new elemT[capacity()];
    }
    if( (size() != fla.size()) || (capacity() != fla.capacity()) ){
      
    assert( fla.a );
    if( a ){
      assert( size() == fla.size() );
    }
    else{
      s = fla.size();
      a = new elemT[size()];
    }
    memcpy( a, fla.a, size()*sizeof(elemT) );
    return *this;
  }
  VLArray operator=( std::vector<elemT> v ){
    if( a )  assert( s == v.size() );
    else{
      s = v.size();
      a = new elemT[size()];
    }
    for( size_t i = 0; i < s; ++i )  a[i] = v[i];
    return *this;
  }
  void zero(){ fill( 0 ); }
  void fill( elemT fillVal ){
    for( size_t i = 0; i < s; ++i ) a[i] = fillVal;
  }
  void fillWithByte( int fillByte ){
    memset( a, fillByte, sizeof(elemT) * s );
  }
  bool validIndex( size_t index ) const{
    if( index >= s ){
      std::cerr << " !!! VLArray index: " << index << " >= size: " << s << std::endl << std::flush;
#ifdef GDB_DEBUG
      for( ;; );
#endif // defined GDB_DEBUG
      return false;
    }
    return true;
  }
  bool operator==( const VLArray& fla ) const{
    if( fla.size() != size() ) return false;
    return(  !memcmp( a, fla.a, size()*sizeof(elemT) )  );
  }
  virtual elemT& operator[]( const size_t index ) const{
    assert( validIndex( index ) );
    return( a[index] );
  }
  virtual elemT& operator[]( int index ) const{
    if( index < 0 ) index = size() + index;
    assert( validIndex( index ) );
    return( a[index] );
  }
  void copyTo( const elemT& elem, size_t index ){ // copy by value.
    assert( validIndex( index ) );
    a[index] = elem;
  }
  virtual void printNumbered( std::ostream& os = std::cout, unsigned int startLabel=0 ) const{
    for( size_t i = 0;; ++i ){
      os << i+startLabel << ":" << a[i];
      if( i >= size() - 1 ) break;
      os << ", ";
    }
    os << std::endl;
  }
  size_t size() const{ return s; }
  size_t capacity() const{ return cap; }
  iterator begin() const{ return a; }
  iterator end() const{ return a+s; }
protected:
  size_t s;
  size_t cap;
  elemT* a;
};


template <typename elemT>
class VLArrayFast : public VLArray<elemT>{
public:
  VLArrayFast() : VLArray<elemT>(){}
  VLArrayFast( const size_t sz ) : VLArray<elemT>(sz){}
  VLArrayFast( size_t sz, elemT fillVal ) : VLArray<elemT>(sz, fillVal){}
  VLArrayFast( size_t sz, elemT* b ) : VLArray<elemT>( sz, b){}
  VLArrayFast( std::vector<elemT> v ) : VLArray<elemT>( v ){}
  VLArrayFast( elemT v[], unsigned int numElems ) : VLArray<elemT>(v, numElems){}
  VLArrayFast( const VLArray<elemT>& fla ) : VLArray<elemT>( fla ){}
  VLArrayFast operator=( std::vector<elemT> v ){
    std::cout << "Calling vector constructor\n"; 
    return(  *(dynamic_cast< VLArray<elemT>* >(this)) = v  ); }
  VLArrayFast operator=( const VLArrayFast& fla ){ 
    std::cout << "Calling reference constructor\n";
    return(  *(dynamic_cast< VLArray<elemT>* >(this)) = fla  ); }
  elemT& operator[]( const size_t index ) const{
#ifdef VLARRAY_ALWAYS_CHECK_INDICES
    assert( validIndex(index) );
#endif // defined VLARRAY_ALWAYS_CHECK_INDICES
    return( a[index] );
  }
};


template <typename elemT>
class FLNumArray : virtual public VLArray<elemT>{
public:
  FLNumArray() : VLArray<elemT>(){}
  FLNumArray( const size_t sz ) : VLArray<elemT>(sz){}
  FLNumArray( size_t sz, elemT fillVal ) : VLArray<elemT>(sz, fillVal){}
  FLNumArray( size_t sz, elemT* b ) : VLArray<elemT>( sz, b){}
  FLNumArray( std::vector<elemT> v ) : VLArray<elemT>( v ){}
  FLNumArray( elemT v[], unsigned int numElems ) : VLArray<elemT>(v, numElems){}
  FLNumArray( const VLArray<elemT>& fla ) : VLArray<elemT>( fla ){}
  FLNumArray operator=( std::vector<elemT> v ){ return(  *(dynamic_cast< VLArray<elemT>* >(this)) = v  ); }
  FLNumArray operator=( const FLNumArray& fla ){
    if( !fla.a ){
      assert( !a );  // only allow empty array to be assigned to another empty array.
      return (*this);
    }
    if( a ){
      assert( size() == fla.size() );
    }
    else{
      s = fla.size();
      a = new elemT[size()];
    }
    memcpy( a, fla.a, size()*sizeof(elemT) );
    return *this;
  }
  FLNumArray& operator*=( elemT el ){
    for( size_t i = 0; i < size(); ++i ) a[i] *= el;
    return( *this );
  }
  FLNumArray& operator/=( elemT el ){
    for( size_t i = 0; i < size(); ++i ) a[i] /= el;
    return( *this );
  }
  elemT sum() const{
    elemT retVal = 0;
    for( size_t i = 0; i < size(); ++i ) retVal += a[i];
    return retVal;
  }
  elemT max() const{
    assert( size() > 0 );
    elemT retVal = a[0];
    for( size_t i = 1; i < size(); ++i ) if( retVal < a[i] ) retVal = a[i];
    return retVal;
  }
  size_t maxIndex() const{ // returns first index in case of ties.
    assert( size() > 0 );
    size_t retIndex = 0;
    elemT maxVal = a[0];
    for( size_t i = 1; i < size(); ++i ) if( maxVal < a[i] ){ maxVal = a[i]; retIndex = i; }
    return retIndex;
  }
  void squareElements(){ for( size_t i = 0; i < size(); i++ ) a[i] = a[i] * a[i]; }
};




template <typename elemT>
class FLNumArrayFast : public FLNumArray<elemT>{
public:
  FLNumArrayFast() : VLArray<elemT>(){}
  FLNumArrayFast( const size_t sz ) : VLArray<elemT>(sz){}
  FLNumArrayFast( size_t sz, elemT fillVal ) : VLArray<elemT>(sz, fillVal){}
  FLNumArrayFast( size_t sz, elemT* b ) : VLArray<elemT>( sz, b){}
  FLNumArrayFast( std::vector<elemT> v ) : VLArray<elemT>( v ){}
  FLNumArrayFast( elemT v[], unsigned int numElems ) : VLArray<elemT>(v, numElems){}
  FLNumArrayFast( const VLArray<elemT>& fla ) : VLArray<elemT>( fla ){}
  FLNumArrayFast operator=( std::vector<elemT> v ){ return(  *(dynamic_cast< VLArray<elemT>* >(this)) = v  ); }
  FLNumArrayFast operator=( const FLNumArrayFast& fla ){ return(  *(dynamic_cast< VLArray<elemT>* >(this)) = fla  ); }
  elemT& operator[]( const size_t index ) const{
#ifdef VLARRAY_ALWAYS_CHECK_INDICES
    assert( validIndex(index) );
#endif // defined VLARRAY_ALWAYS_CHECK_INDICES
    return( a[index] );
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

template <typename elemT>
std::ostream& operator<<( std::ostream &os, const VLArray<elemT>& fla ){
  if( fla.size() == 0 ) return os;
  os << fla[0];
  for( size_t i = 1; i < fla.size(); ++i ){
    os << " " << fla[i];
  }
  return os;
}

};
#endif // defined _VLARRAY_HH_
