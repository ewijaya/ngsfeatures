/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.11
 *  Last Modified: $Date: 2009/05/12 02:30:15 $
 *
 *  Description: Some constants for the binary format of
 *               the RecountNeighborProbGraph
 *
 *  Purpose: Created for the RECOUNT project
 *
 */
#ifndef RECOUNTNEIGHBORPROBGRAPHFORMAT_HH_
#define RECOUNTNEIGHBORPROBGRAPHFORMAT_HH_
#include <iostream>

namespace cbrc{

namespace RecountNeighborProbGraphFormat{


  /* ********** CONSTANTS ********** */
  inline const std::string&  signature(){
    static const std::string  _signature( "recountGraph000\n" );
    return _signature;
  }


  inline std::ifstream::pos_type  headerSize(){
    //            signature         nodeCount
    return(  signature().size() + sizeof(size_t)  );
  }

}


} // end namespace cbrc
#endif // RECOUNTNEIGHBORPROBGRAPHFORMAT_HH_
