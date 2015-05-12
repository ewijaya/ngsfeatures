/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.8.11
 *
 *  Last Modified: 2003.8.11
 *  
 *  Purpose: test UintMath.
 */
#include <iostream>
#include "../uintMath.hh"

int main( int argc, char** argv ){
  std::string usage( " x y\n          computes x^y\n" );
  if( argc != 3 ){
    std::cout << "Usage: " << argv[0] << usage;
    exit( 1 );
  }
  unsigned int x = atoi( argv[1] );
  unsigned int y = atoi( argv[2] );
  try{
    std::cout << x << "^" << y << " = " << cbrc::uintMath::pow( x, y ) << std::endl;
  }catch( const std::exception& error ){
    std::cerr << "error says: " << error.what() << std::endl;
  }
  return 1;
}

