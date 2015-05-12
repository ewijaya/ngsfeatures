/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.11.2
 *  Last Modified: $Date: 2009/11/02 06:12:14 $
 *
 *  Purpose: try code involving StringSpecEnum
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../StringSpecEnum.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  void tryStringSpecEnum(){

  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::tryStringSpecEnum();
  return 1;
}

