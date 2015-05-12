/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.7.22
 *
 *  Last Modified: 2003.7.22
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test FLArrayFunctions.
 */
#include <iostream>
#include "../FLArrayFunctions.hh"

void testFLArrayFunctions(){
  cbrc::FLNumArray<unsigned int> a( 7 );
  cbrc::FLArrayFunctions::setNumCombinationsVector<unsigned int>( a );
  
  std::cout << "combinations are:\n  " << a << std::endl;

  std::vector< std::string > testStrings;
  testStrings.push_back( "id01" );
  testStrings.push_back( "iris" );
  testStrings.push_back( "-1.9" );
  testStrings.push_back( "45" );
  testStrings.push_back( "34.9" );

  cbrc::FLNumArray<double> nums = cbrc::FLArrayFunctions::doubleFLNumArrayFromStrings( testStrings, 2, 5 );

 std::cout << "nums are: " << nums << std::endl;
  
  
}

int main(){
  testFLArrayFunctions();
  return 1;
}

