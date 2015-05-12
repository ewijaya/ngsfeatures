/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.12
 *  Last Modified: $Date: 2009/09/13 05:21:10 $
 *
 *  Purpose: try code involving _sigma4bitPackingUtils
 */
#include <iostream>
#include "boost/foreach.hpp"
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/pushbackLiteral.hh"
#include "../sigma4bitPackingUtils.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  const byte a[]  =  {3, 3, 2, 0, 1, 0};
  /***/ std::vector<byte> v( a, a + sizeof(a) );

  void dumpVec(  const std::vector<byte>&  vec  ){
    std::cout  <<  "vec: ";
    BOOST_FOREACH( const byte b, vec ){
      std::cout  <<  (unsigned) b  <<  ",";
    }
    std::cout  <<  std::endl;
  }


  void try_sigma4bitPackingUtils(){

    const size_t  vInitialSize  =  v.size(); 

    std::cout  <<  "initial state of vector\n";
    dumpVec( v );

    byte* mem  =  new byte[ sigma4bitPackingUtils::numBytesNeededToStore( v.size() ) ];

    sigma4bitPackingUtils::pack( mem, v );

    std::cout  <<  "vector cleared\n";
    v.clear();
    dumpVec( v );

    sigma4bitPackingUtils::unpack( v, mem, vInitialSize );

    std::cout  <<  "vector after packing and unpacking\n";
    dumpVec( v );


    std::cout  <<  "testing toUnpacked...\n";
    std::vector<byte> w  =  sigma4bitPackingUtils::toUnpacked( mem, vInitialSize );
    dumpVec( w );

  }


} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::try_sigma4bitPackingUtils();
  return 1;
}

