/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.6.8
 *
 *  Last Modified: $Date: 2007/09/19 03:57:23 $
 *  
 *  Description: Some functions related to FLSorted.
 *
 */
#include "utils/FLArray/FLSorted.hh"

namespace cbrc{

namespace FLSortedFunctions{
  void read( FLSorted<std::string>& ss, std::istream& is );
  void write( const FLSorted<std::string>& ss, std::ostream& os );
} // end namespace FLSortedFunctions.

} // end namespace cbrc.
