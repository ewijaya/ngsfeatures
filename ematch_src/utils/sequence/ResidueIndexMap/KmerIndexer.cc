/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2005.8.1
 *  Last Modified: $Date: 2008/07/19 00:07:16 $
 *
 *  Description: See header files.
 *
 *  Achtung!: This class appears to be unused 2008/07/19!
 */

#include "KmerIndexer.hh"

namespace cbrc{
  
  indexT KmerIndexer::toIndex( const ResidueIndexMap::indexT* const r,
			       const kmerLenT&                      k ) const{
#ifdef CBRC_DEBUG
    for( kmerLenT i = 0; i < k; ++i ) assert(  ResidueIndexMap::validResidueIndex( r[k] )  );
#endif //definned CBRC_DEBUG
    indexT retVal = 0;
    size_t multiplier = 1;
    for( kmerLenT i = k-1; i < k; --i ){
      retVal += multiplier * r[i];
      multiplier *= sigma;
    }
    return retVal;
};


}; // end namespace cbrc
