/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.4.21
 *  Last Modified: $Date: 2009/05/12 02:33:04 $
 *
 *  Purpose: try code involving SortedStringSet_readFromTextStream
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../SortedStringSet.hh"


struct argsT{
  std::istream* istreamPtr;
} args;


namespace cbrc{
  void trySortedStringSet_readFromTextStream(){

    SortedStringSet strSet( *args.istreamPtr );

    std::cout << strSet;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "[SortedStringSet_textInputFile]" );

  args.istreamPtr = argP.getIstreamPtr( 1 );
  
  argP.dieIfUnusedArgs();
  cbrc::trySortedStringSet_readFromTextStream();
  return 1;
}

