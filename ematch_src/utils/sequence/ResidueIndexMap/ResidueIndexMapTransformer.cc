/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.11
 *  Last Modified: $Date: 2008/08/11 11:22:01 $
 *  Description: See header file.
 */
#include <algorithm>
#include <numeric>
#include "utils/gdb/gdbUtils.hh"
#include "ResidueIndexMapTransformer.hh"
#define UNINITIALIZED 255

namespace cbrc{

ResidueIndexMapTransformer::
ResidueIndexMapTransformer(  const ResidueIndexMap& origResidueIndexMap,
			     const ResidueIndexMap& tranResidueIndexMap,
			     const std::string& origAscii,
			     const std::string& tranAscii  )
  : origResidueIndexMap(origResidueIndexMap),
    tranResidueIndexMap(tranResidueIndexMap),
    origAscii(origAscii),
    tranAscii(tranAscii)
{
  ResidueIndexMap::arrayT origIndexed(  origResidueIndexMap.toResidueIndices( origAscii )  );
  ResidueIndexMap::arrayT tranIndexed(  tranResidueIndexMap.toResidueIndices( tranAscii )  );
  
  ResidueIndexMap::indexT origMaxIndex = origIndexed.max();

  GDB_ASSERTF( origMaxIndex < UNINITIALIZED,
	       "unitialized marker value (%u), not compatible with max index (%u)",
	       (unsigned) origMaxIndex, (unsigned) UNINITIALIZED );

  origToTran.setSize(  origIndexed.max() + 1 /* 'cause C counts from zero */  );

  origToTran.fill( UNINITIALIZED );

  for( size_t i = 0; i < origIndexed.size(); ++i ){
    origToTran[ origIndexed(i) ] = tranIndexed(i);
  }
} // end constructor ResidueIndexMapTransformer( const ResidueIndexMap&,,,, )



inline void
ResidueIndexMapTransformer::
transformInPlace( ResidueIndexMap::indexT& indexToTransform ){

  GDB_ASSERTF( origToTran(indexToTransform) != UNINITIALIZED,
	       "transformation of index (%u) not defined",
	       (unsigned) indexToTransform );

  indexToTransform = origToTran(indexToTransform);
}


void
ResidueIndexMapTransformer::
transformInPlace( ResidueIndexMap::arrayT& indicesToTransform ){

  for(  ResidueIndexMap::arrayT::iterator iter = indicesToTransform.begin();
        iter != indicesToTransform.end();
        ++iter  ){
    transformInPlace( *iter );
  }
}

  

} // end namespace cbrc

