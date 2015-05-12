/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.20
 *  Last Modified: $Date: 2007/09/20 05:30:52 $
 *
 *  Purpose: try out ResidueIndexMap constructor argument checks
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMap.hh"

namespace cbrc{
  void tryResidueIndexMap_constructorArgumentChecks(){

    std::cout << "try ResidueIndexMap( \"acgt\", \"acgtn\" )\n";
    try{
      ResidueIndexMap rim( "acgt", "acgtn" );
    }
    catch( std::invalid_argument& err ){
      std::cout << "err: " << err.what() << std::endl;
    }


    std::cout << "\ntry ResidueIndexMap( \"acgtn\", \"acgt\" )\n";
    try{
      ResidueIndexMap rim( "acgtn", "acgt" );
    }
    catch( std::invalid_argument& err ){
      std::cout << "err: " << err.what() << std::endl;
    }


    std::cout << "\ntry ResidueIndexMap( \"acgt\", \"tgca\" )\n";
    try{
      ResidueIndexMap rim( "acgt", "tgca" );
    }
    catch( std::invalid_argument& err ){
      std::cout << "err: " << err.what() << std::endl;
    }


    std::cout << "\ntry ResidueIndexMap( \"acgt\", \"acgtTACG\" )\n";
    try{
      ResidueIndexMap rim( "acgt", "acgtTACG" );
    }
    catch( std::invalid_argument& err ){
      std::cout << "err: " << err.what() << std::endl;
    }


    std::cout << "\ntry ResidueIndexMap( \"acgt\", \"acgtTACG\", true )\n";
    try{
      ResidueIndexMap rim( "acgt", "acgtTACG", true );
    }
    catch( std::invalid_argument& err ){
      std::cout << "err: " << err.what() << std::endl;
    }

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMap_constructorArgumentChecks();
  return 1;
}

