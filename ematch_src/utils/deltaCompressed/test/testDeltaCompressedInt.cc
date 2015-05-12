/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.8
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test DeltaCompressedInt.
 */
#include <iostream>
#include "../DeltaCompressedInt.hh"


void testDeltaCompressedInt(){
  size_t lSize = 65000;
  std::vector<unsigned int> v( lSize );
  for( size_t i = 0; i < v.size(); ++i ){
    //    size_t j = i+98;
    size_t j = i+0;
    v[i] = j*j;
  }
  cbrc::DeltaCompressedInt<unsigned int> dc( v );
  //  v[13] = 13; // change v.
  assert( dc == v );
  std::cout << "array size is " << dc.arraySize() << std::endl;
  std::cout << "uncompressed list is: " << dc;
}

int main(){
  testDeltaCompressedInt();
  return 1;
}
