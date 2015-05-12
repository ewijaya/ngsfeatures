/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.11.30
 *  Last Modified: $Date: 2009/04/26 08:18:37 $
 *
 *  Description: Provide serial numbers for strings. Also allows strings to be looked up by
 *               serial number. Currently uses linear search. If time becomes an issue in the
 *               future binary search or as hash table could be used without necessarily
 *               changing this interface.
 *
 *               See pod at bottom of this file for more details.
 */
#ifndef _SERIALNUMBERSFORSTRINGS_HH_
#define _SERIALNUMBERSFORSTRINGS_HH_
#include <vector>
#include <iostream>
#include <string>
#include <assert.h>
#include "utils/stl/stringBinaryIO.hh"

namespace cbrc{

  // the reasonable types for serialNumT are the various length unsigned integer types.
  template <typename serialNumT>
  class SerialNumbersForStrings{

    /* ********** TYPEDEFS ********** */
    typedef std::vector<std::string>    stringVecT;
    typedef stringVecT::const_iterator  const_iterator;

  public:
    /* ********** CONSTRUCTORS ********** */
    SerialNumbersForStrings(){};

    SerialNumbersForStrings( const SerialNumbersForStrings& sns ){
      _strings.reserve( sns.size() );
      for( size_t i = 0; i < sns.size(); ++i ){
	_strings.push_back( sns.strings()[i] );
      }
    }

    SerialNumbersForStrings( const stringVecT& strings ){
      _strings.reserve( strings.size() );
      for( size_t i = 0; i < strings.size(); ++i ){
	registerNewSerialNumber( strings[i] );
      }
    }

    virtual ~SerialNumbersForStrings(){}


    bool operator==( const SerialNumbersForStrings& sns ) const;
    bool operator!=( const SerialNumbersForStrings& sns ) const;
    
    /* ***** Methods which generate new serial numbers ***** */
    // Return serial number of string, creating a new serial number if necessary.
    virtual serialNumT getSerialNumber( const std::string& s );

    // assumes string S is new to container
    virtual void registerNewSerialNumber( const std::string& s ){
      assert(  !has( s )  );
      _strings.push_back( s );
    }

    // assumes strings are new to the container
    virtual void registerNewSerialNumbers( const stringVecT& strings ){
      for( size_t i = 0; i < strings.size(); ++i )  registerNewSerialNumber( strings[i] );
    }

    virtual void
    registerNewSerialNumbers( const stringVecT::const_iterator& beg,
			      const stringVecT::const_iterator& end ){
      for(  stringVecT::const_iterator cur = beg;  cur != end;  ++cur  ){
	registerNewSerialNumber( *cur );
      }
    }

    /* ********** ACCESSORS ********** */
    virtual const_iterator begin() const {  return strings().begin();  }
    virtual const_iterator end()   const {  return strings().end();    }

    virtual size_t size() const{ return _strings.size(); }

    // returns vector instead of const vector&, for the sake of derived classes.
    virtual stringVecT strings() const{
      return _strings;
    }

    // return serial number of *existing* string S, otherwise die.
    virtual size_t operator()( const std::string& s ) const;

    // return true iff string S has a serial number
    virtual bool has( const std::string& s ) const{
      for( stringVecT::const_iterator it = _strings.begin();
	   it != _strings.end();
	   ++it ){
	if( s == *it )  return true;
      }
      return false;
    }


    virtual const std::string& operator()( const size_t& serialNumber ) const{
      return _strings.at( serialNumber );
    }


    /* ********** BINARY I/O ********** */
    void read( std::istream& iStream = std::cin  ){
      stringBinaryIO::read( _strings, iStream );
    }

    void write( std::ostream& oStream = std::cout ) const{
      stringBinaryIO::write( _strings, oStream );
    }

  private:
    // class SerialNumbersForStrings object data
    stringVecT _strings;

  }; // end class SerialNumbersForStrings


  template <typename serialNumT>
  bool
  SerialNumbersForStrings<serialNumT>::operator==( const SerialNumbersForStrings& sns ) const{

    if( sns.size() != size() )  return false;

    for( size_t i = 0; i < size(); ++i ){
      if( sns(i) != (*this)(i) )  return false;
    }

    return true;
  }


  template <typename serialNumT>
  bool
  SerialNumbersForStrings<serialNumT>::operator!=( const SerialNumbersForStrings& sns ) const{
    return(  !operator==( sns )  );
  }


  /* ***** SerialNumbersForStrings::LabelInfo method definitions ***** */
  // Get serial number of string s, creating a new serial number if necessary.
  template <typename serialNumT>
  serialNumT SerialNumbersForStrings<serialNumT>::getSerialNumber( const std::string& s ){ 
    for( size_t i = 0; i < size(); ++i ){
      if( s == _strings[i] )  return i;
    }

    // else if string S is a new string to this container
    _strings.push_back( s );
    return(  size() - 1  );
  }


  // return serial number of *existing* string s, otherwise die.
  template <typename serialNumT>
  size_t SerialNumbersForStrings<serialNumT>::operator()( const std::string& s ) const{
    for( size_t i = 0; i < size(); ++i ){
      if( s == strings()[i] )  return i;
    }
    std::cerr << __FILE__ << ":" << __LINE__
	      <<  "Error: serial number does not exist for string: \"" << s << "\"\n";
    exit( -1 );
  }



  template <typename serialNumT>
  inline std::ostream& operator<<( std::ostream& os,
				   const SerialNumbersForStrings<serialNumT>& sns ){
    for( size_t i = 0; i < sns.size(); ++i ){
      os << i << " " << sns(i) << std::endl;
    }
    return os;
  }

}; // end namespace
#endif // defined _SERIALNUMBERSFORSTRINGS_HH_
#if 0 // ** start pod **
=pod

=head1 NAME

SerialNumbersForStrings

=head1 SYNOPSIS

Class for mapping strings to serial numbers

=head1 DESCRIPTION

B<SerialNumbersForStrings><serialNumT> is a template class for mapping
strings onto serial numbers. I<serialNumT> is the unsigned integer type
(such as size_t) used to hold serial numbers.

=head1 Serial Numbers

For this class serial numbers a series of successive integers
starting with zero and ending with one minus the number of strings
contained in the object.

=head1 Implementation Notes

=head2 Time/Space Efficiency

This container is implemented as a wrapper around a vector of strings.
The structure is very compact and allows strings to be looked up by
serial number in constant time. Unfortunately linear time is required
to look up serial numbers by their strings.

=head1 Constructors

In addition to a null and copy constructor, a B<SerialNumbersForStrings>
object can be constructed from a vector of std::strings.

  SerialNumbersForStrings( const std::vector<std::string>& strings );

=head1 Methods with generate new serial numbers

Some methods may generate new serial numbers:

  serialNumT getSerialNumber( const std::string& s );

void registerNewSerialNumber ( const             std::string& s  );
void registerNewSerialNumbers( const std::vector<std::string>& s );

B<getSerialNumber> is the most basic method. If string I<s>
is contained in the B<SerialNumbersForStrings> object its
serial number is returned. Otherwise a new serial number is
issued and returned.

B<registerNewSerialNumber>(s) assume the string I<s> is known
to be new to the object. When compiled with NDEBUG, it may be
significantly faster than B<getSerialNumber>.

=head1 Accessors

  size_t size() const;

B<size()> returns the number of strings contained in the object.

  const std::vector<std::string>& strings() const;

Returns the strings as a vector (in serial number order).

=head2 Access by string

  size_t operator()( const std::string& s ) const;

operator() returns the serial number of string I<s>, or
B<dies with error message> if I<s> is not contained in
the object.

  bool has( const std::string& s ) const;

Returns true iff object contains string I<s>.

=head2 Access by serial number

  const std::string& operator()( const size_t& serialNumber ) const;

operator() returns the string with serial number I<serialNumber>. Error
checking is performed with std::vector.at().

=head1 SEE ALSO

Derived class L<SerialNumbersForStrings_accessCounting>.


=head1 AUTHOR

Paul B. Horton

=cut

#endif // ** end of pod **
