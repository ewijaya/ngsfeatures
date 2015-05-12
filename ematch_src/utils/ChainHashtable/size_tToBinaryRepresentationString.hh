/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Based on: function int2bin by "Kishan", found on the internet.
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.8
 *  Last Modified: $Date: 2008/06/08 04:19:17 $
 *
 *  Description: binary string representation of a size_t variable
 *
 *  Purpose: Created to test hashtable StringKeyHashtable
 *
 */
#ifndef _SIZE_TTOBINARYREPRESENTATIONCSTRING_HH
#define _SIZE_TTOBINARYREPRESENTATIONCSTRING_HH
#include <iostream>
#include <string>

namespace cbrc{


std::string size_tToBinaryRepresentationString( size_t a ){
  const size_t numBitsPerByte = 8;
  size_t wordBitSize  =  numBitsPerByte * sizeof(size_t);

  // initialize all bits to '0'
  std::string retval( wordBitSize, '0' );

  size_t curBit = wordBitSize - 1;

  // fill in '1' bits, in order of increasingly significant bits
  while( a > 0 ){

    if(  ( a%2 == 1 )  ){
      retval[curBit] = '1';
    }

    --curBit;
    a /= 2;
  }

  return retval;
}


} // end namespace cbrc
#endif // _SIZE_TTOBINARYREPRESENTATIONCSTRING_HH
