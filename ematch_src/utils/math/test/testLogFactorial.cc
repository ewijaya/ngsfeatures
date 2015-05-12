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
 *  Purpose: test LogFactorial.
 */
#include <iostream>
#include "../LogFactorial.hh"

void testLogFactorial(){
  cbrc::LogFactorial f(40);

  for( size_t i = 0; i < f.size(); ++i ){
    std::cout << "log " << i << "! = " << f(i) << std::endl;
  }
}

int main(){
  testLogFactorial();
  return 1;
}

