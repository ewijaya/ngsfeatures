/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.1.6
 *  Last Modified: $Date: 2008/06/08 05:52:37 $
 *
 *  Purpose: try or run code involving trybitLeftRotate.
 */
#include <iostream>
#include <iomanip>
#include "../size_tToBinaryRepresentationString.hh"
#include "../bitLeftRotate.hh"

namespace cbrc{
  void trybitLeftRotate(){
    const size_t wordOrig  =  
      ((sizeof(size_t) == 4) ? 0x01234567
      /* else */             : 0x0123456789ABCDEF);

    size_t word = wordOrig;

    std::cout << "   " << size_tToBinaryRepresentationString(word) << std::endl;

    const size_t numIterations = 8 * sizeof(size_t);

    for( size_t i = 0; i < numIterations; ++i ){
      BIT_ROTATE_LEFT( word );
      std::cout << std::setw(2) << i << " " 
		<< size_tToBinaryRepresentationString(word)
		<< std::endl;
    }

    if( word != wordOrig ){
      std::cout << "Error, did not cycle back after "
		<< numIterations << " iterations\n";
      exit( 1 );
    }
  }
}; // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::trybitLeftRotate();
  return 1;
}

