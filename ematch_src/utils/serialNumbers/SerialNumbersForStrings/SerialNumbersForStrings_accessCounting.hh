/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.11.30
 *  Last Modified: $Date: 2009/04/20 12:39:05 $
 *
 *  Description: Specialization of SerialNumbersForStrings, see pod below
 *               for details.
 */
#ifndef _SERIALNUMBERSFORSTRINGS_ACCESSCOUNTING_HH_
#define _SERIALNUMBERSFORSTRINGS_ACCESSCOUNTING_HH_
#include <vector>
#include <iostream>
#include <string>
#include <assert.h>
#include "SerialNumbersForStrings.hh"

namespace cbrc{

  // the reasonable types for serialNumT are the various length unsigned integer types.
  template <typename serialNumT>
  class SerialNumbersForStrings_accessCounting : public SerialNumbersForStrings<serialNumT>{

    /* ********** sub-class StringInfo ********** */
    class StringInfo{
    public:
      /* ***** constructors ***** */
      StringInfo(){  _accessCount=0;  }

      StringInfo( const std::string& _value )
	: _value(_value){
	_accessCount=1;
      }

      /* ***** accessors ***** */
      const std::string& value() const{ return _value; }
      serialNumT accessCount() const{ return _accessCount; }

      /* ***** methods for sorting ***** */
      bool operator>( const StringInfo& stringInfo ) const{
	return( accessCount() > stringInfo.accessCount()  );
      }

      static bool sortingOrder( const StringInfo& stringInfo0,
				const StringInfo& stringInfo1 ){
	return(  stringInfo0.accessCount() > stringInfo1.accessCount()  );
      }

      /* ***** general methods ***** */
      void incrementAccessCount();
    private:
      // sub-class StringInfo object data
      serialNumT  _accessCount;
      std::string _value;
    }; // end class StringInfo


    // resume class SerialNumbersForStrings_accessCounting
  public:
    /* ***** Constructors ***** */
    SerialNumbersForStrings_accessCounting(){}      

    SerialNumbersForStrings_accessCounting( const SerialNumbersForStrings<serialNumT>& sns ){
      elems.reserve( sns.size() );
      for( size_t i = 0; i < sns.size(); ++i ){
	registerNewSerialNumber( sns(i) );
      }
    }

    SerialNumbersForStrings_accessCounting( const std::vector<std::string>& strings ){
      elems.reserve( strings.size() );
      for( size_t i = 0; i < strings.size(); ++i ){
	registerNewSerialNumber( strings[i] );
      }
    }

    bool operator==( const SerialNumbersForStrings_accessCounting& sns ) const{
      if( sns.size() != size() ) return false;
      for( size_t i = 0; i < size(); ++i )  if( sns[i] != (*this)[i] ) return false;
      return true;
    }


    /* ***** Accessors ***** */
    size_t size() const{ return elems.size(); }

    // Get serial number of string, creating a new serial number if necessary.
    // Increments access count of string S
    serialNumT getSerialNumber( const std::string& s );

    void registerNewSerialNumber( const std::string& s ){
      elems.push_back( s );
    }

    // return serial number of *existing* string s, otherwise die.
    size_t operator()( const std::string& s ) const;

    bool contains( const std::string& s ) const{
      for( size_t i = 0; i < size(); ++i ){
	if( s == elems[i].value() )  return true;
      }
      return false;
    }

    std::vector<std::string> strings() const;

    size_t accessCount( const size_t& serialNumber ) const{
      return elems.at( serialNumber ).accessCount();
    }

    const std::string& operator()( const size_t& serialNumber ) const{
      return elems.at( serialNumber ).value();
    }


    void renumberByAccessCount(){
      std::sort( elems.begin(), elems.end(), elems.begin()->sortingOrder );
    }
  private:
    // class SerialNumbersForStrings_accessCounting object data
    std::vector<StringInfo> elems;
  }; // end class SerialNumbersForStrings_accessCounting



  /* ***** SerialNumbersForStrings_accessCounting::StringInfo method definitions ***** */
  // Get serial number of string, creating a new serial number if necessary.
  // Increments access count of label s.
  template <typename serialNumT>
  serialNumT SerialNumbersForStrings_accessCounting<serialNumT>::getSerialNumber( const std::string& s ){ 
    for( size_t i = 0; i < size(); ++i ){
      if( s == elems[i].value() ){
	elems[i].incrementAccessCount();
	return i;
      }
    }
    elems.push_back( s );
    return(  size() - 1  );
  }



  template <typename serialNumT>
  void SerialNumbersForStrings_accessCounting<serialNumT>::StringInfo::incrementAccessCount(){ 
    ++_accessCount;
#ifdef CBRC_DEBUG
    assert( accessCount() ); // check for overflow
#endif // defined CBRC_DEBUG
  }


  // return serial number of *existing* string s, otherwise die.
  template <typename serialNumT>
  size_t
  SerialNumbersForStrings_accessCounting<serialNumT>::operator()( const std::string& s ) const{
    for( size_t i = 0; i < size(); ++i ){
      if( s == elems[i].value() )  return i;
    }
    // else if string S not found
    std::cerr << __FILE__ << ":" << __LINE__
	      <<  "Error: serial number does not exist for string: \"" << s << "\"\n";
    exit( -1 );
  }


  template <typename serialNumT>
  std::vector<std::string> SerialNumbersForStrings_accessCounting<serialNumT>::strings() const{
    std::vector<std::string> v;
    v.reserve( size() );
    for( size_t i = 0; i < size(); ++i ){
      v.push_back( operator()(i) );
    }
    return v;
  }


  template <typename serialNumT>
  inline std::ostream& operator<<( std::ostream& os,
				   const SerialNumbersForStrings_accessCounting<serialNumT>& sns ){
    for( size_t i = 0; i < sns.size(); ++i ){
      os << i << " " << sns(i) << std::endl;
    }
    return os;
  }

}; // end namespace
#endif // defined _SERIALNUMBERSFORSTRINGS_ACCESSCOUNTING_HH_
#if 0 // ** start pod **
=pod

=head1 NAME

SerialNumbersForStrings_accessCounting

=head1 SYNOPSIS

Access counting specialization of L<SerialNumbersForStrings>.

=head1 DESCRIPTION

Specialization of L<SerialNumbersForStrings> which counts access frequency
through method B<getSerialNumber>. For documentation on methods shared with
class B<SerialNumbersForStrings>, please see the documentation in that file.

=cut

=head1 Methods

  void renumberByAccessCount();

Reassign serial numbers such the serial numbers are sorted in order of most
frequently accessed.

 accessCount( const size_t& serialNumber ) const;

Returns access count of serial number I<serialNumber>.

=head1 AUTHOR

Paul B. Horton

#endif // ** end of pod **
