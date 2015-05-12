/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.12.31
 *  Last Modified: $Date: 2009/04/18 23:26:07 $
 *
 *  Purpose: try code involving FLESorted
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLESorted.hh"
#define ELEMENT_TYPE size_t

namespace cbrc{
  void tryFLESorted(){
    std::vector<ELEMENT_TYPE> elements;
    elements.push_back( 314 );
    elements.push_back( 278 );
    elements.push_back( 400 );

    std::cout << "constructing intToSerialNumber with elements: 314, 278, 400\n";
    FLESorted<ELEMENT_TYPE> intToSerialNumber( elements );

    std::cout << "second element: ";
    std::cout << intToSerialNumber(1) << std::endl;

    std::cout << "rank of 313: ";
    std::cout << intToSerialNumber.elementRank( 313 ) << std::endl;

    std::cout << "rank after ties of 313: ";
    std::cout << intToSerialNumber.elementRankAfterTies( 313 ) << std::endl;


    std::cout << "rank of 314: ";
    std::cout << intToSerialNumber.elementRank( 314 ) << std::endl;

    std::cout << "rank after ties of 314: ";
    std::cout << intToSerialNumber.elementRankAfterTies( 314 ) << std::endl;


    std::cout << "dumping contents with iterator...\n";
    for( FLESorted<ELEMENT_TYPE>::const_iterator itr = intToSerialNumber.begin();
	 itr != intToSerialNumber.end();
	 ++itr ){
      std::cout << ( (itr == intToSerialNumber.begin()) ? "" : ", "  );
      std::cout << *itr;
    }
    std::cout << std::endl;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  args.dieIfUnusedArgs();
  cbrc::tryFLESorted();
  return 1;
}

