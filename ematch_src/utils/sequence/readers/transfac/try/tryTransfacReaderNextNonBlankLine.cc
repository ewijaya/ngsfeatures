/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.12
 *  Last Modified: $Date: 2006/09/12 05:00:29 $
 *
 *  Purpose: try or run code involving TransfacReader.NextNonBlankLine()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../TransfacReader.hh"

namespace cbrc{
  void tryTransfacReaderNextNonBlankLine(){
    TransfacReader tfr( std::cin );
    for( TransfacTextLine ttl( tfr.nextNonBlankLine() );
	 !(ttl.empty());
	 ttl = tfr.nextNonBlankLine() ){
      std::cout << ttl;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "stream tranfac text format file from standard in." );
  
  args.dieIfUnusedArgs();
  cbrc::tryTransfacReaderNextNonBlankLine();
  return 1;
}

