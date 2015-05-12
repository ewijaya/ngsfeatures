/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.12
 *  Last Modified: $Date: 2009/09/13 07:35:49 $
 *
 *  Description: Functions to bit pack strings with alphabet size 4
 *
 */
#ifndef SIGMA4BITPACKINGUTILS_HH_
#define SIGMA4BITPACKINGUTILS_HH_
#include <cmath>
#include <iostream>
#include <vector>
#include <cstring> // for memset
#include "utils/universalTypedefs.hh"
#include "utils/gdb/gdbUtils.hh"
#include "Sigma4bitPackedIdx.hh"



namespace cbrc{



namespace sigma4bitPackingUtils{
  
  /* --------------- Typedefs --------------------------- */

  // idxT.first = byte index,  idxT.second = position in byte
  typedef  std::pair<size_t, byte>          idxT;
  typedef  std::vector<byte>        unpackedVecT;


  /* --------------- Interface Definition --------------- */

  // pack contents of SOURCE into memory pointed to by DEST.
  void pack(   /***/ byte* const&   dest,
	       const unpackedVecT&  source   );


  // unpack SIZE bytes of contents pointed to by SOURCE and assign to DEST.
  void unpack(   /***/ unpackedVecT&  dest,
	         const byte* const&   source,
		 const size_t&        size   );

  inline
  unpackedVecT  toUnpacked(   const byte* const&  source,
			      const size_t&       size   ){
    unpackedVecT retVal;
    unpack( retVal, source, size );
    return retVal;
  }


  // assign packedIdx element of source to destByte
  void assignNthUnpacked(   /***/ byte&                destByte,
			    const byte* const&         source,
			    const Sigma4bitPackedIdx&  packedIdx   );

  // assign Nth element of source to destByte
  inline void assignNthUnpacked(   /***/ byte&         destByte,
				   const byte* const&  source,
				   const size_t&       n   ){
    assignNthUnpacked(  destByte,  source,  Sigma4bitPackedIdx( n )  );
  }


  /* --------------- Public Class Methods ------------- */

  static inline size_t  numBytesNeededToStore( const size_t& numElems ){
    return(   (size_t)  ceil(  (double) numElems / SIGMA  )   );
  }

  /* --------------- Internal Functions --------------- */

}


} // end namespace cbrc
#endif // SIGMA4BITPACKINGUTILS_HH_
