/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton 2006, All rights reserved.
 *  Creation Date: 2006.9.9
 *  Last Modified: $Date: 2006/09/10 22:36:35 $
 *
 *  Purpose: try or run code involving TransfacBindingFactor
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../TransfacBindingFactor.hh"

namespace cbrc{
  void tryTransfacBindingFactor(){
    TransfacBindingFactor 
      tbf( "T00505; aMEF-2; Species: mouse, Mus musculus." );
    std::cout << "binding factor: " << tbf << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  args.dieIfUnusedArgs();
  cbrc::tryTransfacBindingFactor();
  return 1;
}

