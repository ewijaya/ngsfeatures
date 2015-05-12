/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.12
 *  Last Modified: $Date: 2009/09/12 06:55:01 $
 *
 *  Purpose: try code involving Sigma4bitPackedIdx
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../Sigma4bitPackedIdx.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  void trySigma4bitPackedIdx(){
    Sigma4bitPackedIdx packedIdx( 7 );

    std::cout  <<  " packedIdx: "  <<  packedIdx  << std::endl;

    ++packedIdx;
    std::cout  <<  " packedIdx: "  <<  packedIdx  << std::endl;

    ++packedIdx;
    std::cout  <<  " packedIdx: "  <<  packedIdx  << std::endl;
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::trySigma4bitPackedIdx();
  return 1;
}

