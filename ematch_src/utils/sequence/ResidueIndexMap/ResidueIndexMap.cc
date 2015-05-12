/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul Horton, All rights reserved.
 *  Last Modified: $Date: 2009/06/22 21:08:14 $
 *
 *  Purpose: See header file.
 *
 */
#include "utils/stl/stringBinaryIO.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMap.hh"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ctype.h>

namespace cbrc{


/* *************** Constructors *************** */

  ResidueIndexMap::ResidueIndexMap( const std::string& alphabet,
				    const bool& caseSensitive )
    throw( std::invalid_argument )
    : _alphabetProper(alphabet),
      _alphabet(alphabet),
      _caseSensitive(caseSensitive)
  {
    _assertAlphabetCharactersUnique();
  }


  ResidueIndexMap::ResidueIndexMap( const char* const& alphabetProper,
				    const char* const& alphabet,
				    const bool& caseSensitive )
    throw( std::invalid_argument )
    : _alphabetProper(alphabetProper),
      _alphabet(alphabet),
      _caseSensitive(caseSensitive)
  {
    _assertAlphabetProperIsPrefix();
    _assertAlphabetCharactersUnique();
  }


  ResidueIndexMap::ResidueIndexMap( const std::string& alphabetProper,
				    const std::string& alphabet,
				    const bool& caseSensitive
				    ) throw( std::invalid_argument )
    : _alphabetProper(alphabetProper),
      _alphabet(alphabet),
      _caseSensitive(caseSensitive)
  {
    _assertAlphabetProperIsPrefix();
    _assertAlphabetCharactersUnique();
  }



/* *************** Modifying methods *************** */

// add residue to end of alphabet, see pod for more
void ResidueIndexMap::extendAlphabet( const char& residue ) throw( std::invalid_argument ){
  // if residue already in alphabet
  // if it is the last residue do nothing, otherwise exit with error message
  if(   validResidue( residue )    ){
    if( !residuesAreEquivalent( residue, alphabet()[alphabet().size()-1] ) ){
      throw std::invalid_argument( "residue already present in alphabet" );
    }
    // else if residue is the last residue in the alphabet do nothing
    // this makes repeated identical calls to extendAlphabet idempotent
    return;
  }
  _alphabet.push_back( residue );
}



/* ********** Predicates about residues and residue indices ********** */

// confirm residue is in alphabet by linear scan
bool ResidueIndexMap::validResidue( const char& residue ) const{
  
  for( size_t i = 0; i < alphabet().size(); ++i ){
    if(   residuesAreEquivalent( residue, alphabet()[i] )   ){
      return true;
    }
  }
  return false;
}


bool ResidueIndexMap::validResidues( const std::string& residues ) const{

  for( size_t i = 0; i < residues.size(); ++i ){
    if( !validResidue( residues[i] ) )  return false;
  }
  return true;
}


// confirm residue is in alphabetProper by linear scan
bool ResidueIndexMap::properResidue( const char& residue ) const{

  for( size_t i = 0; i < alphabetProper().size(); ++i ){
    if(   !residuesAreEquivalent( residue, alphabetProper()[i] )   ){
      return false;
    }
  }
  return true;
}


bool ResidueIndexMap::properResidues( const std::string& residues ) const{

  for( size_t i = 0; i < residues.size(); ++i ){
    if( !properResidue( residues[i] ) )  return false;
  }
  return true;
}


// find index corresponding to residue by linear scan of alphabet,
// or throw exception if residue is not a valid residue.
ResidueIndexMap::indexT
ResidueIndexMap::toResidueIndex( const char& residue ) const throw( std::invalid_argument ){

  for( indexT i = 0; i < alphabet().size(); ++i ){
    if(   residuesAreEquivalent( residue, alphabet()[i] )   ){
      return i;
    }
  }

  // else throw error
  throw std::invalid_argument( std::string("invalid residue char: ")
			       + residue );
}



char ResidueIndexMap::toResidue( const indexT& residueIndex ) const throw( std::invalid_argument ){

  // throw error residueIndex out of range
  if( residueIndex >= alphabet().size() ){
    throw
      std::invalid_argument( std::string( "residue index:" )
			     + boost::lexical_cast<std::string>( (size_t) residueIndex )
			     + std::string( " ≧ alphabet().size():" )
			     + boost::lexical_cast<std::string>( alphabet().size() )
			     );
  }

  // else return apropriate residue
  return alphabet()[residueIndex];
}



ResidueIndexMap::vectorT
ResidueIndexMap::toResidueIndicesVector( const std::string& residues ) const throw( std::invalid_argument ){

  vectorT retVal( residues.length() );
  for( size_t i = 0; i < residues.length(); ++i ){
    retVal[i] = toResidueIndex( residues[i] );
  }
  return retVal;
}



ResidueIndexMap::arrayT
ResidueIndexMap::toResidueIndices( const std::string& residues ) const throw( std::invalid_argument ){

  arrayT retVal( residues.length() );
  for( size_t i = 0; i < residues.length(); ++i ){
    retVal[i] = toResidueIndex( residues[i] );
  }
  return retVal;
}



void ResidueIndexMap::assignResidueIndices( arrayT& residueIndices,
					    const std::string& residues ) const
  throw( std::invalid_argument ){

  residueIndices.setSize( residues.size() );
  for( size_t i = 0; i < residues.length(); ++i ){
    residueIndices[i] = toResidueIndex( residues[i] );
  }
}


void ResidueIndexMap::assignResidueIndices( vectorT& residueIndicesVec,
					    const std::string& residues ) const
  throw( std::invalid_argument ){

  residueIndicesVec.clear();
  residueIndicesVec.reserve( residues.size() );

  for(  std::string::const_iterator  itr  =  residues.begin();
	itr  !=  residues.end();
	++itr  ){
    residueIndicesVec.push_back(  toResidueIndex( *itr )  );
  }
}


void
ResidueIndexMap::assignResidueIndices( const arrayT::iterator& residueIndicesBegin,
				       const std::string::const_iterator& residuesBegin,
				       const std::string::const_iterator& residuesEnd ) const
  throw( std::invalid_argument ){

  arrayT::iterator curResidueIndexPtr = residueIndicesBegin;
  for( std::string::const_iterator curResiduePtr = residuesBegin;
       curResiduePtr < residuesEnd;
       ++curResiduePtr ){
    *curResidueIndexPtr = toResidueIndex( *curResiduePtr );
    ++curResidueIndexPtr;
  }
}


std::string
ResidueIndexMap::toResidues( const std::string& residueIndices ) const throw( std::invalid_argument ){
  std::string retVal;
  retVal.resize( residueIndices.length() );
  for( size_t i = 0; i < residueIndices.length(); ++i ){
    retVal[i] = toResidue( residueIndices[i] );
  }
  return retVal;
}


std::string
ResidueIndexMap::vectorToResidues( const ResidueIndexMap::vectorT& residueIndices ) const throw( std::invalid_argument ){
  std::string retVal;
  retVal.resize( residueIndices.size() );
  for( size_t i = 0; i < residueIndices.size(); ++i ){
    retVal[i] = toResidue( residueIndices[i] );
  }
  return retVal;
}


  
std::string ResidueIndexMap::toResidues( const arrayT& residueIndices ) const throw( std::invalid_argument ){
  std::string retVal;
  retVal.resize( residueIndices.size() );
  for( size_t i = 0; i < residueIndices.size(); ++i ){
    retVal[i] = toResidue( residueIndices[i] );
  }
  return retVal;
}


std::string ResidueIndexMap::toResidues( const arrayT::const_iterator& residueIndicesBegin,
					 const arrayT::const_iterator& residueIndicesEnd )
  const throw( std::invalid_argument ){
  std::string retVal;
  retVal.resize( residueIndicesEnd - residueIndicesBegin );

  std::string::iterator curResiduePtr = retVal.begin();
  for( arrayT::const_iterator curResidueIndexPtr = residueIndicesBegin;
       curResidueIndexPtr < residueIndicesEnd;
       ++curResidueIndexPtr ){
    *curResiduePtr = toResidue( *curResidueIndexPtr );
    ++curResiduePtr;
  }
  return retVal;
}


// return true if residue0 == residue1  (possibly ignoring case)
bool ResidueIndexMap::residuesAreEquivalent( const char& residue0, const char& residue1 ) const{
  if( caseSensitive() ){
    return(   (residue0 == residue1)   );
  }

  // else do case insensitive comparison
  char r0 = tolower( residue0 );
  char r1 = tolower( residue1 );
  return(   (r0 == r1)   );
}



/* ********** Binary I/O ********** */

void ResidueIndexMap::read( std::istream& is ) throw( std::invalid_argument ){

  // calling object should be empty
  assert( !alphabet().size() );

  
  /* ******* read in signature and confirm if it is correct ******* */

  std::string signatureReadFromStream;

  stringBinaryIO::read( signatureReadFromStream, is, binaryStreamSignature().size() );

  if(   signatureReadFromStream != binaryStreamSignature()   ){
    perlish::die( __FILE__, __LINE__, "Signature read from input stream does not match\n" );
  }


  /* ******* read in object data ******* */

  // read in alphabetProper and alphabet
  stringBinaryIO::read( _alphabetProper, is );
  stringBinaryIO::read( _alphabet, is );

  // read in caseSensitive
  is.read( (char*) &_caseSensitive, sizeof(_caseSensitive) );
  if( is.fail() ){
    perlish::die( __FILE__, __LINE__, "Failed to read caseSensitive\n" );
  }

  // check validity
  _assertAlphabetProperIsPrefix();
  _assertAlphabetCharactersUnique();
}



void ResidueIndexMap::write( std::ostream& os ) const{

  // write contents (but not) size of binaryStreamSignature
  stringBinaryIO::writeContentsOnly( binaryStreamSignature(), os );

  // write size and contents of alphabetProper and alphabet
  stringBinaryIO::write( alphabetProper(), os );

  stringBinaryIO::write( alphabet(), os );
  
  // write caseSensitive
  os.write( (char*) &_caseSensitive, sizeof(_caseSensitive) );

  if( os.fail() ){
    perlish::die( __FILE__, __LINE__, "Failed to write caseSensitive\n" );
  }
}


/* ********** Other methods ********** */

std::vector<ResidueIndexMap::vectorT>
ResidueIndexMap::splitIntoProperOnlySubstrings( const vectorT& residueIndexVector ) const{

  std::vector<vectorT> retval;
  size_t substringStart = 0;

  while( true ){
    // advance substringStart to next proper residue.

    for(  ; /* no initialization */ 
	  (substringStart < residueIndexVector.size())
	    && !properResidueIndex( residueIndexVector[substringStart] );
	  ++substringStart );

    if( substringStart >= residueIndexVector.size() )  return retval;

    // else residueIndexVector[substringStart] is a proper residue.
    size_t substringEnd = substringStart + 1;
    // advance substringEnd to next extra residue.

    for(  ; /* no initialization */
	  (substringEnd < residueIndexVector.size())
	    && properResidueIndex( residueIndexVector[substringEnd] );
	  ++substringEnd );

    retval.push_back( vectorT(residueIndexVector.begin() + substringStart,
			      residueIndexVector.begin() + substringEnd) );
    if( substringEnd >= residueIndexVector.size() )  return retval;
    substringStart = substringEnd + 1;
  }
}



/* *************** Private Methods *************** */

// throw exception if alphabetProper is not a prefix of alphabet.
void ResidueIndexMap::_assertAlphabetProperIsPrefix() const throw( std::invalid_argument ){

  if( alphabetProper().size() > alphabet().size() ){
    throw std::invalid_argument( "alphabetProper bigger than alphabet" );
  }

  for( size_t i = 0; i < alphabetProper().size(); ++i ){
    if( alphabetProper()[i] != alphabet()[i] ){
      throw std::invalid_argument( "alphabetProper is not a prefix of alphabet" );
    }
  }
}



// throw exception if the characters in alphabet are not unique
// behavior depends on caseSensitive()
void ResidueIndexMap::_assertAlphabetCharactersUnique() const throw( std::invalid_argument ){

  std::string sortedAlphabet( alphabet() );

  // sort the alphabet so that if duplicate characters are present,
  // they will be adjacent to each other.
  if( caseSensitive() ){
    std::sort( sortedAlphabet.begin(), sortedAlphabet.end() );
  }
  else{
       std::sort( sortedAlphabet.begin(), sortedAlphabet.end(), _caseInsensitiveSortPredicate );
  }


  // check for duplicate characters

  for( size_t i = 1; i < sortedAlphabet.size(); ++i ){
    if(   residuesAreEquivalent( sortedAlphabet[i-1], sortedAlphabet[i] )   ){
      throw std::invalid_argument( "characters in alphabet are not unique" );
    }
  }
}



bool ResidueIndexMap::_caseInsensitiveSortPredicate( const char& c0, const char& c1 ){

  char lc0 = tolower( c0 );
  char lc1 = tolower( c1 );

  return(  lc0 < lc1  );
}



/* ********** Functions using ResidueIndexMap ********** */

std::ostream& operator<<( std::ostream& os, const ResidueIndexMap& rim ){

  os << rim.alphabetProper() << ", " << rim.alphabet()
     << ", case " 
     << ( rim.caseSensitive()? "" : "in" )
     << "sensitive";
  return os;
}

} // end namespace cbrc
