/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.3.30
 *  Last Modified: $Date: 2008/03/30 23:45:27 $
 *
 *  Purpose: try code involving FLEArray::operator<<
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLEArray.hh"

namespace cbrc{
  void tryFLEArray_printOperator(){
    
    unsigned char t[] = { 'a', 'b', 'c' };

    FLEArray<unsigned char> asChars( sizeof(t)/sizeof(t[0]), t );
    FLENumArray<unsigned char> asNums( asChars );

    std::cout << "array as chars: " << asChars << std::endl;

    std::cout << "array as nums: " << asNums << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryFLEArray_printOperator();
  return 1;
}

