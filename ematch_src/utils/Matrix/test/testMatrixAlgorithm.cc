/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.18
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test MatrixAlgorithm.
 */
#include <iostream>
#include "../MatrixAlgorithm.hh"
#include "../Matrix.hh"

void testMatrixAlgorithm(){
  cbrc::Matrix<float> Mf(10,4);
  Mf[3][0] = 1;
  Mf[3][1] = 2;
  Mf[3][2] = 3;
  Mf[3][3] = 4;

  Mf[5][0] = 4;
  Mf[5][1] = 3;
  Mf[5][2] = 2;
  Mf[5][3] = 1;
  size_t data_valid;
  float result = cbrc::pearson_corr<float, double>(Mf, 3, 5, &data_valid);
  std::cout << "CC of 3-5 is " << result << " and valid data is " << data_valid << std::endl;

  cbrc::log_transform<float>(Mf);
}

int main(){
  testMatrixAlgorithm();
  return 1;
}

