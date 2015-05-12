/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.12
 *  Last Modified: $Date: 2009/09/12 12:17:22 $
 *
 *  Description: Class to represent index into a sigma4 bit packed array.
 *
 */
#ifndef SIGMA4BITPACKEDIDX_HH_
#define SIGMA4BITPACKEDIDX_HH_
#include <iostream>
#include "utils/universalTypedefs.hh"
#include "sigma4.hh"

namespace cbrc{

class Sigma4bitPackedIdx{
public:
  /* ---------- CONSTRUCTORS ---------- */
  Sigma4bitPackedIdx(){}  // null constructor does nothing.

  Sigma4bitPackedIdx(   const Sigma4bitPackedIdx&  objectToCopy  )
    : _byteIdx  (  objectToCopy.byteIdx()    ),
      _posInByte(  objectToCopy.posInByte()  )
  {}

  Sigma4bitPackedIdx(  const size_t&  unpackedIdx  )
    : _byteIdx  (  unpackedIdx / SIGMA  ),
      _posInByte(  unpackedIdx % SIGMA  )
  {}

  void operator++(){
    if(  ++_posInByte  ==  SIGMA  ) {   _posInByte = 0;  ++_byteIdx;   }
  }      

    

  /* ---------- ACCESSORS ------------- */
  const size_t&  byteIdx  () const {  return _byteIdx;    }
  const byte&    posInByte() const {  return _posInByte;  }
private:
  // object data
  size_t  _byteIdx;
  byte    _posInByte;
};


inline std::ostream& operator<<( std::ostream& oStream, const Sigma4bitPackedIdx& packedIdx ){

  oStream  <<  "("  <<             packedIdx.byteIdx()
	   <<  ","  <<  (unsigned) packedIdx.posInByte()  << ")";

  return oStream;
}

} // end namespace cbrc
#endif // SIGMA4BITPACKEDIDX_HH_
