/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.7.15
 *  Last Modified: $Date: 2008/07/27 10:01:38 $
 *
 *  Description: Look up table for fixed elements, mapping the objects to elements
 *               to 0, 1, ... based on their sorted order.
 *
 *  Purpose: Provide compact structure allowing quick access between elements and serial
 *           number type numerical ids.
 *
 */
#ifndef _FLSORTEDLIST_HH_
#define _FLSORTEDLIST_HH_
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "utils/gdb/gdbUtils.hh"
#include "utils/FLArray/FLArray.hh"

namespace cbrc{

template <typename T> class FLSorted;

template <typename T>
std::ostream& operator<<( std::ostream &os, const FLSorted<T>& sl );


template <typename T>
class FLSorted{
public:
  /* ********** CONSTRUCTORS ********** */
  FLSorted(){}
  FLSorted( const std::vector<T>& v ) : a(v){
    std::sort( a.begin(), a.end() );
    a.resize(  unique( a.begin(),a.end() ) - a.begin()  ); // remove duplicates and resize.
  }

  void operator=( const std::vector<T>& v ){

    GDB_ASSERTF(  a.empty(), "FLSorted object can only be assigned to once" );

    a = v;
    std::sort( a.begin(), a.end() );
    a.resize(  unique( a.begin(),a.end() ) - a.begin()  );
  }

  void assign( const typename std::vector<T>::const_iterator& begin,
	       const typename std::vector<T>::const_iterator& end ){
    if( !a.empty() ){
      if( std::equal( begin, end, a.begin() ) ){
	GDB_DIEF( "redundant assignment of FLSorted object" );
      }else{
	GDB_DIEF( "FLSorted object can only be assigned to once" );
    }
    a.assign( begin, end );
    std::sort( a.begin(), a.end() );
    a.resize(  unique( a.begin(),a.end() ) - a.begin()  );
    }
  }

  friend std::ostream& operator<< <T>( std::ostream &os, const FLSorted<T>& sl );

  
  /* ********** ACCESSORS ********** */
  size_t size()            const{ return a.size(); }
  bool   has( const T& t ) const{ return(  std::binary_search( a.begin(), a.end(), t )  ); }

  const T&     operator()( const size_t& i ) const{  return a(i);  }
  /***/ T&     operator[]( const size_t& i ) const{  return a[i];  }

  // Achtung! No error given if t is not in the list.
  /***/ size_t operator()( const T& t      ) const{
    return std::lower_bound( a.begin(), a.end(), t ) - a.begin();
  }

  typename FLArray<T>::iterator begin() const{ return a.begin(); }
  typename FLArray<T>::iterator end()   const{ return a.end();   }
private:
  FLArray<T> a;
};


template <typename T>
std::ostream& operator<<( std::ostream &os, const FLSorted<T>& sl ){
  os << sl.a;
  return os;
}

}; // end namespace cbrc
#endif // defined _FLSORTEDLIST_HH_

