/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.10.8
 *  Last Modified: $Date: 2009/10/08 01:43:32 $
 *
 *  Purpose: try code involving _complementDNA
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ComplementDNA.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  void try_complementDNA(){

    const  char  tryChars[] = {'a','c','g','t','A','C','G','T','b'};

    for( size_t i = 0; i < sizeof(tryChars)/sizeof(tryChars[0]); ++i ){
      const  char  tryChar  =  tryChars[i];
      std::cout  <<  "complement of '"  <<  tryChar
		 <<  "'  -->  '"  <<  complementDNA(tryChar)  << "'\n";
    }
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::try_complementDNA();
  return 1;
}

