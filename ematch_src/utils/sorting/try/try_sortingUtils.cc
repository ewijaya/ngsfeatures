/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.13
 *  Last Modified: $Date: 2009/09/13 04:47:41 $
 *
 *  Purpose: try code involving _sortingUtils
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../sortingUtils.hh"



namespace cbrc{

  void try_sortingUtils(){
    
    int a[]  =  {3, 1, 4, 4, 5};


    for(  size_t i = 0;  i < 4;  ++i  ){
      std::cout  <<  a[i]  <<  " <=> "  <<  a[i+1]  <<  " = "
		 <<  sortingUtils::cmp( a[i], a[i+1] )  <<  std::endl;
    }


    double x[]  =  {3.1416, 3.1416, 2.78, 17.9};

    for(  size_t i = 0;  i < 3;  ++i  ){
      std::cout  <<  x[i]  <<  " <=> "  <<  x[i+1]  <<  " = "
		 <<  sortingUtils::cmp( x[i], x[i+1] )  <<  std::endl;
    }

  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "put usage message here!" );
  
  argvP.dieIfUnusedArgs();
  cbrc::try_sortingUtils();
  return 1;
}

