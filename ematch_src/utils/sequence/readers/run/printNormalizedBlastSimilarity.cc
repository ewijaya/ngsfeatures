/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.5.18
 *
 *  Last Modified: 2004.5.18
 *  
 *  Description: Prints list of: queryName hitName -lg(eValue)
 *               for blast8 results
 *
 *  Purpose: Made to use Blast Scores in conjunction with WoLF.
 */
#include <iostream>
#include "../Blast8ResultReader.hh"

namespace cbrc{
  void printNormalizedBlastSimilarity(){
    Blast8ResultReader blastResults( std::cin, false );
    blastResults.printNormalizedSimilarity( std::cout );
  }
}; // end namescape cbrc

int main(){
  cbrc::printNormalizedBlastSimilarity();
  return 1;
}

