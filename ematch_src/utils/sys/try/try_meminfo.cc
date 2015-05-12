/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.15
 *  Last Modified: $Date: 2008/04/15 14:13:36 $
 *
 *  Purpose: try code involving _meminfo
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../meminfo.hh"

namespace cbrc{
  void try_meminfo(){
    std::cout << "memory size is: "
	      << meminfo::total() << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::try_meminfo();
  return 1;
}

