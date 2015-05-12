/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.29
 *  
 *  Last Modified: 2003.7.18
 *  
 *  Purpose: test FLMatrix.
 */
#include <iostream>
#include "../FLMatrix.hh"

void testFLMatrix(){
  const unsigned int s1 = 4;
  cbrc::FLNumMatrix<double> flm( 6, s1, 0.25 );
  std::cout << flm;

  double a[s1] = {0.2, 0.4, 0.1, 0.3};
  cbrc::FLArray<double> fla( a, s1 );

  flm.shiftLeft( fla );
  flm.shiftLeft( fla );

  std::cout << "shifted left twice:\n" << flm;

  flm.shiftRight( fla );

  std::cout << "shifted right:\n" << flm;

  flm += 0.1;

  std::cout << "added 0.1\n" << flm;

  flm /= 2;

  std::cout << "divided by 2\n" << flm;

  std::cout << "sum 0 2: " << flm.sum( 0, 2 ) << std::endl;
  std::cout << "sum 1 2: " << flm.sum( 1, 2 ) << std::endl;

  flm.normalize( 0 );

  std::cout << "normalized:\n" << flm;

  flm += flm;

  std::cout << "used += to double:\n" << flm;
}

int main(){
  testFLMatrix();
  return 1;
}

