/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.7.16
 *
 *  Last cvs check in: $Date: 2004/07/15 21:39:12 $
 *  
 *  Purpose: Test whether __i386__
 */
#include <iostream>

namespace cbrc{
  void testPlatformMacro(){
#ifdef __i386__
    std::cout << __FILE__ << ":" << __LINE__ << " Platform is i386\n";
#else
    std::cout << __FILE__ << ":" << __LINE__ << " Platform is not i386\n";
#endif // defined __i386__
  }
}; // end namescape cbrc

int main(){
  cbrc::testPlatformMacro();
  return 1;
}

