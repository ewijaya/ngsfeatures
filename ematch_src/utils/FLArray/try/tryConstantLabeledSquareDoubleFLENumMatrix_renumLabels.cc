/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.3
 *  Last Modified: $Date: 2008/07/23 02:16:16 $
 *
 *  Purpose: try code involving ConstantLabeledSquareDoubleFLENumMatrix
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ConstantLabeledSquareDoubleFLENumMatrix.hh"

namespace cbrc{
  void tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels( std::istream& matrixInputStream ){

    // **** read in matrix and print it ****
    ConstantLabeledSquareDoubleFLENumMatrix mat( matrixInputStream );

    std::cout << "matrix is:\n" << mat;


    /* construct SerialNumbersForStrings object with same
     * labels as mat, but in reverse order
     */
    const SerialNumbersForStrings<size_t>& labelStrings = mat.labels();

    SerialNumbersForStrings<size_t> labelStringsReversed;
    
    for( size_t i = labelStrings.size()-1; i <= labelStrings.size(); --i ){
      labelStringsReversed.getSerialNumber( labelStrings(i) );
    }

    std::cout << "new labels are:\n" << labelStringsReversed << std::endl;


    // **** renum mat labels and print result ****
    mat.renumLabels( labelStringsReversed );

    std::cout << "matrix after relabeling is:\n" << mat;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "matrixInputFile" );

  std::ifstream matrixInputFile;
  args.setOrDie( matrixInputFile, 1 );
  args.dieIfUnusedArgs();
  cbrc::tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels( matrixInputFile );
  return 1;
}
