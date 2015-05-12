/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.4.15
 *  Last Modified: $Date: 2008/04/14 23:18:37 $
 *
 *  Description: Some functions regarding meminfo size
 *
 *  Purpose: General purpose.
 *
 */
#ifndef _MEMINFO_HH
#define _MEMINFO_HH
#include <iostream>

namespace cbrc{

namespace meminfo{
  // total memory in bytes
  size_t total();
}

} // end namespace cbrc
#endif // _MEMINFO_HH
