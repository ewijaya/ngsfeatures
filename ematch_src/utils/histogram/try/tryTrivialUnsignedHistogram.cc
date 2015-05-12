/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.29
 *  Last Modified: $Date: 2006/04/30 12:56:11 $
 *
 *  Purpose: try or run code involving TrivialUnsignedHistogram.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../TrivialUnsignedHistogram.hh"

namespace cbrc{
  void tryTrivialUnsignedHistogram(){
    // make FLENumArray
    size_t a[3] = {2, 2, 0};
    std::vector<size_t> v;
    for( size_t i = 0; i < 3; ++i ) v.push_back( a[i] );
    FLENumArray<size_t> b(v);

    // construct and print histogram
    TrivialUnsignedHistogram<size_t> histo( b );

    std::cout << "histo: " << histo << std::endl;

    std::cout << "non zero entries:\n";
    histo.printNonzeroEntries();
    std::cout << std::endl;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  args.dieIfUnusedArgs();
  cbrc::tryTrivialUnsignedHistogram();
  return 1;
}

