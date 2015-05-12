/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.19
 *  Last Modified: $Date: 2008/03/29 22:55:04 $
 *
 *  Purpose: try code involving FLEArray::print()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLEArray_print( std::vector<int> ints ){

    FLEArray<int> intArray( ints );

    intArray.print( std::cout );

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "int1 [int2...]" );

  if( !args.numArgs() ){
    args.die();
  }

  args.printDoc();

  std::vector<int> ints;
  for( std::string curString = args.shift();
       curString.size();
       curString = args.shift() ){
    ints.push_back(   cbrc::perlish::toInt( curString )   );
  }

  args.dieIfUnusedArgs();

  cbrc::tryFLEArray_print( ints );
  return 1;
}

