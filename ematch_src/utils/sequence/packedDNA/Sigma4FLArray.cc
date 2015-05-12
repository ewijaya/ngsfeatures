/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.21
 *  Last Modified: $Date: 2009/09/01 07:08:20 $
 *  Description: See header file.
 */
#include <cmath>
#include <cassert>
#include <algorithm>
#include <boost/foreach.hpp>
#include "utils/gdb/gdbUtils.hh"
#include "utils/stl/stlUtils.hh"
#include "Sigma4FLArray.hh"


namespace cbrc{


static const Sigma4FLArray::byte  mask[]  =  {0xc0, 0x30, 0x0c, 0x03};



Sigma4FLArray::Sigma4FLArray( const std::vector<byte>& elems )
  : _size( elems.size() )
{

  assert(  elems.size()  <=  max_size()  );

  /* ***** Allocate and zero memory ***** */
  _a  =  new byte[ byteSize() ];

  memset( _a, 0, byteSize() );


  /* ***** Range Check elems ***** */
  GDB_ASSERTF(  stlUtils::max( elems )  < sigma(),
		"found element: %zu, when maximum allowed value is %zu",
		(size_t) stlUtils::max( elems ), (size_t) sigma()  );


  std::vector<byte>::const_iterator  elemPtr  =  elems.begin();

  byte* aPtr  =  _a;


  /* ***** Fill in Value for All Full Bytes in a ***** */
  const byte* const  aFullByteEnd  =  a() + elems.size() / sigma();


  while( aPtr < aFullByteEnd ){
    *aPtr  |=  ((*elemPtr++) <<6);
    *aPtr  |=  ((*elemPtr++) <<4);
    *aPtr  |=  ((*elemPtr++) <<2);
    *aPtr  |=   *elemPtr++;
    ++aPtr;
  }


  /* ***** Finish Filling in Remaining 0-3 Elements ***** */
  if(  elemPtr == elems.end()  )  return;  // EXIT
  *aPtr  |=  ((*elemPtr++) <<6);

  if(  elemPtr == elems.end()  )  return;  // EXIT
  *aPtr  |=  ((*elemPtr++) <<4);

  if(  elemPtr == elems.end()  )  return;  // EXIT
  *aPtr  |=  ((*elemPtr++) <<2);

  GDB_ASSERTF(  elemPtr == elems.end(),
		"Programming error"  );
} // end Sigma4FLArray( const std::vector<byte> )



void Sigma4FLArray::operator=(  const Sigma4FLArray&  arrayToCopy  ){

  if(  arrayToCopy.byteSize()  !=  byteSize()  ){
    delete []_a;
    _a   =   new byte[ arrayToCopy.byteSize() ];
  }

  memcpy( _a, arrayToCopy.a(), arrayToCopy.byteSize() );
  _size  =  arrayToCopy.size();
}


Sigma4FLArray::byte Sigma4FLArray::operator()( const idxT& internalIdx ) const{


  // extract value.
  byte val  =  (  a(internalIdx.first)  &  mask[internalIdx.second]  );


  // shift values as necessary, fallthrough in this switch statement is intentional.
  switch( internalIdx.second ){
  case 0:  val  =  (val >>2);
  case 1:  val  =  (val >>2);
  case 2:  val  =  (val >>2);
  }

  std::cout  <<  std::dec;

  return val;
}


bool  Sigma4FLArray::operator<( const Sigma4FLArray& otherArray ) const{
  
  const size_t  minByteSize  =  std::min( byteSize(), otherArray.byteSize() );

  const int cmpResult  =  memcmp( a(), otherArray.a(), minByteSize );

  if( cmpResult < 0 )  return true;

  if( cmpResult > 0 )  return false;

  return(  size() < otherArray.size()  );
}


std::vector<Sigma4FLArray::byte>  Sigma4FLArray::toByteVector() const{

  std::vector<byte>  retVal( size() );

  for(  size_t i = 0;  i < size();  ++i  ){
    retVal[i] = (*this)(i);
  }

  return retVal;
}


void Sigma4FLArray::read( std::istream& iStream ){

  size_type newElemSize;
  iStream.read( (char*) &newElemSize, sizeof( newElemSize ) );

  const size_t  curByteSize  =  byteSize();
  const size_t  newByteSize  =  numBytesNeededToStore( newElemSize );

  if(  curByteSize  !=  newByteSize ){
    delete []_a;  // delete null is a noop, so okay even when _a is NULL
    _a  =  new byte[ newByteSize ];
  }

  iStream.read( (char*) _a, newByteSize );

  _size  =  newElemSize;
}



void Sigma4FLArray::write( std::ostream& oStream ) const{
  
  oStream.write(  (char*) &_size,  sizeof( _size )  );
  oStream.write(  (char*) a(),      byteSize()      );
}



Sigma4FLArray::idxT  Sigma4FLArray::toInternalIdx( const size_t& externalId ){

  idxT  retVal;

  retVal.first  =  externalId / sigma();
  retVal.second =  externalId % sigma();

  return retVal;
}



const char* const  Sigma4FLArray::c_str() const{

  char* const  retVal  =  new char[ size()+1 ];

  for(  size_t i = 0;  i < size();  ++i  ){
    retVal[i]  =  '0' + (*this)(i);
  }

  retVal[ size() ]  =  (char) NULL;  // c-string terminal byte.
  
  return retVal;
}


void Sigma4FLArray::hexDump(){

  for( size_t i = 0;  i < byteSize();  ++i  ){
    if( i )   std::cout  <<  ", ";
    std::cout  <<  std::hex  <<  (size_t) a(i);
  }
  std::cout  <<  std::dec  <<  std::endl;
}



} // end namespace cbrc

