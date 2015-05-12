/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2009/08/25 12:01:27 $
 *
 *  Description: Define non-configurable fasta strings
 *
 */
#ifndef _FASTACONSTANTS_HH
#define _FASTACONSTANTS_HH
#include <iostream>

namespace cbrc{

class FastaConstants{
public:
  static const char& recordMarker(){
    static const char _recordMarker( '>' );
    return _recordMarker;
  }

  // Note: The seq termination marker is a constant but whether to use it is an option
  static const char& seqTerminationMarker(){
    static const char _recordMarker( '*' );
    return _recordMarker;
  }

private:
};

} // end namespace cbrc
#endif // _FASTACONSTANTS_HH
