/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.3
 *  Last Modified: $Date: 2008/08/10 13:40:56 $
 *
 *  Purpose: try or run code involving TopKSorted
 */
#include <iostream>
#include <vector>
#include "utils/argvParsing/ArgvParser.hh"
#include "../TopKSorted.hh"

struct argsT{
  size_t k;
  size_t maxElemsInMemory;
} args;


namespace cbrc{


void tryTopKSorted(){
  void printArray( const FLEArray<float>& arr );
  size_t sz = 1000000;

  TopKSorted< float, std::less<float> > topK( args.k, args.maxElemsInMemory );
  for( size_t i = 0; i < sz; ++i ){
    topK.insertValueIfInTopK( drand48() );
  }

  //  std::cout << "original array: " << f;
  std::cout << "top k:\n";
  for( size_t i = 0; i < args.k; ++i ){
    std::cout << topK(i) << " ";
  }
  std::cout << std::endl;

  topK.sort();
  std::cout << "printing as array\n";
  printArray( topK.stack() );
}


  // auxillary function of tryTopKSorted
  void printArray( const FLEArray<float>& arr ){
    std::cout << "Array arr:" << std::endl;
    for( size_t i = 0; i < arr.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout << arr[i];
    }
    std::cout << std::endl;
  }

}; // end namespace cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "Usage: $0 k maxElemsInMemory\n\
\n\
Reasonable values for k and maxElemsInMemory would be something like:\n\
% $0 10 4096\n\
" );

  std::cout << "Command line: " << argP.commandLine() << std::endl;
  
  argP.setOrDie( args.k, 1 );
  argP.setOrDie( args.maxElemsInMemory, 2 );

  argP.dieIfUnusedArgs();

  cbrc::tryTopKSorted();
  return 1;
}


