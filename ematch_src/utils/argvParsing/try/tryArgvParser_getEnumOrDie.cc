/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.3
 *  Last Modified: $Date: 2009/06/03 06:53:56 $
 *
 *  Purpose: try code involving ArgvParser_getEnumOrDie
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#define  ENUM_SPEC  left|right


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  void tryArgvParser_getEnumOrDie(){

  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "("Q(ENUM_SPEC)")" );

  std::string enumArg(  argvP.getEnumOrDie( 1, Q(ENUM_SPEC) )  );

  std::cout  <<  "enum arg is: "  <<  enumArg  <<  std::endl;
  
  argvP.dieIfUnusedArgs();
  cbrc::tryArgvParser_getEnumOrDie();
  return 1;
}

