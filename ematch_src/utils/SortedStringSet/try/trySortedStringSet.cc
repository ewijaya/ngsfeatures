/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.4.21
 *  Last Modified: $Date: 2009/04/21 14:31:17 $
 *
 *  Purpose: try code involving SortedStringSet
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/toVector.hh"
#include "../SortedStringSet.hh"


struct argsT{

} args;


namespace cbrc{
  void trySortedStringSet(){
    SortedStringSet strSet(  toVector( "cat", "giraffe", "wallaby", "kangaroo" )  );


    std::cout << "set is:\n==========\n" << strSet << "==========\n\n";

    std::cout << "set has " << ( strSet.has("dog") ? "" : "no " ) << "dog\n";

    std::cout << "set has " << ( strSet.has("cat") ? "" : "no " ) << "cat\n";


    std::cout << "serial number of giraffe is: " << strSet( "giraffe" ) << std::endl;

    std::cout << "string with serial number 3 is: " << strSet( 3 ) << std::endl;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "put usage message here!" );
  
  argP.dieIfUnusedArgs();
  cbrc::trySortedStringSet();
  return 1;
}

