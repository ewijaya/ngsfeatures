/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2008.7.2
 *  Last Modified: $Date: 2009/05/03 12:22:07 $
 *
 *  Purpose: try code involving GDB_DIE macro in gdbUtils.hh
 */
#include <iostream>
#include "../gdbUtils.hh"



int main( int argc, const char* argv[] ){

  const std::string fallingThing( "the sky" );

  GDB_DIE( fallingThing );
}

