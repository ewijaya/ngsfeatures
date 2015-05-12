/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004,2007,2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.5.20
 *  Last Modified: $Date: 2008/11/23 02:12:05 $
 *
 *  Purpose: try code involving StringKeyHashtable
 */
#include <iostream>
#include "utils/ChainHashtable/StringKeyHashtable.hh"

namespace cbrc{
  void tryStringKeyHashtable(){
    StringKeyHashtable<int> table;
    table["danny"] = 4;
    table["danny"] = 0;
    table["fred"] = 37;
    std::cout << " value for danny is: " << table( "danny" ) << std::endl;
    std::cout << " value for fred is: " << table["fred"] << " or " << table("fred") << std::endl;


    // print notFoundValue
    table.setNotFoundValue( -1 );
    std::cout << " value for larry is: " << table( "larry" ) << std::endl;

    StringKeyHashtable<int> copy = table;
    std::cout << " value for danny is: " << copy( "danny" ) << std::endl;
    std::cout << " value for fred is: " << copy["fred"] << " or " << copy("fred") << std::endl;

  }
}; // end namescape cbrc

int main(){
  cbrc::tryStringKeyHashtable();
  return 1;
}

