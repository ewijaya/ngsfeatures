/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.8.19
 *
 *  Last Modified: 2003.8.19
 *  
 *  Purpose: test FLStack.
 */
#include <iostream>
#include "../FLStack.hh"

void testFLStack(){
  const int size=5;
  cbrc::FLStack<int> stack( size );
  stack.push( 3 );
  stack.push( 7 );
  for( cbrc::FLStack<int>::const_iterator p = stack.begin(); p != stack.end(); ++p ){
    std::cout << *p << ", ";
  }
  std::cout << std::endl;

  std::cout << "poped once\n";
  stack.pop();
  std::cout << "stack: " << stack << "\n";

  try{
    stack.pop();
    stack.pop();
  }catch( std::out_of_range& err ){
    std::cout << "out of range err says: " << err.what() << std::endl;
  }
}

int main(){
  testFLStack();
  return 1;
}

