/*  
 *  Author: Please set BPLT_AUTHOR environment variable
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Please set BPLT_AUTHOR environment variable, All rights reserved.
 *  Creation Date: 2006.9.18
 *  Last Modified: $Date: 2006/09/17 23:28:28 $
 *
 *  Description: Allocator for fixed size ResidueIndexWindow.
 *
 */
#ifndef _RESIDUEINDEXWINDOWFACTORY_HH_
#define _RESIDUEINDEXWINDOWFACTORY_HH_
#include "../ResidueIndexMap.hh"
#include "ResidueIndexWindow.hh"
#include "ConstResidueIndexSequenceBoundaries.hh"

namespace cbrc{

class ResidueIndexWindowFactory{
public:
  ResidueIndexWindowFactory( const ResidueIndexMap& rim,
			     const ResidueIndexMap::indexT* const& startPtr,
			     const size_t& size ) :
    seqBoundaries( rim, startPtr, size ){}
  const ResidueIndexMap& residueIndexMap() const{ return seqBoundaries.residueIndexMap(); }
  const ResidueIndexMap::indexT* const startPtr() const{ return seqBoundaries.startPtr(); }
  const size_t endIdx() const{
    return seqBoundaries.endPtr() - seqBoundaries.startPtr();
  }
  ResidueIndexWindow getResidueIndexWindow( const size_t& windowStartIdx,
					    const size_t& windowSize ){
    return(  ResidueIndexWindow( &seqBoundaries,
				 windowStartIdx,
				 windowStartIdx + windowSize )  );
  }
private:
  const ConstResidueIndexSequenceBoundaries seqBoundaries;
};

}; // end namespace cbrc
#endif // defined _RESIDUEINDEXWINDOWFACTORY_HH_

