/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.11
 *  Last Modified: $Date: 2006/09/12 01:03:28 $
 *
 *  Purpose: try or run code involving TransfacTextLine
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "../TransfacTextLine.hh"

namespace cbrc{
  void tryTransfacTextLine(){
    for( std::string line = perlish::slurpLine(); 
	 line.size();
	 line = perlish::slurpLine() ){
      TransfacTextLine transfacLine( line );
      std::cout << transfacLine;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "stream transfac format file from standard in" );
  args.dieIfUnusedArgs();
  cbrc::tryTransfacTextLine();
  return 1;
}

