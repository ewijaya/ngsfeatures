/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.7.7
 *
 *  Last Modified: $Date: 2009/04/25 07:34:08 $
 *  
 *  Description: See header files.
 *
 *  Purpose:
 */
#include "SerialNumbersForStrings.hh"


namespace cbrc{

std::ostream& operator<<( std::ostream& os, const SerialNumbersForStrings& sns ){
  for( size_t i = 0; i < sns.size(); ++i ){
    os << i << " " << sns[i] << std::endl;
  }
  return os;
}

}; // end namespace cbrc
