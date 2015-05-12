/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.19
 *  
 *  Last Modified: 2003.7.15
 *  
 *  Purpose: test FLArray.
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "../FLArray.hh"

namespace cbrc{

  void print( int elem ){
    std::cout << elem << " ";
  }

  void testForEach(){
    std::vector<int> v;
    for( size_t i = 0; i < 10; ++i )  v.push_back(i);
    std::cout << "print vector: ";
    for_each( v.begin(), v.end(), print );
    std::cout << std::endl;

    FLNumArray<int> a(v);
    std::cout << "\nprint array: ";
    std::for_each( a.begin(), a.end(), print );
    std::cout << std::endl;
  }
}


int main(){
  cbrc::testForEach();
  return 1;
}
