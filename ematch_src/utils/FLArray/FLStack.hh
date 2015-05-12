/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.13
 *  Last Modified: $Date: 2008/02/24 04:05:34 $
 *
 *  Description: 
 *
 *  Purpose: (high level purpose or motivation)
 *
 */
#ifndef _FLSTACK_HH_
#define _FLSTACK_HH_
#include <stdexcept>
#include "FLArray.hh"

namespace cbrc{

template <typename elemType>
class FLStack {
public:
  typedef const elemType* const_iterator;
  FLStack(){ sizeVar = 0; }
  FLStack( const size_t sz ){
    sizeVar = 0;
    ar.setSize( sz );
  }
  void setCapacity( const size_t sz ){ ar.setSize( sz ); }
  void push( elemType b ) throw(std::length_error){
    if( sizeVar == capacity() ){
      char errorMessage[256];
      snprintf( errorMessage, 256, " %s: line %d  pushed element past stack capacity = %zu",
		__FILE__, __LINE__, capacity() );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
      std::cerr << "cur Stack contents:\n" << (*this) << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::length_error( errorMessage )  );
    }
    ar[sizeVar] = b;
    ++sizeVar;
  }
  bool empty(){ return( sizeVar == 0 ); }
  elemType& pop() throw(std::out_of_range){
    if( empty() )  throw(  std::out_of_range( " tried to pop empty stack" )  );
    return ar[--sizeVar];
  }
  void zero(){ sizeVar = 0; }
  elemType& operator[]( const size_t& index ) const throw(std::out_of_range){
    if( index >= size() ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d  index = %zu not smaller than size = %zu",
		__FILE__, __LINE__, index, size() );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::out_of_range( errorMessage )  );
    }
    return ar[index];
  }
  size_t capacity() const{ return ar.size(); }
  size_t size() const{ return sizeVar; } 

  const_iterator begin() const{ return ar.begin(); }
  const_iterator end() const{ return ar.begin()+size(); }
  friend void testFLStack();
private:
  FLArrayFast<elemType> ar;
  unsigned int sizeVar;
};


template <typename elemType>
std::ostream& operator<<( std::ostream &os, const FLStack<elemType>& fls ){
  if( fls.size() == 0 ) return os;
  os << fls[0];
  for( size_t i = 1; i < fls.size(); ++i ){
    os << " " << fls[i];
  }
  return os;
}

}; // end namespace
#endif // defined _FLSTACK_HH_

