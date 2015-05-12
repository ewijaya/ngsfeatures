/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2005-2007, Paul B. Horton, All rights reserved.
 *  Copyright: All rights reserved
 *  Creation Date: 2005.12.7
 *  Last Modified: $Date: 2008/08/14 22:47:31 $
 *  
 *  Description: Bijection from residueIndex strings to successive integers
 *               from 0 to sigma^k - 1, for string of length k < maxStringLen() where
 *               maxStringLen() is determined by sigma and the size of
 *               ResidueIndexMap::indexT.
 *
 *               The mapping is defined by interpreting the length k
 *               residueIndex string as a base sigma length k integer.
 *
 *               The most significant base sigma digit (sigit) is
 *               defined to be the first character in the
 *               string. Equivalently, the least significant sigit is
 *               the last character in the string.
 *
 *               Note this class maps a ResidueIndex string onto a single 
 *               integer. This is different from ResidueIndexMap which maps a
 *               string of ResidueIndex onto a string of integers.
 *
 *  Purpose: Created for use with motif discovery programs.
 *
 */
#ifndef _RESIDUESINDEXMAP_HH_
#define _RESIDUESINDEXMAP_HH_

#include "ResidueIndexMapPredefined.hh"
#include "ResidueIndexWindow/ResidueIndexWindow.hh"

namespace cbrc{

class ResiduesIndexMap{
public:
  /* ******* Constructor and typedefs ******* */
  typedef unsigned int indexT;

  ResiduesIndexMap( const ResidueIndexMap& rim );


  /* ******* operator(), to map string to integer ******* */
  indexT operator()( const ResidueIndexMap::arrayT& residueIndices ) const{
    return this->operator()( &residueIndices[0], residueIndices.size() );
  }
  indexT operator()( const ResidueIndexMap::indexT* const& r,
		     const size_t&  stringLen ) const;

  indexT operator()( const ResidueIndexWindow& riw ) const{
    return(  this->operator()( riw.windowStartPtr(), riw.size() )  );
  }

  indexT operator()( const std::string& residues ) const;


  /* ******* Methods to map integer to string. Opposite of operator() ******* */
  // returns residueIndices of string
  ResidueIndexMap::vectorT toResidueIndicesVector( const ResiduesIndexMap::indexT& idx, 
						   const size_t&  stringLen ) const;

  // returns actual residues of string
  std::string toResidues( const indexT& residuesIndex, const size_t& stringLen ) const;



  /* ******* Accessors ******* */
  const size_t&          maxStringLen()    const{ return _maxStringLen; }
  size_t                 sigma()           const{ return rim.sigma(); }
  const ResidueIndexMap& residueIndexMap() const{ return rim; }

private:
  /* ******* Private methods ******* */

  // Return longest string length for with the index will not numerically overflow.
  size_t _computeMaxStringLen() const; 

  // Return sigma raised to y power.
  indexT _sigmaRaisedToPower( size_t y ) const{
    indexT retVal;
    for( retVal = 1; y > 0; --y )  retVal *= sigma();
    return retVal;
  }

  /* ******* Object data ******* */
  const ResidueIndexMap& rim;
  size_t _maxStringLen;
};


const ResiduesIndexMap DNAProperResiduesIndexMap( DNAProperResidueIndexMap );
const ResiduesIndexMap RNAProperResiduesIndexMap( RNAProperResidueIndexMap );
const ResiduesIndexMap aminoResiduesIndexMap(     aminoResidueIndexMap );

inline const ResiduesIndexMap& residuesIndexMapFromName( const std::string& seqTypeName ){
  if( seqTypeName == "DNA" )  return DNAProperResiduesIndexMap;
  if( seqTypeName == "dna" )  return DNAProperResiduesIndexMap;
  if( seqTypeName == "RNA" )  return RNAProperResiduesIndexMap;
  if( seqTypeName == "rna" )  return RNAProperResiduesIndexMap;
  if( seqTypeName == "amino" )  return aminoResiduesIndexMap;
  std::cout << "Error: unknown sequence type name: \"" << seqTypeName << "\"\n";
  exit( -1 );
}


} // end namespace cbrc
#endif // defined _RESIDUESINDEXMAP_HH_
