/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.03
 *  Last Modified: $Date: 2009/01/29 21:57:33 $
 *
 *  Purpose: test whether ConstantLabeledSquareDoubleFLENumMatrix is symmetric.
 *
 *  Input: ConstantLabeledSquareDoubleFLENumMatrix streamed from standard in.
 *
 */
#include <iostream>
#include "../ConstantLabeledSquareDoubleFLENumMatrix.hh"

namespace cbrc{
  void isConstantLabeledSquareDoubleFLENumMatrixSymmetric(){
    ConstantLabeledSquareDoubleFLENumMatrix mat( std::cin );
    if( mat.symmetric() ){
      std::cout << "matrix is symmetric\n";
    }
    else{
      std::cout << "matrix is not symmetric!\n";
    }
  }
}; // end namescape cbrc

int main(){
  cbrc::isConstantLabeledSquareDoubleFLENumMatrixSymmetric();
  return 1;
}

