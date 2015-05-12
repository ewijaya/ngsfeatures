/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.11.1
 *  Last Modified: $Date: 2009/11/01 07:05:19 $
 *
 *  Purpose: try code involving ResidueColorGenebee
 */
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueColorGenebee.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */

namespace cbrc{

  void tryResidueColorGenebee(){
    const std::string  residues( "ABCDEFGHIJKLMNOPQRSTUVWYZ" );

    BOOST_FOREACH(  const char& res,  residues  ){
      std::cout  <<  res  <<  ":"  <<  residueColorGenebee.getColor( res )  <<  std::endl;
    }
  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "no args." );
  
  argvP.dieIfUnusedArgs();
  cbrc::tryResidueColorGenebee();
  return 1;
}

