/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.11.30
 *  Last Modified: $Date: 2008/03/01 23:10:24 $
 *
 *  Purpose: try code involving SerialNumbersForStrings
 */
#include <iostream>
#include "../SerialNumbersForStrings.hh"

namespace cbrc{
  void trySerialNumberString(){
    SerialNumbersForStrings<unsigned char> sns;
    std::cout << " serial number for hippo is "
	      << (size_t) sns.getSerialNumber( "hippo" ) << std::endl;

    std::cout << " serial number for giraffe is "
	      << (size_t) sns.getSerialNumber( "giraffe" ) << std::endl;

    std::cout << " serial number for hippo is "
	      << (size_t) sns.getSerialNumber( "hippo" ) << std::endl;

    std::cout << " serial number for elphant is "
	      << (size_t) sns.getSerialNumber( "elephant" ) << std::endl;

    std::cout << " serial number for hippo is "
	      << (size_t) sns.getSerialNumber( "hippo" ) << std::endl;

    if( sns.contains( "giraffe" ) )  std::cout << "has giraffe\n";

    if( !sns.contains( "teddy" ) )  std::cout << "but no teddy!\n";

    std::vector<std::string> strings = sns.strings();
    std::cout << "strings are: ";
    for( size_t i = 0; i < strings.size(); ++i ){
      std::cout << strings[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "\n strings are:\n";
    for( size_t i = 0; i < sns.size(); ++i ){
      std::cout << sns[i] << std::endl;
    }

    std::cout << "\nwith numbers:\n" << sns << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::trySerialNumberString();
  return 1;
}


