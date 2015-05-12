/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.19
 *  Last Modified: $Date: 2009/04/20 21:44:21 $
 *
 *  Purpose: try out code involving FLESorted read and write methods.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../FLESorted.hh"

namespace cbrc{
  void tryFLESorted_readWrite( const std::string& filename,
			       const std::vector<int>& ints ){

    // create FLESorted object
    FLESorted<int> sortedInts( ints );

    // open filename for write
    std::ofstream ofs;
    perlish::openCautiously( ofs, filename );

    // write to file
    sortedInts.write( ofs );
    ofs.close();

    // read FLESorted object in from string
    std::ifstream ifs( filename.c_str() );
    FLESorted<int> readinSortedInts( ifs );

    std::cout << "Array readinSortedInts:" << std::endl;
    for( size_t i = 0; i < readinSortedInts.size(); ++i ){
      if( i ) std::cout << " ";
      std::cout << readinSortedInts(i);
    }
    std::cout << std::endl;
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "outputFile int1 [int2...]" );
  args.printDoc();

  std::string outputFilename(   args.shiftOrDie()   );

  std::vector<int> ints;
  for( std::string curString = args.shift();
       curString.size();
       curString = args.shift() ){
    ints.push_back(   cbrc::perlish::toInt( curString )   );
  }

  args.dieIfUnusedArgs();

  cbrc::tryFLESorted_readWrite( outputFilename, ints );
  return 1;
}

