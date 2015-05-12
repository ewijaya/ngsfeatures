/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.3
 *  Last Modified: $Date: 2008/11/03 09:08:31 $
 *
 *  Purpose: try code involving SprotReader
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../SprotReader.hh"


struct argT{
  std::istream* sprotIstreamPtr;
} args;


namespace cbrc{
  void trySprotReader(){
    SprotReader sprotReader;

    while( sprotReader.readRecord( *args.sprotIstreamPtr ) ){
      std::cout << "sequence is: " << sprotReader.sequence() << std::endl;
    }
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "[sprotFile]" );
  
  args.sprotIstreamPtr = argP.getIstreamPtr( 1 );

  argP.dieIfUnusedArgs();
  cbrc::trySprotReader();
  return 1;
}

