/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/05/06 05:14:14 $
 *
 *  Purpose: try code involving FastaInputOptions
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastaInputOptions.hh"




namespace cbrc{

  void tryFastaInputOptions(){
    std::cout << " default options are:\n"
	      << defaultFastaInputOptions;
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "no args" );
  
  argP.dieIfUnusedArgs();
  cbrc::tryFastaInputOptions();
  return 1;
}

