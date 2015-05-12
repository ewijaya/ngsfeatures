/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.24
 *  Last Modified: $Date: 2008/08/24 14:01:34 $
 *
 *  Description: Specific type wrapper for map. the key is an unsigned
 *                                              the value is a double.
 *
 *  Purpose: 1. Make code cleaner by packaging some of the make_pair operations
 *           2. Decouple code from details of stl map.
 *
 *  Usage: read with              table( unsigned key ).
 *         write double x with    table( unsigned key, double val ).
 *         check presence with    table.has( unsigned key ).
 */
#ifndef UNSIGNEDDOUBLEMAP_HH_
#define UNSIGNEDDOUBLEMAP_HH_
#include <iostream>
#include <map>
#include "utils/gdb/gdbUtils.hh"

namespace cbrc{

class UnsignedDoubleMap{
public:
  /* ********** TYPEDEFS ********** */
  typedef  std::map<unsigned, double>  mapT;

  /* ********** CONSTRUCTORS ********** */
  UnsignedDoubleMap(){}

  /* ********** MAIN METHODS ********** */
  bool has( const unsigned& key ) const{
    return(  map().find( key ) != map().end()  );
  }

  double operator()( const unsigned& key ) const{
    mapT::const_iterator matchingEntry = map().find( key );
    if( matchingEntry != map().end() ){
      return matchingEntry->second;
    }
    GDB_DIEF( "could not find entry for key %u", key );
  }

  void operator()( const unsigned& key, const double& val ){
    mapT::iterator matchingEntry = _map.find( key );
    if( matchingEntry == map().end() ){
      _map.insert( std::make_pair(key, val) );
    }
    else{
      matchingEntry->second = val;
    }
  }

  /* ********** ACCESSORS ********** */
  const mapT& map() const{  return _map;  }

//   friend std::ostream& operator<<( std::ostream& oStream,
// 				   const UnsignedDoubleMap& unsignedDoubleMap );
private:
  // object data
  mapT  _map;
};


inline std::ostream& operator<<( std::ostream& oStream,
			  const UnsignedDoubleMap& unsignedDoubleMap ){
  for( UnsignedDoubleMap::mapT::const_iterator itr =  unsignedDoubleMap.map().begin();
       itr != unsignedDoubleMap.map().end();
       ++itr ){
    oStream << itr->first << ", " << itr->second << std::endl;
  }
  return oStream;
}


} // end namespace cbrc
#endif // UNSIGNEDDOUBLEMAP_HH_
