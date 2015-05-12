/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2005.8.1
 *  Last Modified: $Date: 2008/07/19 00:07:06 $
 *  
 *  Description: Map k-mer substrings to non-negative integer indicies.
 *
 *  Purpose: Created for motif discovery programs.
 *
 *  Achtung!: This class appears to be unused 2008/07/19!
 */

#ifndef _KMERINDEXER_HH_
#define _KMERINDEXER_HH_
#include "ResidueIndexMap.hh"

namespace cbrc{

class KmerIndexer{
public:
  typedef unsigned char kmerLenT;
  typedef size_t        indexT;

  KmerIndexer( const indexT& sigma ) : sigma(sigma){}

  indexT toIndex( const ResidueIndexMap::indexT* const r,
		  const kmerLenT&                      k ) const;

  std::string toIndexResidues( const indexT& kmerIndex ) const;

  const indexT sigma;
};

}; // end namespace cbrc
#endif // defined _KMERINDEXER_HH_

