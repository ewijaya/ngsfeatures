/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.7
 *  Last Modified: $Date: 2008/06/30 07:25:12 $
 *
 *  Purpose: try ResidueIndexMap code involving extended DNA alphabets
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapPredefined.hh"


namespace cbrc{
  void tryExtendedDNAAlphabetSizeMethods(){
    // could also use const ResidueIndexMap& rim if the ResidueIndexMap to use does not change.
    ResidueIndexMap const* rimPtr = &DNAWithNResidueIndexMap;

    std::cout << "testing DNAWithNResidueIndexMap...\n";
    std::cout << "alphabet().size() = "
	      << rimPtr->alphabet().size()
	      << ",  aka sigma() = "
	      << rimPtr->sigma()
	      << std::endl;

    std::cout << "alphabetProper().size(): "
	      << rimPtr->alphabetProper().size()
	      << std::endl;
    


    rimPtr = &DNAExtendedResidueIndexMap;

    std::cout << "\n\ntesting DNAExtendedResidueIndexMap...\n";
    std::cout << "alphabet().size() = "
	      << rimPtr->alphabet().size()
	      << ",  aka sigma() = "
	      << rimPtr->sigma()
	      << std::endl;

    std::cout << "alphabetProper().size(): "
	      << rimPtr->alphabetProper().size()
	      << std::endl;
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  args.dieIfUnusedArgs();
  cbrc::tryExtendedDNAAlphabetSizeMethods();
  return 1;
}

