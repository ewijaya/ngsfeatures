/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.22
 *  Last Modified: $Date: 2008/06/27 09:44:37 $
 *  
 *  Purpose: pack up to 8 DNA bases in a single byte. Mapping characters to binary 00, 01, 10, 11.
 *
 *    Many logical error are checked for.
 *
 *      Trying to construct a DNAArrayBit16 with a length greater than maxLength (= 8 bases)
 *      throws a std::length_error.
 *
 *      Trying to access an element beyond the length of the array throws a
 *      std::out_of_range error. Giving an integer greater than maxResidueIndex (=3) for
 *      a base or an integer greater than 4^len-1 for a string of len bases
 *      (e.g. in setBases()) throws a std::invalid_argument error. Unless silenced by
 *      defining CBRC_QUIET_EXCEPTIONS, errors are printed to std:cerr before throwing.
 *
 *  Example Usage: DAArrayBit16<6> shortArray( DNAResidueIndexMap.getResidueIndices( "gactta" ) );
 *                 shortArray.substitute( 1, DNAResidueIndexMap.getResidue( 't' );
 *                 std::cout << shortArray; // prints "gtctta"
 *
 *  See also: ResidueIndexMap
 */

#ifndef _DNAARRAYBIT16_HH_
#define _DNAARRAYBIT16_HH_
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include <stdexcept>

namespace cbrc{


typedef unsigned char byte;
typedef unsigned short int DNAArrayBit16_bases8;

const DNAArrayBit16_bases8 DNAArrayBit16_setMask[] = {0x0003, 0x000c, 0x0030, 0x00c0, 0x0300, 0x0c00, 0x3000, 0xc000};
const DNAArrayBit16_bases8 DNAArrayBit16_clearMask[] = {0xfffc, 0xfff3, 0xffcf, 0xff3f, 0xfcff, 0xf3ff, 0xcfff, 0x3fff};
const DNAArrayBit16_bases8 DNAArrayBit16_maxBases[] = {0x0000, 0x0003, 0x000f, 0x003f, 0x00ff, 0x03ff, 0x0fff, 0x3fff, 0xffff};

const unsigned DNAArrayBit16_numPossible[] =
  { 0, 4, 16, 64, 256, 1024, 4096, 16384, 65536 };

const DNAArrayBit16_bases8 DNAArrayBit16_maxPossible[] =
  { 0, 3, 15, 63, 255, 1023, 4095, 16383, 65535 };

const byte DNAArrayBit16_maxResidueIndex = 3;
const byte DNAArrayBit16_maxLength = 8;


template<size_t len>
class DNAArrayBit16{
public:
  DNAArrayBit16(){ DNAArrayBit16( 0 ); }
  DNAArrayBit16( const DNAArrayBit16_bases8 bases ) throw( std::length_error, std::invalid_argument ) : bases(bases) {
    checkLen();
    checkBases( bases );
  }
  DNAArrayBit16( ResidueIndexMap::arrayT residueIndices /* binary values, \0, \1, not "a", "c"... */
		 ) throw( std::length_error ){
    checkLen();
    bases = 0;
    set( residueIndices );
  }
  DNAArrayBit16_bases8 getMaxBases() const{ return DNAArrayBit16_maxBases[len]; }
  DNAArrayBit16_bases8 getBases() const{ return bases; }
  void setBases( DNAArrayBit16_bases8 bs ) throw( std::invalid_argument ){
    checkBases( bs );
    bases = bs;
  }

  class const_iterator{
  public:
    const_iterator( DNAArrayBit16<len> a, byte inValidState ) : a(a), _inValidState(inValidState){}
    void operator++(){ 
      if( a.bases == DNAArrayBit16_maxPossible[len] )  _inValidState = false;
      else                                              ++a.bases;
    }
    static const_iterator first(){ return const_iterator( 0, true ); }
    bool inValidState() const{ return _inValidState; }
    //    bool operator!=( byte& it ){ 
    bool operator!=( const const_iterator& it ){ 
      if( inValidState() != it.inValidState() ) return true;
      return( a != it.a );
    }
    DNAArrayBit16<len> operator*() const{ return a; }
    operator size_t() const{ return a.bases; }
    DNAArrayBit16<len> a;
  private:
    byte _inValidState;
  };

  // automatic type conversion to size_t. Useful for use as array index.
  operator size_t() const{ return static_cast<size_t>(bases); }

  // == Does not throw exeception for non DNA string, say "zebra". Just returns false.
  bool operator==( const char* const s ) const{
    for( size_t i = 0; i < len; ++i ){
      if( !DNAProperResidueIndexMap.validResidue(s[i]) ) return false;
      if( (*this)(i) != DNAProperResidueIndexMap.toResidueIndex(s[i]) ) return false;
    }
    if( s[len] ) return false;
    return true;
  }
  void operator++() throw(std::invalid_argument){
    if( bases == DNAArrayBit16_numPossible[len] ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d incremented too far, past maxIndex = %d",
		__FILE__, __LINE__, DNAArrayBit16_maxResidueIndex);
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::invalid_argument( errorMessage )  );
    }
    ++bases;
  }
    
  DNAArrayBit16_bases8 operator() ( const size_t index ) const throw(std::out_of_range){
    checkIndex( index );
    DNAArrayBit16_bases8 retVal = bases & DNAArrayBit16_setMask[index];
    retVal >>= index; retVal >>= index;
    return retVal;
  }
  void set( ResidueIndexMap::arrayT residueIndices
	    /* binary values, \0, \1, not "a", "c"... */ ) throw( std::length_error ){
    if( residueIndices.size() != len ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d  residueIndices.size() = %lu, but should equal len = %lu", 
		__FILE__, __LINE__, residueIndices.size(), len );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::length_error( errorMessage )  );
    }
    bases = 0;
    for( size_t i = 0; i < length(); ++i ) set( i, residueIndices[i] );
  }
  void set( const size_t index, const byte base ) throw( std::out_of_range, std::invalid_argument ){
    checkIndex( index );
    checkBase( base );
    DNAArrayBit16_bases8 b = base;
    b <<= index; b <<= index;
    bases &= DNAArrayBit16_clearMask[index];
    bases |= b;
  }

  // substitute base into array at position index.
  DNAArrayBit16<len> substitute( const size_t index, const byte base ) const
                                                     throw( std::out_of_range, std::invalid_argument ){
    checkIndex( index );
    checkBase( base );
    DNAArrayBit16<len> retVal(bases);
    retVal.set( index, base );
    return retVal;
  }
  static std::string toResidues( DNAArrayBit16_bases8 bases ){
    return DNAArrayBit16<len>( bases ).getResidues();
  }
  std::string getResidues() const{ return DNAProperResidueIndexMap.toResidues( getIndices() ); }
  std::string getIndices() const{
    std::string retVal;
    retVal.resize( len );
    for( unsigned int i = 0; i < len; ++i )  retVal[i] = (*this)(i);
    return retVal;
  }
  size_t length() const{ return len; }
  friend class DNAArrayBit16Hamming;
  void friend testDNAArrayBit16();
private:

  // error checking routines.
  void checkLen() const throw( std::length_error ){
    if( len > DNAArrayBit16_maxLength ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d  length = %lu, but should not exceed %d", 
		__FILE__, __LINE__, len, DNAArrayBit16_maxLength );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::length_error( errorMessage )  );
    }
  }    
  void checkBases( const DNAArrayBit16_bases8 bs ) const throw( std::invalid_argument ){
    if( bs > DNAArrayBit16_maxBases[len] ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d  bases = %d, but should not exceed %d for len = %lu", 
		__FILE__, __LINE__, bs, DNAArrayBit16_maxBases[len], len );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::length_error( errorMessage )  );
    }
  }
  void checkBase( const byte base ) const throw( std::invalid_argument ){
    if( base > DNAArrayBit16_maxResidueIndex ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d  base index = %d, but should not exceed %d", 
		__FILE__, __LINE__, base, DNAArrayBit16_maxResidueIndex );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::invalid_argument( errorMessage )  );
    }
  }
  void checkIndex( const size_t& index ) const throw( std::out_of_range ){
    if( index >= len ){
      char errorMessage[256];
      snprintf( errorMessage, 256, "%s: line %d  index = %lu, but should not exceed len = %lu", 
		__FILE__, __LINE__, index, len );
#ifndef CBRC_QUIET_EXCEPTIONS
      std::cerr << errorMessage << std::endl;
#endif // not defined CBRC_QUIET_EXCEPTIONS
      throw(  std::out_of_range( errorMessage )  );
    }
  }      
  DNAArrayBit16_bases8 bases;
};


template<size_t len>
std::ostream& operator<<( std::ostream& os, const DNAArrayBit16<len>& dnaArray ){
  for( size_t i = 0; i < dnaArray.length(); ++i )  os << DNAProperResidueIndexMap.toResidue( dnaArray(i) );
  return os;
}

}; // end namespace
#endif // defined _DNAARRAYBIT16_HH_

