/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.8
 *  Last Modified: $Date: 2009/06/08 14:08:23 $
 *
 *  Description: First-in First-out vector of bounded length
 *
 */
#ifndef BOUNDEDLENFIFOVECTOR_HH_
#define BOUNDEDLENFIFOVECTOR_HH_
#include <iostream>

namespace cbrc{

template <typename T>
class BoundedLenFifoVector{
public:
  /* ********** CONSTRUCTORS ********** */
  BoundedLenFifoVector( const size_t& capacity ) :
    _size( 0 ),
    _capacity( capacity ),
    oldestIdx( 0 )
  {
    v.resize( capacity );
  }

  /* ********** ACCESSORS ********** */
  const size_t&  capacity() const {  return _capacity;  }

  const size_t&  size()     const {  return _size;      }


  /* ********** GENERAL METHODS ********** */

  // oldestElem( 0 ) returns oldest elem, oldestElem( capacity()-1 ) returns newest.
  const T&  oldestElem( const size_t& oldnessRank )  const {
    const size_t vectorIdx  =  (oldnessRank + oldestIdx) % capacity();
    return v[vectorIdx];
  }
    
  // push element onto vector, displacing old element if vector is full
  void push( const T& elem );

  void popOldest();
private:


  // object data
  std::vector<T> v;
  /***/ size_t _size;
  const size_t _capacity;
  /***/ size_t oldestIdx;  // index of oldest element (valid when size() > 0)
};



// push element onto vector, displacing old element if vector is full
template <typename T>
inline void BoundedLenFifoVector<T>::push( const T& elem ){

  if(  size()  <  capacity()  ){
    v[  ( oldestIdx + size() ) % capacity()  ]   =   elem;
    ++_size;
  }
  else{
    v[oldestIdx]  =  elem;  // overwrite oldest element.
    oldestIdx  =  (oldestIdx + 1) % capacity();
  }
    
}


// pop oldest element off of vector. Die if vector empty
template <typename T>
inline void BoundedLenFifoVector<T>::popOldest(){

  assert( size() );

  // circularly decrement oldestIdx
  oldestIdx  =  (oldestIdx + 1) % capacity();

  --_size;
}


} // end namespace cbrc
#endif // BOUNDEDLENFIFOVECTOR_HH_
