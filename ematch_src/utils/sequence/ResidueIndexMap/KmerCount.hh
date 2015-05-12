/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.8.1
 *
 *  Last Modified: $Date: 2008/07/19 00:07:28 $
 *  
 *  Description: Frequency counts of substings up to length k.
 *
 *  Purpose: Originally made for a higher order background model for motif discovery.
 *
 *  Achtung!: This class appears to be unused 2008/07/19!
 *
 */

#ifndef _KMERCOUNT_HH_
#define _KMERCOUNT_HH_
#include "ResidueIndexMap.hh"
#include "KmerIndexer.hh"

namespace cbrc{

class KmerCount{
public:
  KmerCount( const kmerLenT k, std::vector<ResidueIndexMap::residueArrayT> seqs ) : k(k){
    for( std::vector<ResidueIndexMap::residueArrayT>::const_iterator s = seqs.begin();
	 s != seqs.end(); ++s ){
      add( s );
    }
  }
  size_t add( const ResidueIndexMap::residueArrayT& ); // put body in KmerCount.cc
  size_t count( const ResidueIndexMap::indexT* const ResidueIndexMap::indexT, const unsigned int substringLen );
  static size_t substring
  friend void testKmerCount();
private:
  kmerLenT k;
};

}; // end namespace cbrc
#endif // defined _KMERCOUNT_HH_

