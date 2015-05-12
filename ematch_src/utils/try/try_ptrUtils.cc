/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.13
 *  Last Modified: $Date: 2009/09/13 23:50:46 $
 *
 *  Purpose: try code involving _ptrUtils
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ptrUtils.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
// static  arg_


namespace cbrc{

  void try_ptrUtils(){
    int a[]  =  {1, 2, 3, 4, 5};

    int* mid  =  ptrUtils::average(  &a[0],  &a[4]  );

    std::cout  <<  "middle element is: "  <<  *mid  <<  std::endl;
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::try_ptrUtils();
  return 1;
}

