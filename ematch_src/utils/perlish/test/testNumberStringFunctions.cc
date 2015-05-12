/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.2.19
 *
 *  Last Modified: 2004.2.19
 *  
 *  Purpose: test UnsignedIntFunctions.
 */
#include <iostream>
#include "../perlish.hh"

namespace cbrc{
  void testNumberStringFunctions(){
    std::string a( "1" );
    std::string b( "11" );

    std::cout << "a: " << a << " is unsigned int? " << perlish::looksLikeUnsignedInt( a.c_str() ) << std::endl;
    std::cout << "b: " << b << " is unsigned int? " << perlish::looksLikeUnsignedInt( b.c_str() ) << std::endl;

    a = "-9.23";
    std::cout << "a: " << a << " is real? " << perlish::looksLikeReal( a ) << std::endl;
    double d = perlish::toDouble( a );
    std::cout << "a = " << d << std::endl;

    a = "fred39";
    std::cout << "a: " << a << "(+4) is unsigned int? " << perlish::looksLikeUnsignedInt( a, 4 ) << std::endl;
    unsigned int i = perlish::toUnsignedInt( a, 4 );
    std::cout << "a(+4) = " << i << std::endl;

  }
}; // end namescape cbrc

int main(){
  cbrc::testNumberStringFunctions();
  return 1;
}

