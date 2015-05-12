/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.06.13
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test Matrix.
 */
#include <iostream>
#include "../Matrix.hh"

void testMatrix(){
  cbrc::Matrix<double> m1(8,6);
  m1.resize(8794,86);
  cbrc::Matrix<double> m2(3,5);
  std::cout << "m1 row x col: " << m1.row_size() << " x " << m1.column_size() << std::endl;
  std::cout << "m2 row x col: " << m2.row_size() << " x " << m2.column_size() << std::endl;
  m1[0][2] = 3;
  m1[8793][1] = 3;
  m1 = m2;
  m2[2][2] = m2[2][2];
  std::cout << m2[0][2] << std::endl;
  std::cout << "m1 row x col: " << m1.row_size() << " x " << m1.column_size() << std::endl;
  std::cout << "m2 row x col: " << m2.row_size() << " x " << m2.column_size() << std::endl;
  m1[2][3] = m2[4][2];
}

int main(int argc, char ** argv){
  testMatrix();
  return 1;
}
