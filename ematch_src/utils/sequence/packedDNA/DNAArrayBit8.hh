/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.4
 *  Last Modified: $Date: 2008/06/27 12:41:48 $
 *  
 *  Purpose: pack up to 4 DNA bases in a single byte. Mapping characters to binary 00, 01, 10, 11.
 *
 *  This class is not being maintained right now. Please see DNAArrayBit16 which is.
 *  This class does work but the exception handling has problems, one of which is relying on the
 *  conincidental fact that the number of bases (4) is the same as the maximum length of the array
 *  8 bit byte / 2 bits per base.
 *
 *  Usage: DAArrayBit8<3> shortArray( rim.getResidueIndices( "gac" ) );
 *
 *  See also: DNAArrayBit16, ResidueIndexMap
 */

#ifndef _DNAARRAYBIT8_HH_
#define _DNAARRAYBIT8_HH_
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include <stdexcept>

namespace cbrc{

typedef unsigned char byte;
static const byte setMask[] = {0x03, 0x0c, 0x30, 0xc0};
static const byte clearMask[] = {0xfc, 0xf3, 0xcf, 0x3f};
static const byte maxResidueIndex = 3;

template<size_t len>
class DNAArrayBit8{
public:
  DNAArrayBit8(){ DNAArrayBit8( 0 ); }
  DNAArrayBit8( byte bases ) throw( std::length_error ) : bases(bases) {
    if( len > 4 ) throw(  std::length_error( "length of DNAArrayBit8 cannot exceed 4" )  );
  }
  DNAArrayBit8( ResidueIndexMap::arrayT residueIndices
		/* binary values, \0, \1, not "a", "c"... */ ) throw( std::length_error ){
    if( residueIndices.size() != length() ){
      std::cerr << "residueIndices.size() = " << residueIndices.size() << " != ";
      std::cerr << "length() = " << length();
      throw std::length_error( "residueIndices.size() != length()" );
    }
    bases = 0;
    for( size_t i = 0; i < length(); ++i ) set( i, residueIndices[i] );
  }
  byte const getBases(){ return bases; }
  void setBases( byte bs ){ bases = bs; }

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
    
  byte operator() ( const size_t index ) const throw(std::out_of_range){
    if( index >= length() ){
      std::cout << "index: " << index << " >= length(): " << length() << std::endl;
      throw std::out_of_range( "index >= length()" );
    }
    byte retVal = bases & setMask[index];
    retVal >>= index; retVal >>= index;
    return retVal;
  }
  void set( const size_t index, const byte base ) throw(std::out_of_range,std::invalid_argument){
    if( index >= length() ){
      std::cout << "index: " << index << " >= length(): " << length() << std::endl;
      throw std::out_of_range( "index >= length()" );
    }
    if( base > maxResidueIndex ){
      std::cout << "base: " << static_cast<int>(base);
      std::cout << " >= maxResidueIndex: " << static_cast<int>(maxResidueIndex) << std::endl;
      throw std::invalid_argument( "base > maxResidueIndex" );
    }
    byte b = base;
    b <<= index; b <<= index;
    bases &= clearMask[index];
    bases |= b;
  }
  DNAArrayBit8<len> substitute( const size_t index, const byte base ) const{
    DNAArrayBit8<len> retVal(bases);
    retVal.set( index, base );
    return retVal;
  }
  std::string getIndices() const{
    std::string retVal;
    retVal.resize( len );
    for( unsigned int i = 0; i < len; ++i )  retVal[i] = (*this)(i);
    return retVal;
  }
  size_t length() const{ return len; }
  void friend testDNAArrayBit8();
private:
  byte bases;
};


template<size_t len>
std::ostream& operator<<( std::ostream& os, const DNAArrayBit8<len>& dnaArray ){
  for( size_t i = 0; i < dnaArray.length(); ++i )  os << DNAProperResidueIndexMap.toResidue( dnaArray(i) );
  return os;
}

}; // end namespace
#endif // defined _DNAARRAYBIT8_HH_

