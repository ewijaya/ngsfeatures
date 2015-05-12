/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.8
 *  Last Modified: $Date: 2009/05/08 07:22:52 $
 *
 *  Purpose: try code involving PushbackLiteral
 */
#include <iostream>
#include "../pushbackLiteral.hh"



namespace cbrc{

void tryPushbackLiteral(){
  std::vector<unsigned> uvals;

  pushbackLiteral( uvals, " 34 20 92 10 0 39" );

  std::cout << "Array unsigned vals:" << std::endl;
  for( size_t i = 0; i < uvals.size(); ++i ){
    if( i ) std::cout << " ";
    std::cout << uvals[i];
  }
  std::cout << std::endl;
    

  std::vector<double> dvals;

  pushbackLiteral( dvals, "3.14 2.72 99.6" );

  std::cout << "Array double vals:" << std::endl;
  for( size_t i = 0; i < dvals.size(); ++i ){
    if( i ) std::cout << " ";
    std::cout << dvals[i];
  }
  std::cout << std::endl;


} // end tryPushbackLiteral()    



} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::tryPushbackLiteral();
  return 1;
}

