/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.5.20
 *
 *  Last Modified: 2004.5.20
 *  
 *  Purpose: test String2KeyHashtable.
 */
#include <iostream>
#include "utils/ChainHashtable/String2KeyHashtable.hh"

namespace cbrc{
  void testString2KeyHashtable(){
    String2KeyHashtable<int> table;
    table.ref("danny","george") = 4;
    table.ref("danny","george") = 0;
    table.ref("fred", "larry") = 37;

    std::cout << "table.has( fred, larry ) " << table.has( "fred", "larry" ) << std::endl;
    std::cout << "table.has( larry, fred ) " << table.has( "larry", "fred" ) << std::endl;

    std::cout << " value for danny, george is: " << table( "danny", "george" ) << std::endl;
    std::cout << " value for fred, larry is: " << table.ref("fred", "larry") << " or " << table("fred", "larry" ) << std::endl;

    String2KeyHashtable<int> copy = table;
    std::cout << " value for danny, george is: " << copy( "danny", "george" ) << std::endl;
    std::cout << " value for fred, larry is: " << copy.ref( "fred", "larry" ) << " or " << copy("fred", "larry") << std::endl;

  }
}; // end namescape cbrc

int main(){
  cbrc::testString2KeyHashtable();
  return 1;
}

