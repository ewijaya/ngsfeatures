/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2004.1.5
 *  Last Modified: 2004.1.5
 *  
 *  Purpose: try out ArgvParser eat method.
 */
#include <iostream>
#include "../ArgvParser.hh"

int main( int argc, const char** argv ){
  cbrc::ArgvParser args( argc, argv );
  
  std::cout << " arg count is: " << args.argc() << std::endl;
  for( int i = 0; i < args.argc(); ++i ){
    std::cout << "  " << args[i] << std::endl;
  }

  if( args.eat( "-v" ) ){
    std::cout <<  "ate a -v\n";
  }else{
    std::cout << "no -v!\n";
  }

  std::cout << " arg count is: " << args.argc() << std::endl;
  for( int i = 0; i < args.argc(); ++i ){
    std::cout << "  " << args[i] << std::endl;
  }

  return 1;
}
