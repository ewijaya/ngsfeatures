/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.8
 *  Last Modified: $Date: 2008/06/08 04:19:47 $
 *
 *  Purpose: try code involving _size_tToBinaryRepresentationString
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../size_tToBinaryRepresentationString.hh"

namespace cbrc{
  void try_size_tToBinaryRepresentationString( const size_t& integer ){
    std::cout << size_tToBinaryRepresentationString( integer )
	      << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "unsigned" );

  size_t integer;
  args.setOrDie( integer, 1 );

  args.dieIfUnusedArgs();
  cbrc::try_size_tToBinaryRepresentationString( integer );
  return 1;
}

