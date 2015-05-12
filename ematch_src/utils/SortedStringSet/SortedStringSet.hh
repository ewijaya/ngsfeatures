/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.4.18
 *  Last Modified: $Date: 2009/06/06 02:59:01 $
 *
 *  Description: Compact data structure to map strings to serial numbers
 *               and vice versa.
 *
 *  Purpose: Originally created for RECOUNT project.
 *
 */
#ifndef SORTEDSTRINGSET_HH_
#define SORTEDSTRINGSET_HH_
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/perlish/perlish.hh"

namespace cbrc{

class SortedStringSet{
public:
  typedef  std::vector<std::string>    stringVecT;
  typedef  stringVecT::const_iterator  const_iterator;
  typedef  stringVecT::iterator        iterator;
  typedef  enum {binary, text}         fileFormatT;


  /* ********** CONSTRUCTORS ********** */

  SortedStringSet(){}

  SortedStringSet( const stringVecT& vec ) : v(vec){
    initialSort();
  }

  SortedStringSet( std::istream& iStream, fileFormatT fileFormat = text ){
    if( fileFormat == text )  readFromTextStream  ( iStream );
    else                      readFromBinaryStream( iStream );
  }


  void initialSort(){
    std::sort( v.begin(), v.end() );
    v.resize(  unique( v.begin(),v.end() ) - v.begin()  );
  }    

  /* ********** ASSIGNING TO ********** */

  void operator=( const stringVecT& vec ){
    v = vec;
    initialSort();
  }

  void assign( const stringVecT::const_iterator& begin,
	       const stringVecT::const_iterator& end ){
    v.assign( begin, end );
    std::sort( v.begin(), v.end() );
    v.resize(  std::unique( v.begin(),v.end() ) - v.begin()  );
  }


  /* ********** ACCESSORS ********** */

  bool has( const std::string& s ) const{
    return(  std::binary_search( v.begin(), v.end(), s )  );
  }

  const std::string&  operator()( const size_t& i ) const  { return v[i]; }

  // returns the index of the first element: element ≧ val,
  // or size() if val > max(element)
  size_t operator()( const std::string& s ) const{
    return   std::lower_bound( v.begin(), v.end(), s )  -  v.begin();
  }

  // return index of S, if S in set, otherwise die with error message
  size_t getSerialNumberOrDie( const std::string& s ) const{
    size_t retVal = (*this)(s);
    GDB_ASSERTF(  (*this)(retVal) == s,
		  "No serial number for string: \"%s\"", s.c_str()  );
    return retVal;
  }

  size_t  size() const  { return v.size(); }

  // for iteration
  const_iterator  begin() const  { return v.begin(); }
  const_iterator  end()   const  { return v.end();   }


  /* ********** CASTING ********** */
  const stringVecT&  asStringVec() const  { return v; }


  /* ********** I/O METHODS ********** */
  void readFromBinaryStream( std::istream& iStream ){
    // not yet implemented
  }

  // format is one string per line, presorted with no duplicated.
  void readFromTextStream  ( std::istream& iStream ){
    perlish::slurpLines( v, iStream );
    assertSorted();
  }
private:
  void assertSorted(){
    for( size_t i = 0; i < v.size()-1; ++i ){
      GDB_ASSERTF( v[i] < v[i+1],
		   "string number %zu = \"%s\", expected to be lexically less than"
		   "following string = \"%s\"",
		   i, v[i].c_str(), v[i+1].c_str() );
    }
  }

  // object data
  stringVecT  v;
};


inline  std::ostream& operator<<( std::ostream& os, const SortedStringSet& strSet ){
  BOOST_FOREACH( const std::string& curString, strSet ){
    os << curString << std::endl;
  }
  return os;
}


} // end namespace cbrc
#endif // SORTEDSTRINGSET_HH_
#if 0 // ** start pod **
=pod



=head1 NAME

SortedStringSet



=head1 SYNOPSIS

Compact class for looking up strings by serial number, and vice versa



=head1 DESCRIPTION

Compact class for looking up strings by serial number, and vice versa.
Serial numbers start from 0, and are assigned to the strings in sorted
order. This class is designed to hold a fixed set of strings and does
not support incremental addition of strings



=head1 CONSTRUCTORS


  SortedStringSet( const stringVecT& vec );

Create set holding strings in I<vec>. Sorted strings in I<vec>
and discards duplicated strings.

  SortedStringSet( std::istream& iStream, fileFormatT fileFormat = text );

Create object from strings in from I<iStream>, using
L<readFromTextStream()|"readFromTextStream"> or L<readFromBinaryStream()|"readFromBinaryStream">, depending on the value of I<fileFormat>.



=head1 ASSIGNING TO

  void operator=( const stringVecT& vec );

  void assign(  const stringVecT::const_iterator& begin,
	        const stringVecT::const_iterator& end  );

Assign string set held in I<vec> or the range [I<begin>, I<end>).
Old contents of object are discarded.



=head1 ACCESSORS


  bool has( const std::string& s ) const;

Returns true iff object contains string I<s>.


  const std::string&  operator()( const size_t& i ) const;

Return string with serial number I<i>.


  size_t operator()( const std::string& s ) const;

Returns serial number of the lexically greatest
string in object which is


  size_t  size() const;

  // for iteration
  const_iterator  begin() const;
  const_iterator  end()   const;



=head1 AUTHOR

Paul Horton

=cut

#endif // ** end of pod **
