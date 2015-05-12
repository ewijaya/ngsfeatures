/*
 *  Author: Please set CBRC_AUTHOR environment variable
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.6.25
 *
 *  Last Modified: 2004.6.25
 *  
 *  Purpose: test StringStringDoubleMap.
 */
#include <iostream>
#include "../StringStringDoubleMap.hh"

namespace cbrc{
  void testStringStringDoubleMap(){
    StringStringDoubleMap ssdm;
    ssdm( "fred", "larry", 3.4 );
    ssdm( "ted", "paul", 100 );
    std::cout << " ssdm.has( fred, larry ) " <<  ssdm.has( "fred", "larry" ) << std::endl;
    std::cout << " ssdm.has( larry, fred ) " <<  ssdm.has( "larry", "fred" ) << std::endl;
    std::cout << "ssdm( fred, larry ) = " << ssdm( "fred", "larry" ) << std::endl;

    std::cout << "dumping table: " << ssdm;
  }
}; // end namescape cbrc

int main(){
  cbrc::testStringStringDoubleMap();
  return 1;
}

