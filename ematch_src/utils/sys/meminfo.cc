/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.15
 *  Last Modified: $Date: 2009/04/21 08:16:36 $
 *
 *  Description: 
 *
 *  Purpose: Created for
 */
#include <string>
#include "utils/perlish/perlish.hh"
#include "meminfo.hh"

namespace cbrc{

namespace meminfo{

  /* parse /proc/meminfo to determine total memory in bytes
   * expecting a line like:
   *
   *     MemTotal:      1003908 kB
   */
  size_t total(){
    FILE* meminfoPipe
      = popen( "grep -i ^memtotal: /proc/meminfo | sed s/memtotal://i | tr -d [:space:]", "r" );

    // get memtotal line
    std::string line(  perlish::slurpLine( meminfoPipe )  );

    // confirm it contains "kB, then erase it.
    std::string::size_type kB_pos = line.find( "kB" );

    if( kB_pos == std::string::npos ){
      GDB_DIEF( "memtotal line did not end in \"kB\"\n" );
    }

    line.erase( kB_pos );

    // make sure string remaining in LINE is an integer, parse and return it (*1000)
    if( !perlish::looksLikeUnsignedInt( line ) ){
      GDB_DIEF( "memtotal line does not look like integer\n" );
    }

    return(  1000 * perlish::toUnsignedInt( line )  );
  }

} // end namespace meminfo

} // end namescape cbrc
