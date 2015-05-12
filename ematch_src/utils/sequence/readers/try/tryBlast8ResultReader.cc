/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2004.5.18
 *  Last Modified: $Date: 2009/04/25 06:41:32 $
 *
 *  Purpose: try code involving Blast8ResultReader
 */
#include <iostream>
#include "../Blast8ResultReader.hh"


namespace cbrc{

  void tryBlast8ResultReader(){
    Blast8ResultReader blastResults( std::cin, false );
    std::cout << blastResults;
  }

}; // end namescape cbrc


int main(){
  cbrc::tryBlast8ResultReader();
  return 1;
}

