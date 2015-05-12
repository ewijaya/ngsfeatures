/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.6
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test UInt332.
 */
#include <iostream>
#include <stdlib.h>
#include "../UInt332.hh"


void cbrc::testUInt332(){
  //  std::cout << "size of long long is: " << sizeof(unsigned long long int) << std::endl;
  unsigned short int val0 = 78;
  unsigned int val1 = 546;
  unsigned int val2 = 37888;
  unsigned int lsize = 6;
  cbrc::UInt332 edge0( val0, val1, val2 );

  std::cout << "edge0: " << edge0 << std::endl;

  val0--; val1++; val2--;
  cbrc::UInt332 edge1( val0, val1, val2 );


  val0+=45; val1-=32; val2+=34;
  cbrc::UInt332 edge2( val0, val1, val2 );

  val0+=19; val1-=32; val2+=34;
  cbrc::UInt332 edge3( val0, val1, val2 );

  val0-=27; val1+=33; val2+=74;
  cbrc::UInt332 edge4( val0, val1, val2 );

  val1--;
  cbrc::UInt332 edge5( val0, val1, val2 );

  cbrc::UInt332* l = new cbrc::UInt332[lsize];
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

  qsort( l, lsize, sizeof( cbrc::UInt332 ), cbrc::UInt332::cmpAscendVal0 );

  std::cout << "list sorted:\n";
  for( size_t i = 0; i < lsize; ++i ){
    std::cout << l[i] << "\n";
  }
  std::cout << std::endl;


}

int main(){
  cbrc::testUInt332();
  return 1;
}
