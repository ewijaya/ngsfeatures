/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.1.3
 *  Last Modified: $Date: 2006/01/03 13:27:58 $
 *
 *  Purpose: try or run code involving StlCount.
 */
#include <iostream>
#include <algorithm>
#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"

namespace cbrc{
  void tryStlCountIf(){
    std::vector<std::string> lines = perlish::slurpLines();
    size_t numericalCount = std::count_if( lines.begin(), lines.end(), 
					   perlish::looksLikeReal );
    std::cout << "number lines which seem to consist entirely of numerical looking strings is: "
	      << numericalCount << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "pipe lines in from standard in" );
  args.dieIfUnusedArgs();
  cbrc::tryStlCountIf();
  return 1;
}

