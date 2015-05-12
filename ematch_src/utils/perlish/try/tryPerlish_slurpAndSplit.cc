/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.9.11
 *  Last Modified: $Date: 2007/04/08 11:54:23 $
 *
 *  Purpose: try or run code involving Perlish_slurpAndSplit
 */
#include <iostream>
#include "../perlish.hh"

namespace cbrc{
  void tryPerlish_slurpAndSplit(){
    std::vector< std::vector<std::string> > fieldMatrix = perlish::slurpAndSplit( std::cin, " +" );

    for( size_t i = 0; i < fieldMatrix.size(); ++i ){
      for( size_t j = 0; j < fieldMatrix[i].size(); ++j ){
	std::cout << fieldMatrix[i][j] << "|";
      }
      std::cout << std::endl;
    }
  }
}; // end namescape cbrc

int main(){
  cbrc::tryPerlish_slurpAndSplit();
  return 1;
}

