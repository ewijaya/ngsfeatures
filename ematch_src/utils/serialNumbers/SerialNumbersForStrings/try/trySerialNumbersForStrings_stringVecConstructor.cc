/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.11.30
 *  Last Modified: $Date: 2008/03/01 22:37:47 $
 *
 *  Purpose: try code involving std::vector<std::string> constructor
 *           for SerialNumbersForStrings
 */
#include <iostream>
#include "../SerialNumbersForStrings.hh"

namespace cbrc{
  void trySerialNumberString_stringVecConstructor(){
    std::vector<std::string> strings;
    strings.push_back( "ashcroft" );
    strings.push_back( "haste" );
    strings.push_back( "telegraph" );
    strings.push_back( "hearst" );
    strings.push_back( "euclid" );

    SerialNumbersForStrings<size_t> sns( strings );

    std::cout << "serial numbers for strings is:\n"
	      << sns << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::trySerialNumberString_stringVecConstructor();
  return 1;
}


