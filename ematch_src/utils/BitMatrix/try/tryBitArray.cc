/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.23
 *  Last Modified: $Date: 2008/02/24 01:25:26 $
 *
 *  Purpose: try code involving BitArray
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../BitArray.hh"

namespace cbrc{

void tryBitArray(){
  size_t size = 99;
  BitArray ba( size );

  for( size_t i = 0; i < ba.size(); ++i ){
    ba.set( i, ((i % 2) == 0) );
  }
  
  std::cout << "bit contents\n" << ba;

  std::cout << "testing true iterator:\n";
  for( BitArray::trueIterator it = ba.trueBegin(); it != ba.trueEnd(); ++it ){
    std::cout << it << " ";
  }
  std::cout << "\n";
    
}

} // end namespace cbrc

int main(){
  cbrc::tryBitArray();
  return 1;
}

