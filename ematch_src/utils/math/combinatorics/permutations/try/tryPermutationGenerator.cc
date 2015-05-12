/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.24
 *  Last Modified: $Date: 2006/04/30 13:41:31 $
 *
 *  Purpose: try out code involving PermutationGenerator.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/histogram/TrivialUnsignedHistogram.hh"
#include "../PermutationGenerator.hh"

namespace cbrc{
  // a consistent permutation should have a count of one for
  // each integer from 0 to pg.size()
  void permutationConsistent( const PermutationGenerator& pg ){
    TrivialUnsignedHistogram<size_t> histo( pg.arrayRef() );
    for( size_t i = 0; i < histo.size(); ++i ){
      assert( histo(i) == 1 );
    }
  }

  // print out all permutations of permSize
  void tryPermutationGenerator( const size_t permSize ){
    PermutationGenerator pg( permSize );
    permutationConsistent( pg );
    std::cout << pg << std::endl;
    while( pg.next() ){
      permutationConsistent( pg );
      std::cout << pg << std::endl;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "permutation_size" );
  size_t permSize;
  args.set( permSize, 1 );
  args.dieIfUnusedArgs();
  cbrc::tryPermutationGenerator( permSize );
  return 1;
}

