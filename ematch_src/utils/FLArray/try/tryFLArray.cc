/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.19
 *  Last Modified: $Date: 2008/07/01 01:39:40 $
 *
 *  Description: Try out FLArray class
 */
#include <iostream>
#include <vector>
#include "../FLArray.hh"

void tryFLArray(){

  cbrc::FLArray<std::string> flStringArray( 3 );

  flStringArray[0] = std::string( "ted" );
  flStringArray[1] = std::string( "freddy" );
  flStringArray[2] = std::string( "larry" );
  // flStringArray[3] = std::string( "overflow" ); // uncomment out to test range checking.

  std::cout << flStringArray << std::endl;

  std::cout << "print flStringArray using iterators: " << std::endl;
  for( cbrc::FLArray<std::string>::const_iterator p = flStringArray.begin();
       p != flStringArray.end(); ++p ){
    std::cout << *p << ", ";
  }
  std::cout << std::endl;

}



int main(){
  tryFLArray();
  return 1;
}
