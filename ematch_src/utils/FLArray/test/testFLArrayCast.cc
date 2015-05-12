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
#include "../FLArray.hh"


namespace cbrc{

double vectorSum( std::vector<double> a ){
  std::cout << " size a is: " << a.size() << std::endl;
  double sum = 0.0;
  for( size_t i = 0; i < a.size(); ++i ){
    sum += a[i];
  }
  return sum;
}

void testFLArray(){
  size_t n = 10;
  std::vector<double> v(n);
  for( size_t i = 0; i < v.size(); ++i ){
    v[i] = i;
  }
  std::cout << "vector sum = " << vectorSum(v) << std::endl;

  FLNumArray<double> b(n);
  for( size_t i = 0; i < b.size(); ++i ){
    b[i] = i;
  }
  std::cout << "array sum = " << vectorSum(b) << std::endl;
}

}; // end namescape cbrc

int main(){
  cbrc::testFLArray();
  return 1;
}
