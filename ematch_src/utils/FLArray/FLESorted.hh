/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.7.15
 *  Last Modified: $Date: 2009/04/25 07:31:31 $
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
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

template <typename T>
class FLESorted{
public:
  /* ********** TYPEDEFS ********** */
  //  typedef  FLEArray<T>::const_iterator  const_iterator;  // what I mean.
  typedef      const T*                     const_iterator;  // what compiles.

  /* ********** CONSTRUCTORS ********** */

  FLESorted(){}

  FLESorted( const std::vector<T>& v ) : a(v){
    std::sort( a.begin(), a.end() );
    a.resize(  std::unique( a.begin(),a.end() ) - a.begin()  );
  }

  FLESorted( std::istream& is ){
    a.read( is );
  }


  /* ********** ASSIGNING TO ********** */

  void operator=( const std::vector<T>& v ){
    a = v;
    std::sort( a.begin(), a.end() );
    a.resize(  unique( a.begin(),a.end() ) - a.begin()  );
  }

  void assign( const typename std::vector<T>::const_iterator& begin,
	       const typename std::vector<T>::const_iterator& end ){
    a.assign( begin, end );
    std::sort( a.begin(), a.end() );
    a.resize(  std::unique( a.begin(),a.end() ) - a.begin()  );
  }

  
  /* ********** CASTING ********** */

  const FLEArray<T>&  asArray() const  { return a; }


  /* ********** ACCESSORS ********** */

  const size_t&  size()  const  { return a.size(); }

  const T&  operator()( const size_t& i ) const  { return a(i); }

  // for iteration
  const_iterator  begin() const  { return a.begin(); }
  const_iterator  end()   const  { return a.end(); }


  /* ********** SEARCH METHODS ********** */
  bool has( const T& val ) const{
    return(  std::binary_search( a.begin(), a.end(), val )  );
  }

  // returns the index of the first element: element ≧ val
  size_t elementRank( const T& val ) const{
    return std::lower_bound( a.begin(), a.end(), val ) - a.begin();
  }

  // returns the index of the first element: element > val
  size_t elementRankAfterTies( const T& val ) const{
    return std::upper_bound( a.begin(), a.end(), val ) - a.begin();
  }


  /* ********** IO METHODS ********** */

  void read(  std::istream& is )       {  a.read(  is );  } // assumes elements already sorted.

  void write( std::ostream& os ) const {  a.write( os );  }
private:
  // object data
  FLEArray<T> a;
};


template <typename T>
std::ostream& operator<<( std::ostream &os, const FLESorted<T>& sl ){
  os << sl.asArray();
  return os;
}

}; // end namespace cbrc
#endif // defined _FLSORTEDLIST_HH_
#if 0 // ** start pod **
=pod


=head1 NAME

B<FLESorted>E<lt>I<T>E<gt>



=head1 SYNOPSIS

B<FLESorted>E<lt>I<T>E<gt> is a compact datastructure which maps objects
of type I<T> to serial numbers and vice versa.



=head1 DESCRIPTION

B<FLESorted>E<lt>I<T>E<gt> maps a set of n unique objects of type I<T> to the
serial numbers 0..n-1, where the serial number of each object corresponds
to its rank when the objects are sorted by C<std::sort>.

I<T> should be an element type suitable to be held in B<FLEArray>, i.e.
I<T> should be a self contained fixed length chunck of memory which
can be correctly copied with C<memcpy>. Most builtins such as C<int>, C<double>
work fine as do simple non-pointer-containing structs. C<std::string> does not
qualify because it is variable length.



=head1 CONSTRUCTORS

  FLESorted()

Empty constructor.

  FLESorted( const std::vector<T>& v )

Constructs a B<FLESorted> object from the objects held in I<v>.

  FLESorted( std::istream& is )

Reads set of type I<T> objects from input stream I<is>
in binary format. This format is defined in the documentation
fo B<FLEArray|FLEArray> section "BINARY IO".



=head1 ASSIGNING TO

  void operator=( const std::vector<T>& v );

  void assign( const typename std::vector<T>::const_iterator& begin,
	       const typename std::vector<T>::const_iterator& end );

Assigns contents indicated by Iv> or I<begin> and I<end>. Contents do not need to be presorted.
Duplicated elements are ignored.



=head1 CASTING


  const FLEArray<T>& asArray() const

Returns object as a reference of type B<FLEArray>E<lt>I<T>E<gt>.



=head1 ACCESSORS


  size_t size() const;

Returns number of elements.


  const T& operator()( const size_t& i ) const;

Returns constant reference to element with serial number I<i>.


  const_iterator begin() const;
  const_iterator end()   const;

Return constant iterators for accessing elements.



=head1 SEARCH METHODS


  bool has( const T& val ) const;

Returns true iff an element equal to I<val> is present in the set of elements.
Uses binary search requiring lg( C<size()> ) time.


  size_t elementRank( const T& val ) const;

Returns the index of the first element greater than or equal to I<val>.


  size_t elementRankAfterTies( const T& val ) const{

Returns the index of the first element: element greater than I<val>.



=head1 I/O METHODS


  void write( std::ostream& os ) const;

Write contents to output stream I<os> in binary form.


  void read(  std::istream& is );

Read contents in from input stream I<is>.




=head1 RELATED CLASSES

A related class B<sortedStringSet> is planned.


=head1 AUTHOR

Paul B. Horton

=cut

#endif // ** end of pod **

