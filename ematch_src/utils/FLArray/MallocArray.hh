/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.10.3
 *  
 *  Branched from FLArray 2005.4.81
 *
 *  Last Modified: $ Date: $
 *  
 *  Purpose: Provide a fixed length array class for fixed length elements that checks for the 
 *           validity of indices.
 *
 *  Description: The FLArray family of classes can correctly hold fixed length elements. 
 *
 *               As the name suggests MallocArray uses malloc, rather than new T[size()];
 *               to allocate memory.
 *
 *               This is C-like and produce slightly faster code at the cost of having to be
 *               careful not to read uninitialized elements. One convenience it provides is the
 *               ability to hold classes that do not provide a null constructor. See usage.
 *
 *               MallocArray is *not* the correct choice for variable length type elements or 
 *               pointer containing types such as std::string in which the pointed to material 
 *               is also considered as part of the object.
 *               It also does not guarantee calling constructors or 
 *               destructors on the contained elements. The FLArray family of classes is provided
 *               for variable length types such as std::string
 *
 *               MallocArray is a fixed length array that checks the size_t index for being
 *               too large when using the operator [].
 *
 *               MallocArrayFast is a derived class that avoids that small overhead by not checking
 *               the index passed to operator [] for being too large. However, when debugging
 *               MALLOCARRAY_ALWAYS_CHECK_INDICES can be defined to force index checking with 
 *               MallocArrayFast too.
 *
 *               MallocNumArray is an extension of MallocArray which assumes elemType can do arithmetic.
 *               It supplies methods such as sum() which returns the summation of the elements.
 *
 *               MallocNumArrayFast is exactly analogous to MallocArrayFast. In fact I tried to have it
 *               multiple inherit from MallocNumArray and MallocArrayFast -- but I couldn't get that to
 *                work...
 *
 *  Usage: MallocArray can be used like this:
 *         
 *         class StructLikeClass{
 *         public: 
 *           StructLikeClass( double x, double y ) : x(x), y(y){}
 *           double getY() const{ return y; }
 *         private:
 *           double x;
 *           double y;
 *         };
 *         ...
 *         
 *           cbrc::MallocNumArray<StructLikeClass> structs( 2 );
 *           structs[0] = StructLikeClass( 0.6, 7.8 );
 *           std::cout << "y value of first struct is: " << structs[0].getY() << std::endl;
 *
 *        Note that StructLikeClass does not have a null constructor!
 */

#ifndef _MALLOCARRAY_HH_
#define _MALLOCARRAY_HH_

#include <assert.h>
#include <vector>
#include <iostream>

namespace cbrc{

template <typename T>
class MallocArray{
public:
  typedef T* iterator;
  typedef const T* const_iterator;
  
  MallocArray(){ a = NULL; s = 0;}
  explicit MallocArray( const size_t sz ) : s(sz){
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
  }
  MallocArray( size_t sz, T fillVal ) : s(sz){
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
    fill( fillVal );
  }
  MallocArray( size_t sz, T* b ) : s(sz) {
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
    for( size_t i = 0; i < size(); ++i ) a[i] = b[i];
  }
  MallocArray( const std::vector<T>& v ){
    s = v.size();
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
    for( size_t i = 0; i < size(); ++i ) a[i] = v[i];
  }
  MallocArray( T v[], unsigned int numElems ) : s(numElems){
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
    memcpy( a, v, bsize() );
  }
  MallocArray( const MallocArray<T>& fla ) : a(NULL){
    assign( fla.begin(), fla.end() );
  }
  MallocArray( std::istream& is ){
    is.read( (char*)&s, sizeof( size_t ) );
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
    is.read( (char*)a, bsize() );
  }
  virtual ~MallocArray(){ free( a );  }
  operator std::vector<T>() const{
    std::vector<T> v( size() );
    v.assign( begin(), end() );
    return v;
  }
  size_t bsize(){ return size()*sizeof(T); }
  void write( std::ostream& os ){
    os.write( (char*)&s, sizeof( size_t ) );
    os.write( (char*) a, bsize() );
  }
  void read( std::istream& is ){
    size_t newSize;
    is.read( (char*)&newSize, sizeof( newSize ) );
    if( s != newSize ){
      free( a );
      s = newSize;
      a = (T*) malloc( size() * sizeof(T) ); assert( a );
    }
    is.read( (char*)a, bsize() );
  }
  void setSize( size_t sz ){ // set size for the first time.
    assert( !a );
    s = sz;
    a = (T*) malloc( size() * sizeof(T) ); assert( a );
  }
  void resize( size_t sz ){
    if( a ){
      if( s == sz ) return;
      a = (T*) realloc( (T*) a, sz*sizeof(T) ); assert( a );
      s = sz;
    }else{
      setSize( sz );
    }
  }
  MallocArray operator=( const MallocArray& fla ){
#ifdef CBRC_DEBUG
    std::cout << __FILE__ << ":" << __LINE__ << "entered operator=\n";
#endif // defined CBRC_DEBUG
    if( !fla.a ){
      if( fla.size() != 0 ){
	std::cout << __FILE__ << ":" << __LINE__ << " Error: tried to assign from null array with non-zero size\n";
#ifdef GDB_DEBUG
	for( ;; );
#endif // defined GDB_DEBUG
	exit( -1 );
      }
      s = 0;
      a = NULL;
    }
    if( a ){
      if( size() != fla.size() ){
	std::cout << __FILE__ << ":" << __LINE__ << " Error: tried to assign array of size " << fla.size() << " to one of size: " << size() << std::endl;
#ifdef GDB_DEBUG
	for( ;; );
#endif // defined GDB_DEBUG
	exit( -1 );
      }
      else{
	s = fla.size();
	a = (T*) malloc( size() * sizeof(T) ); assert( a );
      }
    }
    assign( fla.begin(), fla.end() );
    return *this;
  }
  MallocArray operator=( std::vector<T> v ){ 
    assign( v.begin(), v.end() );
    return *this;
  }
  void assign( const typename MallocArray<T>::const_iterator& begin,
	       const typename MallocArray<T>::const_iterator& end ){
    assert( end >= begin );
    resize( end-begin );
    assert( a );
    memcpy( a, begin, (end-begin)*sizeof(T) );
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
  void fill( T fillVal ){
    for( size_t i = 0; i < s; ++i ) a[i] = fillVal;
  }
  void fillWithByte( int fillByte ){
    memset( a, fillByte, sizeof(T) * s );
  }
  bool validIndex( size_t index ) const{
    if( index >= s ){
      std::cerr << " !!! MallocArray index: " << index << " >= size: " << s << std::endl << std::flush;
#ifdef GDB_DEBUG
      for( ;; );
#endif // defined GDB_DEBUG
      return false;
    }
    return true;
  }
  bool operator==( const MallocArray& fla ) const{
    if( fla.size() != size() ) return false;
    return(  !memcmp( a, fla.a, size()*sizeof(T) )  );
  }
  virtual T& operator[]( const size_t index ) const{
    assert( validIndex( index ) );
    return( a[index] );
  }
  virtual T& operator[]( int index ) const{
    if( index < 0 ) index = size() + index;
    assert( validIndex( index ) );
    return( a[index] );
  }
  void copyTo( const T& elem, size_t index ){ // copy by value.
    assert( validIndex( index ) );
    a[index] = elem;
  }
  size_t size() const{ return s; }
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
class MallocArrayFast : public MallocArray<T>{
public:
  MallocArrayFast() : MallocArray<T>(){}
  MallocArrayFast( const size_t sz ) : MallocArray<T>(sz){}
  MallocArrayFast( size_t sz, T fillVal ) : MallocArray<T>(sz, fillVal){}
  MallocArrayFast( size_t sz, T* b ) : MallocArray<T>( sz, b){}
  MallocArrayFast( std::vector<T> v ) : MallocArray<T>( v ){}
  MallocArrayFast( T v[], unsigned int numElems ) : MallocArray<T>(v, numElems){}
  MallocArrayFast( const MallocArray<T>& fla ) : MallocArray<T>( fla ){}
  MallocArrayFast operator=( std::vector<T> v ){
    return(  *(dynamic_cast< MallocArray<T>* >(this)) = v  ); }
  MallocArrayFast operator=( const MallocArrayFast& fla ){ 
    return(  *(dynamic_cast< MallocArray<T>* >(this)) = fla  ); }
  T& operator[]( const size_t index ) const{
#ifdef MALLOCARRAY_ALWAYS_CHECK_INDICES
    assert( validIndex(index) );
#endif // defined MALLOCARRAY_ALWAYS_CHECK_INDICES
    return( a[index] );
  }
};


template <typename T>
class MallocNumArray : virtual public MallocArray<T>{
public:
  MallocNumArray() : MallocArray<T>(){}
  MallocNumArray( const size_t sz ) : MallocArray<T>(sz){}
  MallocNumArray( size_t sz, T fillVal ) : MallocArray<T>(sz, fillVal){}
  MallocNumArray( size_t sz, T* b ) : MallocArray<T>( sz, b){}
  MallocNumArray( std::vector<T> v ) : MallocArray<T>( v ){}
  MallocNumArray( T v[], unsigned int numElems ) : MallocArray<T>(v, numElems){}
  MallocNumArray( const MallocArray<T>& fla ) : MallocArray<T>( fla ){}
  MallocNumArray operator=( std::vector<T> v ){ return(  *(dynamic_cast< MallocArray<T>* >(this)) = v  ); }
  MallocNumArray operator=( const MallocNumArray& fla ){
    if( !fla.a ){
      assert( !a );  // only allow empty array to be assigned to another empty array.
      return (*this);
    }
    if( a ){
      assert( size() == fla.size() );
    }
    else{
      s = fla.size();
      //      a = new T[size()];
      a = (T*) malloc( size() * sizeof(T) );
      assert( a );
    }
    memcpy( a, fla.a, size()*sizeof(T) );
    return *this;
  }
  MallocNumArray& operator*=( T el ){
    for( size_t i = 0; i < size(); ++i ) a[i] *= el;
    return( *this );
  }
  MallocNumArray& operator/=( T el ){
    for( size_t i = 0; i < size(); ++i ) a[i] /= el;
    return( *this );
  }
  MallocNumArray& operator+=( const MallocNumArray<T>& fla ){
    assert( fla.size() == size() );
    for( size_t i = 0; i < size(); ++i ) a[i] += fla[i];
    return( *this );
  }
  T sum() const{
    T retVal = 0;
    for( size_t i = 0; i < size(); ++i ) retVal += a[i];
    return retVal;
  }
  T max() const{
    assert( size() > 0 );
    T retVal = a[0];
    for( size_t i = 1; i < size(); ++i ) if( retVal < a[i] ) retVal = a[i];
    return retVal;
  }
  size_t maxIndex() const{ // returns first index in case of ties.
#ifdef GDB_DEBUG
    if( size() == 0 ){
      std::cout << __FILE__ << ":" << __LINE__ << " Error size() == 0!\n In gdb hit cntl-c...\n";
      for(;;);
    }
#endif // defined GDB_DEBUG
    assert( size() > 0 );
    size_t retIndex = 0;
    T maxVal = a[0];
    for( size_t i = 1; i < size(); ++i ) if( maxVal < a[i] ){ maxVal = a[i]; retIndex = i; }
    return retIndex;
  }
  void squareElements(){ for( size_t i = 0; i < size(); i++ ) a[i] = a[i] * a[i]; }
};




template <typename T>
class MallocNumArrayFast : public MallocNumArray<T>{
public:
  MallocNumArrayFast() : MallocArray<T>(){}
  MallocNumArrayFast( const size_t sz ) : MallocArray<T>(sz){}
  MallocNumArrayFast( size_t sz, T fillVal ) : MallocArray<T>(sz, fillVal){}
  MallocNumArrayFast( size_t sz, T* b ) : MallocArray<T>( sz, b){}
  MallocNumArrayFast( std::vector<T> v ) : MallocArray<T>( v ){}
  MallocNumArrayFast( T v[], unsigned int numElems ) : MallocArray<T>(v, numElems){}
  MallocNumArrayFast( const MallocArray<T>& fla ) : MallocArray<T>( fla ){}
  MallocNumArrayFast operator=( const MallocNumArrayFast& fla ){ return(  *(dynamic_cast< MallocNumArray<T>* >(this)) = fla  ); }
  T& operator[]( const size_t index ) const{
#ifdef MALLOCARRAY_ALWAYS_CHECK_INDICES
#ifdef GDB_DEBUG
    if( !validIndex( index ) ){
      for( ;; );
    }
#endif // defined GDB_DEBUG
    assert( validIndex(index) );
#endif // defined MALLOCARRAY_ALWAYS_CHECK_INDICES
    return( a[index] );
  }
};


inline std::ostream& operator<<( std::ostream &os, const MallocNumArray<unsigned char>& fla ){
  if( fla.size() == 0 ) return os;
  os << static_cast<unsigned int>(fla[0]);
  for( size_t i = 1; i < fla.size(); ++i ){
    os << " " << static_cast<unsigned int>(fla[i]);
  }
  return os;
}

template <typename T>
std::ostream& operator<<( std::ostream &os, const MallocArray<T>& fla ){
  if( fla.size() == 0 ) return os;
  os << fla[0];
  for( size_t i = 1; i < fla.size(); ++i ){
    os << " " << fla[i];
  }
  return os;
}

};
#endif // defined _MALLOCARRAY_HH_
