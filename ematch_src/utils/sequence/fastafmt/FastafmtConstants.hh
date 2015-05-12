/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 09:36:03 $
 *
 *  Description: Define non-configurable fastafmt strings
 *
 */
#ifndef _FASTAFMTCONSTANTS_HH
#define _FASTAFMTCONSTANTS_HH
#include <iostream>

namespace cbrc{

class FastafmtConstants{
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
#endif // _FASTAFMTCONSTANTS_HH
