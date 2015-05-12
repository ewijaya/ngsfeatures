/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.30
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: test Ttable.
 *  g++ testTtable-gmp.cc -lgmpxx -lgmp -lmpfr
 */
#include <iostream>
#include "../Ttable.hh"

int main(int argc, char **argv){

  if(argc < 3) {
    std::cerr << argv[0] << " t_value d_of_freedom" << std::endl;
    exit(1);
  }
  double t = atof(argv[1]);
  double df = atof(argv[2]);
  
  std::cout << cbrc::TtoP(t, df) << std::endl;
}

