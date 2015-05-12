/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.??.??
 *  Last Modified: $Date: 2009/04/20 12:01:25 $
 *
 *  Purpose: try code using ConstantLabeledSquareDoubleFLENumMatrix::renumLabels()
 *           called with a SerialNumbersForStrings_accessCounting object.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings_accessCounting.hh"
#include "../ConstantLabeledSquareDoubleFLENumMatrix.hh"

namespace cbrc{
  void tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels_SerialNumbersForStrings_accessCounting( std::istream& matrixInputStream ){


    // **** read in matrix and print it ****
    ConstantLabeledSquareDoubleFLENumMatrix mat( matrixInputStream );

    std::cout << "matrix is:\n" << mat;

    const size_t indexToAccess = 1;

    if( mat.size0() <= indexToAccess ){
      std::cout << "Expecting a matrix of size at least " << indexToAccess << std::endl;
      exit( -1 );
    }

    /* construct SerialNumbersForStrings object with same
     * labels as mat, but in reverse order
     */
    SerialNumbersForStrings_accessCounting<size_t> sns( mat.labels() );

    sns.getSerialNumber( mat.labels()(indexToAccess) );

    std::cout << "new labels are:\n" << sns << std::endl;


    // **** renum mat labels and print result ****
    mat.renumLabels( sns );
    std::cout << "matrix after relabeling is:\n" << mat;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "matrixInputFile" );

  std::ifstream matrixInputFile;
  args.setOrDie( matrixInputFile, 1 );
  args.dieIfUnusedArgs();
  cbrc::tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels_SerialNumbersForStrings_accessCounting( matrixInputFile );
  return 1;
}
