/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.27
 *  Last Modified: $Date: 2009/06/27 08:13:16 $
 *
 *  Purpose: try code involving KnapsackObjectVector
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../KnapsackObjectVector.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
static  std::ifstream  arg_ifstream;


namespace cbrc{

  void tryKnapsackObjectVector(){

    KnapsackObjectVector objectVector( arg_ifstream );

    std::cout  <<  "vector is: "  <<  objectVector  <<  std::endl;
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "objectVector_textFile" );

  argvP.setOrDie( arg_ifstream, 1 );

  argvP.dieIfUnusedArgs();
  cbrc::tryKnapsackObjectVector();
  return 1;
}

