/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.1.29
 *
 *  Last Modified: 2004.1.29
 *  
 *  Purpose: test perlish::canStatFile.
 */
#include <iostream>
#include "../perlish.hh"

namespace cbrc{
  void testCanStatFile( std::string filename ){
    if(  perlish::canStatFile( filename )  ){
      std::cout << " can stat file " << filename << std::endl;
    }else{
      std::cout << " can't stat file " << filename << std::endl;
    }
  }
}; // end namescape cbrc

int main( int argc, char** argv ){
  if( argc < 2 ){
    std::cout << "Usage: " << argv[0] << " filename\n";
    exit( -1 );
  }
  std::string filename( argv[1] );
  cbrc::testCanStatFile( filename );
  return 1;
}

