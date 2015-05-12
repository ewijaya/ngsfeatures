/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.13
 *  Last Modified: $Date: 2008/06/13 04:55:10 $
 *
 *  Purpose: try code involving ResidueSimilarityAffineEdit
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueSimilarityAffineEdit.hh"

namespace cbrc{
  void tryResidueSimilarityAffineEdit(){

    std::cout << " residueSimilarityAffineEdit_amino.maxScore(): "
	      << residueSimilarityAffineEdit_amino.maxScore() << std::endl;

    std::cout << " residueSimilarityAffineEdit_amino.minScore(): "
	      << residueSimilarityAffineEdit_amino.minScore() << std::endl;
    
    std::cout << " residueSimilarityAffineEdit_amino.gapInitiation(): "
	      << residueSimilarityAffineEdit_amino.gapInitiation() << std::endl;

    std::cout << " residueSimilarityAffineEdit_amino.gapExtension(): "
	      << residueSimilarityAffineEdit_amino.gapExtension() << std::endl;

    std::cout << " residueSimilarityAffineEdit_amino.score( 0, 0 ): " << residueSimilarityAffineEdit_amino.score( 0, 0 ) << std::endl;

    std::cout << " residueSimilarityAffineEdit_amino.score( 0, 1 ): " << residueSimilarityAffineEdit_amino.score( 0, 1 ) << std::endl;

    std::cout << " residueSimilarityAffineEdit_amino.score( 1, 1 ): " << residueSimilarityAffineEdit_amino.score( 1, 1 ) << std::endl;
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no arguments." );
  
  args.dieIfUnusedArgs();

  cbrc::tryResidueSimilarityAffineEdit();
  return 1;
}

