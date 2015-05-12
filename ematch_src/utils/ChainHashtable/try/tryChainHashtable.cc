/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.10.17
 *
 *  Last Modified: $Date: 2005/10/17 09:32:20 $
 *  
 *  Purpose: try code involving ChainHashtable.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ChainHashtable.hh"
#include "../hashableTypes/HashableSizeTKey.hh"

namespace cbrc{
  void tryChainHashtable(){
    ChainHashtable< HashableSizeTKey<std::string> > tab;
    tab.add( HashableSizeTKey<std::string>( 3, "strikes your out") );
    tab.add( HashableSizeTKey<std::string>( 7, "is your lucky number") );
    HashableSizeTKey<std::string>* foundElement = tab.find( HashableSizeTKey<std::string>(3) );
    std::cout << "tab( 3 ) " << foundElement->second << std::endl;
    foundElement = tab.find( HashableSizeTKey<std::string>(7) );
    std::cout << "tab( 7 ) " << foundElement->second << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  cbrc::tryChainHashtable();
  return 1;
}

