/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.21
 *  Last Modified: $Date: 2009/06/23 23:24:56 $
 *
 *  Purpose: try code involving Sigma4FLArray
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../Sigma4FLArray.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
std::string arg_filename;


namespace cbrc{

void trySigma4FLArray_readWrite(){

  std::vector<unsigned char> vec;
    
  vec.push_back( 2 );
  vec.push_back( 0 );
  vec.push_back( 3 );
  vec.push_back( 2 );
  vec.push_back( 1 );
  vec.push_back( 2 );

  std::cout << "Array vec:" << std::endl;
  for( size_t i = 0; i < vec.size(); ++i ){
    if( i ) std::cout << " ";
    std::cout  <<  (size_t) vec[i];
  }
  std::cout << std::endl;
    

  Sigma4FLArray packedArray( vec );

  std::cout << "Array packedArray:" << std::endl;
  for( size_t i = 0; i < packedArray.size(); ++i ){
    if( i ) std::cout << " ";
    std::cout  <<  (size_t) packedArray(i);
  }
  std::cout << std::endl;


  /* ***** Write Contents out to File ***** */
  std::ofstream ofStream;

  perlish::openCautiously( ofStream, arg_filename );

  packedArray.write( ofStream );

  ofStream.close();



  /* ***** Read Contents Back in from File ***** */
  std::ifstream ifStream;

  perlish::openOrDie( ifStream, arg_filename );

  Sigma4FLArray newPackedArray( ifStream );


  std::cout << "Array newPackedArray:" << std::endl;
  for( size_t i = 0; i < newPackedArray.size(); ++i ){
    if( i ) std::cout << " ";
    std::cout << (size_t) newPackedArray(i);
  }
  std::cout << std::endl;
    
    
} // end trySigma4FLArray_readWrite

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "filename" );

  argvP.set( arg_filename, 1 );
  
  argvP.dieIfUnusedArgs();

  cbrc::trySigma4FLArray_readWrite();
  return 1;
}

