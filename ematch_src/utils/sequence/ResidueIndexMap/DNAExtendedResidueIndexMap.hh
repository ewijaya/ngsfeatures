/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.23
 *  Last Modified: $Date: 2007/09/07 05:43:59 $
 *
 *  Description: Represent extended DNA alphabet which includes "n" for missing
 *               or masked bases.
 * 
 *  CAVEAT: Achtung!! This class is deprecated. It has been replaced by a
 *          constant in ResidueIndexMapPredefinedAlphabets.hh
 *
 */
#ifndef _DNAEXTENDEDRESIDUEINDEXMAP_HH
#define _DNAEXTENDEDRESIDUEINDEXMAP_HH
#define CBRC_DNA_EXTENDED_ALPHABET "acgtn"
#include <iostream>
#include "ResidueIndexMap.hh"

namespace cbrc{

class DNAExtendedResidueIndexMap : public ResidueIndexMap{
public:
  DNAExtendedResidueIndexMap()
    : ResidueIndexMap( CBRC_DNA_ALPHABET_PROPER, CBRC_DNA_EXTENDED_ALPHABET ){}
  bool isProperResidue( const char residue ) const;
  bool knownValidIsProperResidueIndex( const ResidueIndexMap::indexT& residue ) const{
    return( residue < CBRC_DNA_ALPHABET_SIZE );
  }
private:
};


inline bool DNAExtendedResidueIndexMap::isProperResidue( const char residue ) const{
  if( residue == 'a' ) return true;
  if( residue == 'c' ) return true;
  if( residue == 'g' ) return true;
  if( residue == 't' ) return true;
  return false;
}


}; // end namespace cbrc
#endif // _DNAEXTENDEDRESIDUEINDEXMAP_HH

