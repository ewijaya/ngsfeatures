/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.3
 *  Last Modified: $Date: 2007/05/12 07:05:19 $
 *
 *  Purpose: try or run code involving perlish::setMandatory
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"

namespace cbrc{
  void tryPerlishSetMandatory(){
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "-uli unsignedLongInt" );
  unsigned long int uli;
  args.setMandatory( uli, "-uli" );
  printf( "uli: %lu\n", uli );
  args.dieIfUnusedArgs();
  cbrc::tryPerlishSetMandatory();
  return 1;
}

