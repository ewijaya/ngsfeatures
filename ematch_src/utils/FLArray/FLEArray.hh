/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.3
 *  Last Modified: $Date: 2009/05/15 08:25:22 $
 *
 *  Major Change Log: Branched from FLArray 2004.10.3
 *  
 *  Description: See pod at end of this file!
 *
 *  Implementation
 *  notes:  FLENumArrayFast is deprecated. FLEArray can be made to
 *          running without range checking by setting NDEBUG
 *
 */

#ifndef _FLEARRAY_HH_
#define _FLEARRAY_HH_

#ifdef CBRC_DEBUG
#define FLEARRAY_ALWAYS_CHECK_INDICES
#endif // defined CBRC_DEBUG
#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "utils/gdb/gdbUtils.hh"
#include "utils/perlish/perlish.hh"


/* ********** Macros to ease construction from literal data ********** */
#define FLEArrayFromCArray( elemType, fleArray, cArray )		\
FLEArray<elemType> fleArray( sizeof(cArray)/sizeof(cArray[0]), cArray )

#define FLENumArrayFromCArray( elemType, fleArray, cArray )		\
FLENumArray<elemType> fleArray( sizeof(cArray)/sizeof(cArray[0]), cArray )

#define FLEArrayConstruct( elemType, fleArray, semanticType, arrayLiteral )	\
FLEArray<type> fleArray( FLEArray<elemType>::semanticType, arrayLiteral )

namespace cbrc{


template <typename T>
class FLEArray{
public:


  // typedefs
  typedef                T*  iterator;
  typedef       const    T*  const_iterator;
  typedef     unsigned char  uchar;


  // class constants
  static const size_t  npos = static_cast<size_t>(-1);


  /* ***** constructors ***** */
  FLEArray(){ a = NULL; s = 0;}

  explicit FLEArray( const size_t& sz ) : s(sz){
    a = new T[size()];
  }

  FLEArray(  const std::string  semanticTypeDescriptor,
	     const std::string  arrayLiteral  ){
    a = NULL; // start with empty array
    setFromLiteral( semanticTypeDescriptor, arrayLiteral );
  }

  FLEArray( const size_t& sz, const T& fillVal ) : s(sz){
    a = new T[size()];
    fill( fillVal, 0, sz );
  }

  FLEArray( const size_t& sz, const T* const& c_array ) : s(sz) {
    a = new T[size()];
    memcpy( a, c_array, bsize() );
  }

  FLEArray( const std::vector<T>& v ){
    s = v.size();
    a = new T[size()];
    memcpy( a, &v[0], bsize() );
  }

  FLEArray( const FLEArray<T>& fla ) : a(NULL){
    assign( fla.begin(), fla.end() );
  }

  FLEArray( const FLEArray<T>& fla, const size_t& startIndex, const size_t& endIndex ) : a(NULL){
    size_t start = std::max( startIndex, (size_t) 0 );
    size_t end = std::min( endIndex, fla.size() );
    assign( fla.begin()+start, fla.begin()+end );
  }

  // Array size is read from stream
  FLEArray( std::istream& is ){
    is.read( (char*)&s, sizeof( size_t ) );
    a = new T[size()];
    is.read( (char*)a, bsize() );
  }

  FLEArray( const size_t& size, std::istream& is )
    : s(size)
  {
    a = new T[size];
    is.read( (char*)a, bsize() );
  }    

  virtual ~FLEArray(){ delete []a; /* may delete null but that is a noop anyway. */ }

  // cast to std::vector
  operator std::vector<T>() const{
    std::vector<T> v( size() );
    v.assign( begin(), end() );
    return v;
  }


  /* ***** array size related methods ***** */

  // set size of uninitialized array
  void setSize( const size_t& sz ){
    assert( !a );
    s = sz;
    a = new T[size()];
  }

  // resize can be called anytime
  void resize( const size_t& sz );


  FLEArray operator=( const FLEArray& fla );

  FLEArray operator=( const std::vector<T>& v ){ 
    assign( v.begin(), v.end() );
    return *this;
  }


  void assign( const typename FLEArray<T>::const_iterator& begin,
	       const typename FLEArray<T>::const_iterator& end );

  void assign( const typename std::vector<T>::const_iterator& begin,
	       const typename std::vector<T>::const_iterator& end );


  void setFromLiteral(  const std::string  semanticTypeDescriptor,
			const std::string  arrayLiteral  );


  void replace( const size_t& begIdx,
		const FLEArray<T>& replacementArray );

  void replace( const size_t& begIdx,
		const size_t& endIdx,
		const T* const& cArray );


  bool operator==( const FLEArray& fla ) const{
    return(
	   (fla.size() == size())    ?  !memcmp( a, fla.a, size()*sizeof(T) )
	   /* sizes don't match */   :  false
	   );
  }

  bool operator!=( const FLEArray& fla ) const{
    return(
	   (fla.size() == size())    ?  memcmp( a, fla.a, size()*sizeof(T) )
	   /* sizes don't match */   :  true
	   );
  }


  /* ********** ACCESSORS ********** */
  virtual /***/ T& operator[]( const size_t& index ) const{
    assert( _validIndex( index ) );
    return( a[index] );
  }

  virtual const T& operator()( const size_t& index ) const{
    assert( _validIndex( index ) );
    return( a[index] );
  }

  const size_t& size() const{ return s; }

  // size of array in bytes
  size_t bsize() const{ return size()*sizeof(T); }

  bool empty() const{ return s==0; }


  // return true iff index IDX is in range
  bool indexInRange( const size_t& idx ) const{
    return( idx < size() );
  }

  iterator begin() const{ return a; }
  iterator end()   const{ return a+s; }

  const_iterator constBegin() const{ return a; }
  const_iterator constEnd()   const{ return a+s; }


  /* ***** methods for assigning a value to *****
   *       regions or all of the array
   */
  void zero(){ fill( 0 ); }

  void fill( const T& fillVal, const size_t begIdx = 0 ){
    fill( fillVal, begIdx, size() );
  }

  // fill range [begIdx, endIdx) with fillVal
  void fill( const T& fillVal, const size_t& begIdx, const size_t& endIdx );

  void fillWithByte( const int& fillByte ){
    memset( a, fillByte, bsize() );
  }


  /* ***** Binary I/O ***** */
  // read array in from IS
  void read( std::istream& is );

  // like read, skip assumes the stream pointer IS points to the beginning
  // of a binary formated FLEArray<T> data region.
  // But instead of reading that array, it simply moves the stream pointer past it.
  void static skip( std::istream& is ){
    size_t newSize;
    is.read( (char*)&newSize, sizeof( newSize ) );
    is.seekg( newSize * sizeof(T), std::ios::cur );
  }

  void write( std::ostream& os ) const{
    os.write( (char*)&s, sizeof( size_t ) );
    os.write( (char*) a, bsize() );
  }


  /* ***** text I/O methods ***** */
  virtual void print( std::ostream& os = std::cout,
		      const std::string& delimiter = "\t" ) const;


  virtual void printNumbered( std::ostream& os = std::cout,
			      const size_t startLabel=0 ) const;

  /* ***** find methods ***** */
  // return smallest index holding value VAL in range [begIdx, endIdx)
  //        or npos if VAL not found
  size_t find( const T& val, const size_t& begIdx, const size_t& endIdx );

  size_t find( const T& val, const size_t begIdx = 0 ){
    return find( val, begIdx, size() );
  }

  // return largest index holding value VAL in range [begIdx, endIdx)
  //        or npos if VAL not found
  size_t rfind( const T&val, const size_t& begIdx, const size_t& endIdx );

  size_t rfind( const T& val, const size_t begIdx = 0 ){
    return rfind( val, begIdx, size() );
  }


protected:
  static size_t bsize( const size_t& numElems ){
    return(  numElems * sizeof(T)  );
  }

  // if IDX is in range, return true, otherwise print some diagnostics and return false.
  bool _validIndex( const size_t& idx ) const;

  // return true iff [begIdx, endIdx) is a valid range
  bool _validRange( const size_t& begIdx, const size_t& endIdx );


  void _assertNonzeroSize() const;

  void _assertSizeEqual( const FLEArray<T>& fla ) const;


  // object data
  size_t s;
  T* a;
};


template <typename T>
class FLEArrayFast : public FLEArray<T>{
public:
  FLEArrayFast()
    : FLEArray<T>(){}

  FLEArrayFast( const size_t& sz )
    : FLEArray<T>(sz){}

  FLEArrayFast( const size_t& sz, const T& fillVal )
    : FLEArray<T>(sz, fillVal){}

  FLEArrayFast( const size_t& sz, const T* const c_array )
    : FLEArray<T>( sz, c_array){}

  FLEArrayFast( const std::vector<T>& v ) 
    : FLEArray<T>( v ){}

  FLEArrayFast( const FLEArray<T>& fla )
    : FLEArray<T>( fla ){}

  FLEArrayFast( const FLEArray<T>& fla, const size_t& startIndex, const size_t& endIndex )
    : FLEArray<T>( fla, startIndex, endIndex ){}

  FLEArrayFast( const size_t& size, std::istream& is )
    : FLEArray<T>( size, is ){}


  FLEArrayFast operator=( const std::vector<T>& v ){
    return(  *(dynamic_cast< FLEArray<T>* >(this)) = v  );
  }

  FLEArrayFast operator=( const FLEArrayFast& fla ){ 
    return(  *(dynamic_cast< FLEArray<T>* >(this)) = fla  );
  }


  T& operator[]( const size_t& index ) const{
#ifdef FLEARRAY_ALWAYS_CHECK_INDICES
    assert( FLEArray<T>::_validIndex(index) );
#endif // defined FLEARRAY_ALWAYS_CHECK_INDICES
    return( this->a[index] );
  }
};



template <typename T>
class FLENumArray : virtual public FLEArray<T>{
public:
  // typedefs
  typedef       T* iterator;
  typedef const T* const_iterator;

  FLENumArray()
    : FLEArray<T>(){}

  FLENumArray( const size_t& sz )
    : FLEArray<T>(sz){}

  FLENumArray(  const std::string  semanticTypeDescriptor,
		const std::string  arrayLiteral  )
    : FLEArray<T>( semanticTypeDescriptor, arrayLiteral ){}

  FLENumArray( const size_t& sz, const T& fillVal )
    : FLEArray<T>(sz, fillVal){}

  FLENumArray( const size_t& sz, const T* const& b )
    : FLEArray<T>( sz, b){}

  FLENumArray( const std::vector<T>& v )
    : FLEArray<T>( v ){}

  FLENumArray( const FLEArray<T>& fla )
    : FLEArray<T>( fla ){}

  FLENumArray( const FLEArray<T>& fla, const size_t& startIndex, const size_t& endIndex )
    : FLEArray<T>( fla, startIndex, endIndex ){}

  FLENumArray operator=( const std::vector<T>& v ){
    return(  *(dynamic_cast< FLEArray<T>* >(this)) = v  );
  }

  FLENumArray operator=( const FLENumArray<T>& fla ){
    return(  *(dynamic_cast< FLEArray<T>* >(this)) = fla  );
  }

  // multiple by scalar
  FLENumArray& operator*=( T el ){
    for( size_t i = 0; i < this->size(); ++i ) this->a[i] *= el;
    return( *this );
  }

  // divide by scalar
  FLENumArray& operator/=( T el ){
    for( size_t i = 0; i < this->size(); ++i ) this->a[i] /= el;
    return( *this );
  }

  // vector addition
  FLENumArray& operator+=( const FLENumArray<T>& fla ){
    _assertSizeEqual( fla );
    for( size_t i = 0; i < this->size(); ++i ) this->a[i] += fla[i];
    return( *this );
  }

  // vector subtraction
  FLENumArray& operator-=( const FLENumArray<T>& fla ){
    _assertSizeEqual( fla );
    for( size_t i = 0; i < this->size(); ++i ) this->a[i] -= fla[i];
    return( *this );
  }


  T sum() const{
    T retVal = 0;
    for( size_t i = 0; i < this->size(); ++i ) retVal += this->a[i];
    return retVal;
  }

  T min() const;
  T min( const size_t& begIdx,  const size_t& endIdx ) const;
  T max() const;
  T max( const size_t& begIdx,  const size_t& endIdx ) const;

  // returns smallest index i s.t. a[i] == a.min()
  size_t firstMinIndex() const;

  // returns largest  index i s.t. a[i] == a.min()
  size_t finalMinIndex() const;

  // returns smallest index i s.t. a[i] == a.max()
  size_t firstMaxIndex() const;

  // returns largest  index i s.t. a[i] == a.max()
  size_t finalMaxIndex() const;

  void squareElements(){ 
    for( size_t i = 0; i < this->size(); i++ ){
      this->a[i]   =   (  this->a[i] * this->a[i]  );
    }
  }
private:
};




template <typename T>
class FLENumArrayFast : public FLENumArray<T>{
public:
  /* ********** CONSTRUCTORS ********** */
  FLENumArrayFast() : FLEArray<T>(){}

  FLENumArrayFast( const size_t& sz ) : FLEArray<T>(sz){}

  FLENumArrayFast( const size_t& sz, const T& fillVal ) : FLEArray<T>(sz, fillVal){}

  FLENumArrayFast( const size_t& sz, const T* const b ) : FLEArray<T>( sz, b){}

  FLENumArrayFast( const std::vector<T> v ) : FLEArray<T>( v ){}

  FLENumArrayFast( const FLEArray<T>& fla ) : FLEArray<T>( fla ){}


  FLENumArrayFast operator=( const FLENumArrayFast& fla ){ 
    return(  *(dynamic_cast< FLENumArray<T>* >(this)) = fla  );
  }



  /* ********** ACCESSORS ********** */
  T& operator[]( const size_t& index ) const{
#ifdef FLEARRAY_ALWAYS_CHECK_INDICES
    assert( FLEArray<T>::_validIndex(index) );
#endif // defined FLEARRAY_ALWAYS_CHECK_INDICES
    return( this->a[index] );
  }
};



template <typename T>
std::ostream& operator<<( std::ostream &os, const FLEArray<T>& fla ){

  for( size_t i = 0; i < fla.size(); ++i ){
    if( i )  os << " ";
    os << fla[i];
  }

  return os;
}


inline std::ostream& operator<<( std::ostream &os, const FLENumArray<unsigned char>& fla ){

  for( size_t i = 0; i < fla.size(); ++i ){
    if( i )  os << " ";
    os << (unsigned int) fla[i];
  }

  return os;
}





/* ======================================================================
 *          METHOD DEFINITIONS
 * **********************************************************************/

/* ***** Constructors ***** */
template <typename T>
inline void FLEArray<T>::setFromLiteral(  const std::string  semanticTypeDescriptor,
					  const std::string  arrayLiteral  ){
  
  if( semanticTypeDescriptor == "boolean" ){
    resize(  arrayLiteral.size()  );
    for( size_t i = 0;  i < size();  ++i ){
      switch( arrayLiteral[i] ){
      case '0': 
	(*this)[i] = 0;
	break;
      case '1':
	(*this)[i] = 1;
	break;
      default:
	std::cout << __FILE__ << ":" << __LINE__ 
		  << " Error: element of arrayLiteral not equal to either '0' nor '1'" << std::endl;
	exit( -1 );
      } // end switch arrayLiteral[i]
    } // end for i, (*this)[i]
  }
  else if( semanticTypeDescriptor == "numeric" ){
    std::vector<std::string> valStrings
      = perlish::split(  boost::regex( ",\\s*" ), arrayLiteral  );

    resize(  valStrings.size()  );
    for( size_t i = 0;  i < valStrings.size();  ++i ){
      a[i]  =  boost::lexical_cast<T>( valStrings[i] );
    }
  }
  else{
    GDB_DIEF( "unsupported semantic type \"%s\"", semanticTypeDescriptor.c_str() );
  }

} // end FLEArray<T>::setFromLiteral(  const std::string&,  const std::string& arrayLiteral  )

	
    

template <typename T>
void FLEArray<T>::print( std::ostream& os,
			 const std::string& delimiter ) const{
  for( size_t i = 0; i < size(); ++i ){
    if( i )  os << delimiter;
    os << a[i];
  }
  os << std::endl;
}


template <typename T>
void FLEArray<T>::printNumbered( std::ostream& os,
				 const size_t startLabel ) const{

  for( size_t i = 0; i < size(); ++i ){
    if( i )  os << ", ";
    os << i+startLabel << ":" << a[i];
  }

  os << std::endl;
}



template <typename T>
void FLEArray<T>::fill( const T& fillVal, const size_t& begIdx, const size_t& endIdx ){
  // validate arguments
  assert(  _validRange( begIdx, endIdx )  );

  // fill in value
  const T* const endPtr = a+endIdx;
  for( T* curPtr = a+begIdx; curPtr < endPtr; ++curPtr ){
    *curPtr = fillVal;
  }
}


template <typename T>
void FLEArray<T>::resize( const size_t& sz ){

  if( a ){ // if array already initialized

    if( s == sz ) return;

    T* b = new T[sz];
    assert( b );

    const size_t numElemsToCopy = ( s < sz ) ? s : sz;
    memcpy( b, a, numElemsToCopy*sizeof(T) );

    delete []a;
    a = b;
    s = sz;
  }
  else{ // if array is uninitialized
    setSize( sz );
  }
}



template <typename T>
FLEArray<T> FLEArray<T>::operator=( const FLEArray& fla ){

  // die with error if fla is a non-initialized array
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

  // die with error if array sizes don't match
  if( a ){
    if( size() != fla.size() ){
      std::cout << __FILE__ << ":" << __LINE__ << " Error: tried to assign array of size " << fla.size() << " to one of size: " << size() << std::endl;
#ifdef GDB_DEBUG
      for( ;; );
#endif // defined GDB_DEBUG
      exit( -1 );
    }
  }
  
  // copy the contents of fla and return *this
  assign( fla.begin(), fla.end() );
  return *this;
}


template <typename T>
void FLEArray<T>::assign( const typename FLEArray<T>::const_iterator& begin,
			  const typename FLEArray<T>::const_iterator& end ){
  assert( end >= begin );

  size_t newSize = end - begin;
  resize( newSize );

  memcpy( a, begin, newSize * sizeof(T) );
}


template <typename T>
void FLEArray<T>::assign( const typename std::vector<T>::const_iterator& begin,
			  const typename std::vector<T>::const_iterator& end ){
  assert( end >= begin );
  resize( end-begin );

  for( typename std::vector<T>::const_iterator it = begin;
       it != end; ++it ){
    a[it-begin] = *it;
  }
}


template <typename T>
void FLEArray<T>::replace( const size_t& begIdx,
			   const FLEArray<T>& replacementArray ){

  // make sure there is room for the replacement
  assert(  size() - begIdx  >=  replacementArray.size()  );

  memcpy( a+begIdx, replacementArray.a, replacementArray.bsize() );
}


template <typename T>
void FLEArray<T>::replace( const size_t& begIdx,
			   const size_t& endIdx,
			   const T* const& cArray ){

  // make sure there is room for the replacement
  assert(  size() >=  endIdx - begIdx  );

  memcpy(  a+begIdx,  cArray,  bsize( endIdx - begIdx )  );
}
  



template <typename T>
size_t FLEArray<T>::find( const T& val, const size_t& begIdx, const size_t& endIdx ){
  // validate arguments
  assert(  _validRange( begIdx, endIdx )  );

  // fill in value
  const T* const endPtr = a+endIdx;

  for( const T* curPtr = a+begIdx; curPtr < endPtr; ++curPtr ){
    if( *curPtr == val ){
      return( curPtr - a );
    }
  }
  return npos;
}


template <typename T>
size_t FLEArray<T>::rfind( const T& val, const size_t& begIdx, const size_t& endIdx ){
  // validate arguments
  assert(  _validRange( begIdx, endIdx )  );

  // fill in value
  const T* const begPtr = a+begIdx;

  for( const T* curPtr = a+endIdx-1; curPtr >= begPtr; --curPtr ){
    if( *curPtr == val ){
      return( curPtr - a );
    }
  }
  return npos;
}


template <typename T>
bool FLEArray<T>::_validIndex( const size_t& idx ) const{
  if( idx >= s ){
    std::cerr << ": Out of Range Error; FLEArray index: " << idx 
	      << " >= size: " << s << std::endl << std::flush;
#ifdef GDB_DEBUG
    for( ;; );
#endif // defined GDB_DEBUG
    return false;
  }
  return true;
}


template <typename T>
bool FLEArray<T>::_validRange( const size_t& begIdx, const size_t& endIdx ){
  return(
	 /**/ (begIdx == endIdx)       ? true  // all empty ranges OK
	 :    !_validIndex( begIdx   ) ? false
	 :    !_validIndex( endIdx-1 ) ? false
	 :    /* ** default ** */        true
	 );
}


template <typename T>
void FLEArray<T>::_assertNonzeroSize() const{
#ifdef GDB_DEBUG
    if( this->size() == 0 ){
      std::cout << __FILE__ << ":" << __LINE__ << " Error size() == 0!\n Hit cntl-c to terminate\n";
      for(;;);
    }
#endif // defined GDB_DEBUG
    assert(  this->size() > 0  );
}


template <typename T>
void FLEArray<T>::_assertSizeEqual( const FLEArray<T>& fla ) const{
  if( fla.size() != this->size() ){
    std::cerr << "Error expected array sizes to match" << std::endl
	      << "This array size:   " << size()        << std::endl
	      << "Passed array size: " << fla.size()    << std::endl
      ;
#ifdef GDB_DEBUG
    std::cout << "ininitely looping for gdb debugging, hit cntl-c to terminate\n";
    for(;;);
#endif // GDB_DEBUG
    assert( fla.size() == this->size() );
    exit( -1 );
  }
}


template <typename T>
void FLEArray<T>::read( std::istream& is ){
  size_t newSize;
  is.read( (char*)&newSize, sizeof( newSize ) );
  if( s != newSize ){
    delete []a;
    s = newSize;
    a = new T[size()];
  }
  is.read( (char*)a, bsize() );
}


template <typename T>
T FLENumArray<T>::min() const{
  this->_assertNonzeroSize();

  const_iterator curPtr = this->begin();
  T retVal = *curPtr;
  ++curPtr;

  for( /* noop */; curPtr != this->end(); ++curPtr ){
    if( retVal > *curPtr ){
      retVal = *curPtr;
    }
  }

  return retVal;
}


template <typename T>
T FLENumArray<T>::min(  const size_t& begIdx,  const size_t& endIdx  ) const{
  // consistency checks
  this->_assertNonzeroSize();
  assert(  begIdx < endIdx  );
  assert(  this->_validIndex( begIdx )  );
  assert(  this->_validIndex( endIdx )  );

  const_iterator curPtr  =  this->begin() + begIdx;
  const_iterator endPtr  =  this->begin() + endIdx;
  T retVal  =  *curPtr;
  ++curPtr;

  for( /* noop */; curPtr < endPtr; ++curPtr ){
    if( retVal > *curPtr ){
      retVal = *curPtr;
    }
  }

  return retVal;
}


template <typename T>
T FLENumArray<T>::max() const{
  this->_assertNonzeroSize();

  const_iterator curPtr = this->begin();
  T retVal = *curPtr;
  ++curPtr;

  for( /* noop */; curPtr != this->end(); ++curPtr ){
    if( retVal < *curPtr ){
      retVal = *curPtr;
    }
  }

  return retVal;
}


template <typename T>
T FLENumArray<T>::max(  const size_t& begIdx,  const size_t& endIdx  ) const{
  // consistency checks
  this->_assertNonzeroSize();
  assert(  begIdx < endIdx  );
  assert(  this->_validIndex( begIdx )  );
  assert(  this->_validIndex( endIdx )  );

  const_iterator curPtr  =  this->begin() + begIdx;
  const_iterator endPtr  =  this->begin() + endIdx;
  T retVal  =  *curPtr;
  ++curPtr;

  for( /* noop */; curPtr < endPtr; ++curPtr ){
    if( retVal < *curPtr ){
      retVal = *curPtr;
    }
  }

  return retVal;
}


template <typename T>
size_t FLENumArray<T>::firstMinIndex() const{
  this->_assertNonzeroSize();

  size_t retIndex = 0;
  T minVal = this->a[0];
  for( size_t i = 1; i < this->size(); ++i ){
    if( minVal > this->a[i] ){  // if tied with minVal, prefer smaller index
      minVal = this->a[i];
      retIndex = i;
    }
  }
  return retIndex;
}


template <typename T>
size_t FLENumArray<T>::finalMinIndex() const{
  this->_assertNonzeroSize();

  size_t retIndex = 0;
  T minVal = this->a[0];
  for( size_t i = 1; i < this->size(); ++i ){
    if( minVal >= this->a[i] ){ // if tied with minVal, prefer larger index
      minVal = this->a[i];
      retIndex = i;
    }
  }
  return retIndex;
}


template <typename T>
size_t FLENumArray<T>::firstMaxIndex() const{
  this->_assertNonzeroSize();

  size_t retIndex = 0;
  T maxVal = this->a[0];
  for( size_t i = 1; i < this->size(); ++i ){
    if( maxVal < this->a[i] ){  // if tied with maxVal, prefer smaller index
      maxVal = this->a[i];
      retIndex = i;
    }
  }
  return retIndex;
}


template <typename T>
size_t FLENumArray<T>::finalMaxIndex() const{
  this->_assertNonzeroSize();

  size_t retIndex = 0;
  T maxVal = this->a[0];
  for( size_t i = 1; i < this->size(); ++i ){
    if( maxVal <= this->a[i] ){ // if tied with maxVal, prefer larger index
      maxVal = this->a[i];
      retIndex = i;
    }
  }
  return retIndex;
}



} // end namespace CBRC
#if 0 // ** start pod **
=pod


=head1 NAME

B<FLEArray>


=head1 SYNOPSIS

Array to hold fixed byte length elements. Single memory region, no dynamic size change.


=head1 DESCRIPTION

=head2 FLEArray (Base Class)

The B<FLEArray> family of classes can correctly hold fixed length elements. It
is *B<not>* appropriate for variable length type elements or pointer containing types
such as C<std::string> in which the pointed to material is also considered as part
of the object.

The B<FLArray> family of classes is provided
for variable length types such as C<std::string>, but at this point I
would recomment using L<std::vector|FLEArray vs. std::vector> for such types.

=head2 FLEArrayFast (Derived Class)

B<FLEArrayFast> is a derived class of B<FLEArray> which does not normally
L<check that indices are in range|Index Range Checking>. B<FLEArrayFast> is B<!DEPRECATED!>,
instead use B<FLEArray> and set B<NDEBUG> to turn of range checking.

=head2 FLENumArray (Derived Class)

B<FLENumArrayE<lt>TE<gt>> is an extension of B<FLEArray> which assumes element type B<T> can do arithmetic. It supplies methods such as C<sum()> which returns the summation of the elements.
B<FLENumArrayFast> is exactly analogous to B<FLEArrayFast>.

=head2 Ranges

There is no "Range" class defined but several methods take index regions in the form
C<size_t begIdx, size_t endIdx>. As in the C++ standard template library, these intervals
are half open, covering indices I<i>: I<begIdx> ≦ I<i> < I<endIdx>. In this documentation
this is often written as S<"[begIdx, endIdx)">.
The empty range is represented by setting I<begIdx> and I<endIdx> to
the same value (any value is fine for this).


=head1 TYPEDEFS

  typedef        T* iterator;
  typedef  const T* const_iterator;

These are returned by L<iterator accessors|Iterator Accessors>.

=head1 CONSTRUCTORS

  FLEArray();
  FLEArray( const size_t& size );
  FLEArray( const size_t& sz, const T& fillVal );
  FLEArray( const size_t& sz, const T* const& c_array );
  FLEArray( const std::vector<T>& v );
  FLEArray( const FLEArray<T>& fla );
  FLEArray( const FLEArray<T>& fla, const size_t& startIndex, const size_t& endIndex );
  FLEArray( std::istream& inStream );
  FLEArray( const size_t& size, std::istream& inStream );

(see also: L<"Literal Constructors">)
The constructors are mostly self-explanatory. 

  FLEArray();

The null constructor makes an array of size zero.

  FLEArray( const FLEArray<T>& fla, const size_t& startIndex, const size_t& endIndex );

Constructs an array holding the contents of range [I<startIndex>, I<endIndex>)
of array I<fla>.

  FLEArray( std::istream& inStream );
  FLEArray( const size_t& size, std::istream& inStream );

Constructs an array from a L<binary format|"BINARY I/O"> input stream. If I<size> is not given, it is read from the beginning of I<inStream>.

=head2 Literal Constructors

   FLEArray( std::string semanticTypeDescriptor, std::string arrayLiteral )
   FLEArrayFromCArray   ( elemType, fleArray, cArray );
   FLENumArrayFromCArray( elemType, fleArray, cArray );


Examples:

  double dataC[] = { 5.4, 6.0, 6.0, 6.5 };
  FLENumArrayFromCArray( double, dataFLE, dataC );

Constructs an B<FLENumArrayE<lt>doubleE<gt>> holding the contents of
I<dataC>.

  FLENumArray<double> someConstants( "numeric", "3.14, 2.78, 1.0" )   
  FLENumArray<unsigned char> mask( "boolean", "101" );

Constructs an array I<someConstants> holding the given B<double> values
and a boolean array I<mask> holding the values {true, false, true}.


=head1 Index Range Checking

When accessing an array with index I<i> B<FLEArray> always checks that C<i < size()>, but
B<FLEArrayFast> only does if the macro B<FLEARRAY_ALWAYS_CHECK_INDICES>
(or B<CBRC_DEBUG>) is set. 


=head1 CASTING

  operator std::vector<T>() const;

B<FLEArrayE<lt>TE<gt>> objects can be automatically cast to std::vector<E<lt>TE<gt>
objects.


=head1 METHODS TAKING ARRAYS AS ARGUMENTS

=head2 Non-modifying

  bool operator==( const FLEArray& fla ) const{

Does what you would expect.

=head2 Modifying the Whole Array

  FLEArray operator=( const FLEArray& fla );
  FLEArray operator=( const std::vector<T>& v );

  void assign( const typename FLEArray<T>::const_iterator& begin,
	       const typename FLEArray<T>::const_iterator& end );

  void assign( const typename std::vector<T>::const_iterator& begin,
	       const typename std::vector<T>::const_iterator& end );

These methods do what you would expect.

  void setFromLiteral(  const std::string  semanticTypeDescriptor,
			const std::string  arrayLiteral  );

Set array to size and contents represented in the string arrayLiteral.

Currently Supported combinations of type and I<semanticTypeDescriptor> are:

  semantic type     example literal
  "boolean"         "101"
  "numeric"         "2.3, 30, 2.9"

=head2 Modifying Parts of the Array

  void replace( const size_t& startIndex,
		const FLEArray<T>& replacementArray );

Replace the C<replacementArray.size()> elements starting from I<startIndex>
with the contents of I<replacementArray>. Dies if I<replacementArray> is
too big to fit completely into the array with its current size.

  void replace( const size_t& begIdx,
		const size_t& endIdx,
		const T* const& cArray );

Replace the C<endIdx - begIdx> elements starting from I<begIdx>
with the contents pointed to by I<cArray>. Dies if I<replacementArray> is
too big to fit completely into the array with its current size.


=head1 ACCESSORS

  virtual T& operator[]( const size_t& index ) const;
  const size_t& size() const;
        size_t bsize() const;
        bool   empty() const;

These are mostly self-explanatory. B<bsize> returns the array size in bytes.
L<Under some conditions|Index Range Checking> B<operator[]>, performs index range checking.

=head2 Iterator Accessors

    iterator   begin() const;
    iterator     end() const;

These allow B<FLEArray> to be used with some C++ standard template algorithms.


=head1 SIZE RELATED METHODS

  void setSize( const size_t& size );
  void resize(  const size_t& size );

These methods set the size of the array to I<size>. B<setSize> can only be called on
an uninitialized (i.e. null-constructed) object. B<resize> preserves the contents
of the array that are in range of both the old and new size.
C<a.resize(size)> does nothing when I<size> equals C<a.size()>.


=head1 FILL METHODS

  void zero();
  void fill( T fillVal, size_t begIdx, size_t endIdx );
  void fillWithByte( int fillByte ){

B<fill> fills the L<range|Ranges> [I<begIdx>, I<endIdx>) with I<fillVal>.
I<endIdx> defaults to the end of the array, I<begIdx> defaults to zero.
C<zero()> is a shortcut for C<fill(0)>.

B<fillWithByte> fills the array with byte I<fillByte>, regardless of
the element type. B<fillWithByte> uses the system library call
C<memset>; thus C<a.fillWithByte(0)> maybe be faster than
C<a.fill(0)>.


=head1 OTHER METHODS

  bool indexInRange( const size_t& idx ) const;

Returns true iff C<idx < size()>. One place to use it
is to test for termination of "backwards" loops:

  for( size_t i = a.size()-1; a.indexInRange( i ); i-- )

Is arguably more readable than the counter-intuitive
"while C<i < size()>, decrement C<i>",

  for( size_t i = a.size()-1; i < size(); i-- )

which needs to be used when C<i> is an unsigned type.


=head1 BINARY I/O

  void        read(  std::istream& inStream );
  void static skip(  std::istream& inStream );
  void        write( std::ostream& outStream ) const;

These methods all understand the same binary format.

B<read> reads a binary format representation of an array from I<inStream>
and assigns the contents to *this array. The static method B<skip>, is
a faster method, which just moves the I<inStream> head position forward the same
amount as C<read(inStream)> would, *B<without>* doing anything with the
contents.

B<write> writes the contents of *this array to I<outStream> in binary format.


=head1 TEXT I/O METHODS

  std::ostream& operator<<( std::ostream &outStream,
                            const FLEArray<T>& fla );

  std::ostream& operator<<( std::ostream &outStream,
                            const FLENumArray<unsigned char>& fla );

The specialization for C<unsigned char> assumes the elements
represent integers in the range [0-255]. For example zero is
printed as "0" instead of the NULL character.


=head1 FIND METHODS

  size_t  find( T val, size_t begIdx, size_t endIdx );
  size_t rfind( T val, size_t begIdx, size_t endIdx );

These return the first (B<find>) or final (B<rfind>) index in
L<range|Ranges> [I<begIdx>, I<endIdx>) which holds value I<val>,
or a special not-found value B<npos> if no element matches I<val>.
The name B<npos> is taken from C<std::string> but one may also
use C<indexInRange(idx)> to test if a value has been found. For example,

  FLEArray<int> a;
  ... // fill a with values
  if( a.indexInRange(  a.find(34)  ){
    std::cout << "34 occurs at position: " << a.find(34);
  }
  else{
    std::cout << "a does not contain any occurrences of 34"
  }


=head1 NUMERICAL METHODS

FLENumArray(Fast) assumes it holds a numerical type and provides a few
numerical methods for convenience.

  T min() const;
  T min( const size_t& begIdx,  const size_t& endIdx ) const;
  T max() const;
  T max( const size_t& begIdx,  const size_t& endIdx ) const;
  size_t firstMinIndex() const;
  size_t finalMinIndex() const; // returns largest  index i: a[i] == a.min()
  size_t firstMaxIndex() const; // returns smallest index i: a[i] == a.max()
  size_t finalMaxIndex() const; // returns largest  index i: a[i] == a.max()
  T sum() const;
  void squareElements()

=head2 max/min finding methods

  T min() const;
  T min( const size_t& begIdx,  const size_t& endIdx ) const;
  T max() const;
  T max( const size_t& begIdx,  const size_t& endIdx ) const;

These methods find the min/max of the whole array or the range
[begIdx, endIdx).

  size_t firstMinIndex() const; // returns smallest index i: a[i] == a.min()
  size_t finalMinIndex() const; // returns largest  index i: a[i] == a.min()
  size_t firstMaxIndex() const; // returns smallest index i: a[i] == a.max()
  size_t finalMaxIndex() const; // returns largest  index i: a[i] == a.max()

These methods find the first/final index holding the min/max value.

=head2 other numberical methods

  T sum() const;

Returns the sum of all the elements.

  void squareElements();

Replaces each element with its square.



=head1 RELATED CLASSES

The B<FLArray> family of classes can hold objects (C<std::string>) which cannot
be copied with C<memcpy> or compared with C<memcmp>. Currently there is (even)
less reason to use B<FLArray> instead of L<std::vector|FLEArray vs. std::vector>.

The B<MallocArrayE<lt>TE<gt>> family of classes further restricts the element type I<T>
to not require a constructor call upon creation, in fact it can hold user defined element
types that do not define a null constructor. This allows C<malloc> to be used
instead of C++ style array allocation (C<new T[size]>). The only place where this
currently seems to give a potential advantage is in the L<resize|SIZE RELATED METHODS>
method, where the use of C<realloc> potentially avoids copying elements to a new
area of memory. 


=head1 FLEArray vs. std::vector

B<FLEArray> is largely redundant to C<std::vector> but a few methods utilize
the assumption of fixed element size to advantage by using highly optimized
byte based library calls such as C<memcpy> and C<memcmp>. In principle C<std::vector> could
do this for specialized classes such as C<std::vectorE<lt>intE<gt>>, but I do not think
it usually does -- and in any case this would only work for the types which
happened to be specialized.

The assumption of fixed length non-pointer element type allows B<FLEArray> to
provide simple and efficient L<binary I/O|"BINARY I/O"> methods.

Another significant difference between B<FLEArray> and C<std::vector> is that
B<FLEArray> does not have the notion of capacity (nor the C<size_t> size overhead)
that C<std::vector> has; B<FLEArray> never changes size automatically,
it must be L<resized|SIZE RELATED METHODS> explicitly.


=head1 USEFUL IDIOMS

Before L<"Literal Constructors"> evolved to the present day,
it was slightly painful to simply construct a B<FLEArray> or C<std::vector> with a few
initial values. This idiom using a c-style array and C<sizeof> was useful.

    double v[] = { 2.34, 2.59, 2.34, 9.29, 10.9, 2.3, 3.9, 2.20 };

    FLEArray<double> a( sizeof(v)/sizeof(v[0]), v );

and may appear in older code. The current way to do this is:

    FLEArray<double> a( "numeric", "2.34, 2.59, 2.34, 9.29, 10.9, 2.3, 3.9, 2.20" );


=head1 AUTHOR

Paul B. Horton

=cut

#endif // ** end of pod **
#endif // defined _FLEARRAY_HH_
