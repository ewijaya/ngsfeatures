/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.22
 *  Last Modified: $Date: 2008/11/23 00:16:30 $
 *
 *  Description: generic hash functions
 *
 */
#ifndef GENERICHASHFUNCTION_HH_
#define GENERICHASHFUNCTION_HH_


// Generic hash function.
// Return hash value of length numBytes object in memory pointed to by A
inline size_t genericHashValue( const size_t& numBytes, size_t const* a){
  
  /* **** Initialize ***** */
  const size_t numWords      = numBytes / sizeof(size_t);
  const size_t numExtraBytes = numBytes - sizeof(size_t)*numWords;
  /***/ size_t hashValue = 0;
  const size_t* const pastLastFullWord  =  a + numWords;

  // ** modify hashValue according to each word in turn
  for( ; a < pastLastFullWord; ++a ){
    hashValue ^= *a;
    BIT_ROTATE_LEFT( hashValue );
  }


  /* ****** Set Up Mask To Take Care of Extra Bytes After Last Full Word ***** */
  const size_t& b = * (size_t*) pastLastFullWord;
  static const size_t mask[] = {
#ifdef __x86_64__
    0x0000000000000000,
    0x00000000000000FF,
    0x000000000000FFFF,
    0x0000000000FFFFFF,
    0x00000000FFFFFFFF,
    0x000000FFFFFFFFFF,
    0x0000FFFFFFFFFFFF,
    0x00FFFFFFFFFFFFFF,
#elif __i386__
    0x00000000,
    0x000000FF,
    0x0000FFFF,
    0x00FFFFFF,
#elif __sparc__  // big endian
    0x00000000,
    0xFF000000,
    0xFFFF0000,
    0xFFFFFF00,
#else
#error unknown architecture. only __x86_64__, __i386__ and __sparc__ currrently supported
#endif // big or little endian
};

  return(  hashValue ^ (b & mask[numExtraBytes])  );

}



inline size_t stringHashValue( const std::string& s ){
       return genericHashValue( s.size(), (size_t*) s.c_str() ); }


#endif // GENERICHASHFUNCTION_HH_
