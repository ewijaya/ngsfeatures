/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.21
 *  Last Modified: $Date: 2009/06/23 23:38:59 $
 *
 *  Purpose: try code involving Sigma4FLArray
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../Sigma4FLArray.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  void trySigma4FLArray(){

    std::vector<unsigned char> vec;
    
    vec.push_back( 2 );
    vec.push_back( 0 );
    vec.push_back( 3 );
    vec.push_back( 2 );
    vec.push_back( 1 );

    std::cout << "Array vec:" << std::endl;
    for( size_t i = 0; i < vec.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout  <<  (size_t) vec[i];
    }
    std::cout << std::endl;
    

    Sigma4FLArray packedArray( vec );

    packedArray.hexDump();

    std::cout << "Array packedArray:" << std::endl;
    for( size_t i = 0; i < packedArray.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout  <<  (size_t) packedArray(i);
    }
    std::cout << std::endl;

    Sigma4FLArray newPackedArray( packedArray );

    std::cout << "Array newPackedArray:" << std::endl;
    for( size_t i = 0; i < newPackedArray.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout << (size_t) newPackedArray(i);
    }
    std::cout << std::endl;


    std::cout  <<  "newPackedArray as string "
	       <<  newPackedArray.c_str()  <<  std::endl;
    
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::trySigma4FLArray();
  return 1;
}

