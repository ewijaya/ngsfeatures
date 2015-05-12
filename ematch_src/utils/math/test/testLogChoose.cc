/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.9.4
 *
 *  Last Modified: 2003.9.4
 *  
 *  Purpose: test LogChoose.
 */

#include <iostream>
#include "../LogChoose.hh"

void testLogChoose(){
  cbrc::LogChoose lc( 5 );

  std::cout << "4 choose 2 = " << lc( 4, 2 ) << std::endl;
}

int main(){
  testLogChoose();
  return 1;
}

