/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.21
 *  Last Modified: $Date: 2009/09/13 23:51:36 $
 *
 *  Description: Array holding elements taking values in {0,3}
 *               Storage is bit packed to store 4 elements per byte.
 *               "Sigma" stands for alphabet size.
 *
 *  Purpose: Created to represent sequence tags in RECOUNT project.
 *
 *  Representation: Each byte is partitioned into 4 2-byte slots.
 *                  element i is stored in the (i % 4) slot of the i / 4th byte.
 *
 *  Caveats: Dynamic pointer allocation leads to relatively high memory overhead
 *           per object.
 */
#ifndef SIGMA4FLARRAY_HH_
#define SIGMA4FLARRAY_HH_
#include <cmath>
#include <limits>
#include <iostream>
#include <utility>
#include <vector>

namespace cbrc{

class Sigma4FLArray;
std::ostream& operator<<(  std::ostream& oStream,  const Sigma4FLArray& array  );


class Sigma4FLArray{
public:

  /* ********** TYPEDEFS ********** */
  typedef  short unsigned int  size_type;
  typedef       unsigned char  byte;

  // idxT.first = byte index,  idxT.second = position in byte
  typedef  std::pair<size_type, byte>  idxT;


  /* ********** CONSTRUCTORS ********** */
  Sigma4FLArray()
    : _size(0), _a(NULL)
  {}


  Sigma4FLArray( const Sigma4FLArray& arrayToCopy )
    : _size( arrayToCopy.size() ), _a(NULL)
  {
    if( _size ){
      _a  =  new byte[ arrayToCopy.byteSize() ];
      memcpy( _a, arrayToCopy.a(), arrayToCopy.byteSize() );
    }
  }


  Sigma4FLArray( const std::vector<byte>& elems );

  Sigma4FLArray( std::istream& iStream )
    : _size(0), _a(NULL)
  {
    read( iStream );
  }

  ~Sigma4FLArray(){
      delete []_a;
  }

  void operator=( const Sigma4FLArray& );


  /* ********** ACCESSORS ********** */

  const size_type&  size() const {  return _size;  }

  size_t  byteSize() const{
    return(  numBytesNeededToStore( size() )  );
  }

  byte  operator()( const size_t& i ) const{
    return this->operator()(  toInternalIdx( i )  );
  }

  byte  operator()( const idxT& internalIdx ) const;


  std::vector<byte>  toByteVector() const;
    

  /* ********** COMPARISONS ********** */

  bool operator==( const Sigma4FLArray& otherArray ) const{
    return(  ( size() == otherArray.size() )
	     && !memcmp( a(), otherArray.a(), byteSize() )  );
  }


  bool operator!=( const Sigma4FLArray& otherArray ) const{
    return(  ( size() != otherArray.size() )  
	     ||  memcmp( a(), otherArray.a(), byteSize() )  );
  }


  bool operator<( const Sigma4FLArray& otherArray ) const;

  bool operator<=( const Sigma4FLArray& otherArray ) const;

  /* ********** I/O METHODS ********** */

  void read ( std::istream& iStream );  // overwrites contents

  void write( std::ostream& oStream ) const;


  /* ********** INDEX CONVERSION FUNCTIONS ********** */

  static idxT  toInternalIdx( const size_t& externalId );


  /* ********** CLASS CONSTANTS ********** */

  static const size_t&  sigma(){
    const static size_t  _sigma( 4 );
    return _sigma;
  }

  static size_type  max_size(){
    return std::numeric_limits<size_type>::max();
  }


  /* ********** DEBUGGING FUNCTIONS ********** */

  // return c-string representing this object. Alocates a pointer. 
  // If memory leaks are a concern, the caller must delete the returned pointer.
  const char* const  c_str() const;

  // dump contents of array in hexadecimal
  void hexDump();

private:

  /* ********** PRIVATE ACCESSORS ********** */
  const byte* const  a() const{  return _a;  }

  const byte& a( const size_t& i ) const{  return a()[i];  }


  /* ********** OTHER PRIVATE METHODS ********** */

  static size_t  numBytesNeededToStore( const size_t& numElems ){
    return(   (size_t)  ceil(  (double) numElems / sigma()  )   );
  }


  // object data
  size_type  _size;
  byte*      _a;
};


inline std::ostream& operator<<(  std::ostream& oStream,  const Sigma4FLArray& array  ){

  for( size_t i = 0;  i < array.size();  ++i ){
    if( i )   oStream  <<  " ";
    oStream  <<  (size_t)  array(i);
  }

  return oStream;
}

} // end namespace cbrc
#endif // SIGMA4FLARRAY_HH_
