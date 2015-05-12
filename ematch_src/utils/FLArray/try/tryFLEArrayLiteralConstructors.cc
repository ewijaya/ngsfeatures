/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.5.23
 *  Last Modified: $Date: 2008/08/23 22:59:32 $
 *
 *  Purpose: try code involving FLEArrayLiteralConstructors
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/FLArray/FLEArray.hh"

struct argsT{
  std::string booleanArrayLiteral;
  std::string numericArrayLiteral;
} args;


namespace cbrc{
  void tryFLEArrayLiteralConstructors(){

    FLEArray<unsigned char> a( "boolean", args.booleanArrayLiteral );

    std::cout << "Array a:" << std::endl;
    for( size_t i = 0; i < a.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout << (unsigned int) a[i];
    }
    std::cout << std::endl;


    // try semantic type "numeric"
    FLENumArray<double> f( "numeric", args.numericArrayLiteral );
    std::cout << "f is:\n"
	      << f << std::endl;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "binaryArrayLiteral numericArrayLiteral" );

  args.booleanArrayLiteral = argP.shiftOrDie();
  args.numericArrayLiteral = argP.shiftOrDie();
  argP.dieIfUnusedArgs();
  cbrc::tryFLEArrayLiteralConstructors();
  return 1;
}

