/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.6
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test UInt31.
 */
#include <iostream>
#include <stdlib.h>
#include "../UInt31.hh"

void cbrc::testUInt31(){
  unsigned char val0 = 78;
  unsigned int val1 = 546;
  unsigned int lsize = 6;
  cbrc::UInt31 edge0( val0, val1 );
  
  std::cout << "edge0: " << edge0 << std::endl;

  val0--; val1++;
  cbrc::UInt31 edge1( val0, val1 );


  val0+=45; val1-=32;
  cbrc::UInt31 edge2( val0, val1 );

  val0+=19; val1-=32;
  cbrc::UInt31 edge3( val0, val1 );

  val0-=27; val1+=33;
  cbrc::UInt31 edge4( val0, val1 );

  --val1;
  cbrc::UInt31 edge5( val0, val1 );

  cbrc::UInt31* l = new cbrc::UInt31[lsize];
  l[0] = edge0;
  l[1] = edge1;
  l[2] = edge2;
  l[3] = edge3;
  l[4] = edge4;
  l[5] = edge5;
  
  std::cout << "list:\n";
  for( size_t i = 0; i < lsize; ++i ){
    std::cout << l[i] << "\n";
  }
  std::cout << std::endl;

  qsort( l, lsize, sizeof( cbrc::UInt31 ), cbrc::UInt31::cmpAscendVal0 );

  std::cout << "list sorted:\n";
  for( size_t i = 0; i < lsize; ++i ){
    std::cout << l[i] << "\n";
  }
  std::cout << std::endl;


}

int main(){
  cbrc::testUInt31();
  return 1;
}

