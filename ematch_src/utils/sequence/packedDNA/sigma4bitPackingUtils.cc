/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.12
 *  Last Modified: $Date: 2009/09/13 07:35:56 $
 *
 *  Description: See header file.
 *
 */
#include <iostream>
#include "sigma4bitPackingUtils.hh"



namespace cbrc{


  /* -------------------- Constants -------------------- */

  static const byte  mask[]  =  {0xc0, 0x30, 0x0c, 0x03};



  /* -------------------- Functions -------------------- */

  void sigma4bitPackingUtils::pack(   /***/ byte* const&   dest,
				      const unpackedVecT&  source   ){

    byte* destCurBytePtr  =  dest;
    
    const byte* const  destFullByteEnd  =  dest + source.size() / SIGMA;

    // zero dest memory
    memset(   dest,  0,  numBytesNeededToStore( source.size() )   );

    unpackedVecT::const_iterator  sourceCurBytePtr  =  source.begin();

    /* ----- pack and copy source ----- */
    while(  destCurBytePtr  <  destFullByteEnd  ){
      *destCurBytePtr  |=  ((*sourceCurBytePtr++) <<6);
      *destCurBytePtr  |=  ((*sourceCurBytePtr++) <<4);
      *destCurBytePtr  |=  ((*sourceCurBytePtr++) <<2);
      *destCurBytePtr  |=   *sourceCurBytePtr++;
      ++destCurBytePtr;
  }

  /* ----- Finish Filling in Remaining 0-3 Elements ----- */

  if(  sourceCurBytePtr == source.end()  )  return;  // EXIT
  *destCurBytePtr  |=  ((*sourceCurBytePtr++) <<6);

  if(  sourceCurBytePtr == source.end()  )  return;  // EXIT
  *destCurBytePtr  |=  ((*sourceCurBytePtr++) <<4);

  if(  sourceCurBytePtr == source.end()  )  return;  // EXIT
  *destCurBytePtr  |=  ((*sourceCurBytePtr++) <<2);

  GDB_ASSERTF(  sourceCurBytePtr == source.end(),
		"Programming error"  );
} // end sigma4bitPackingUtils::pack( dest, source )



void sigma4bitPackingUtils::unpack( /***/ unpackedVecT&  dest,
				    const byte* const&   source,
				    const size_t&        size   ){
  dest.resize( size );

  
  Sigma4bitPackedIdx packedIdx(0);  
  for(   unpackedVecT::iterator destPtr  =  dest.begin();
	 destPtr  <  dest.end();
	 ++destPtr, ++packedIdx   ){
    assignNthUnpacked( *destPtr, source, packedIdx );
  }
}


// assign Nth byte value to destByte
void sigma4bitPackingUtils::assignNthUnpacked(   /***/ byte&                destByte,
						 const byte* const&         source,
						 const Sigma4bitPackedIdx&  packedIdx   ){


  // extract value.
  destByte  =  (  source[ packedIdx.byteIdx() ]  &  mask[ packedIdx.posInByte() ]  );

  // shift values as necessary, fallthrough in this switch statement is intentional.
  switch( packedIdx.posInByte() ){
  case 0:  destByte  =  (destByte >>2);
  case 1:  destByte  =  (destByte >>2);
  case 2:  destByte  =  (destByte >>2);
  }

} // end sigma4bitPackingUtils::assignNthUnpacked( destByte, source, packedIdx )

} // end namescape cbrc

