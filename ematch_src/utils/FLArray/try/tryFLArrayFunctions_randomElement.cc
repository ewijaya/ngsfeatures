/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.12.11
 *  Last Modified: $Date: 2007/12/11 10:03:44 $
 *
 *  Purpose: try our code involving FLArrayFunctions_randomElement
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLArrayFunctions.hh"

namespace cbrc{
  void tryFLArrayFunctions_randomElement(){
    std::vector<std::string> animalV;
    animalV.push_back( "kangaroo" );
    animalV.push_back( "bear" );
    animalV.push_back( "lion" );
    animalV.push_back( "iguana" );

    FLArray<std::string> animals( animalV );

    for( size_t i = 0; i < 10; ++i ){
      std::cout << FLArrayFunctions::randomElement( animals ) << std::endl;
    }
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLArrayFunctions_randomElement();
  return 1;
}

