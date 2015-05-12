/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.18
 *  Last Modified: $Date: 2008/06/30 07:26:47 $
 *
 *  Purpose: try or run code involving ResidueIndexMap_extendAlphabet
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapPredefined.hh"

namespace cbrc{
  void tryResidueIndexMap_extendAlphabet(){
    ResidueIndexMap rim = DNAProperResidueIndexMap;

    rim.extendAlphabet( '-' );

    std::cout << "test if \'-\' is considered to be a valid residue\n";
    std::cout << (   rim.validResidue( '-' ) ? "yes\n" : "no\n"   );
  }
}; // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args" );
  
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMap_extendAlphabet();
  return 1;
}

