/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.11
 *  Last Modified: $Date: 2009/09/20 08:19:16 $
 *  Description: See header file.
 */
#include "utils/stl/binaryIO.hh"
#include "RecountNeighborProbGraphFormat.hh"
#include "RecountNeighborProbGraphOnDisk.hh"


namespace cbrc{


/* *************** CONSTRUCTORS *************** */

void RecountNeighborProbGraphOnDisk::init(){

  binaryIO::traceOrDie(  RecountNeighborProbGraphFormat::signature(), graphFile  );

  graphFile.read( (char*) &_size, sizeof(_size) );

  GDB_ASSERTF( size() > 0,  "Expected graph size to be > 0" );

}


} // end namespace cbrc

