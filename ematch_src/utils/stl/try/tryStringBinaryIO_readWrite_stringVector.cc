/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.18
 *  Last Modified: $Date: 2008/01/18 14:05:24 $
 *
 *  Purpose: try code involving StringBinaryIO for std::vector<std::string>
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../stringBinaryIO.hh"

namespace cbrc{
  void tryStringBinaryIO_readWrite_stringVector( const std::string& filename,
						 const std::vector<std::string> stringVector ){
    // open filename for write
    std::ofstream ofs;
    perlish::openCautiously( ofs, filename );

    // write to file
    stringBinaryIO::write( stringVector, ofs );
    ofs.close();

    // read new stringVector in from file
    std::ifstream ifs( filename.c_str() );
    std::vector<std::string> readinStringVector;
    stringBinaryIO::read( readinStringVector, ifs );

    // print read in stringVector out.
    for( size_t i = 0; i < readinStringVector.size(); ++i ){
      std::cout << readinStringVector[i] << std::endl;
    }
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "outputFile string1 [string2...]" );
  std::string outputFilename(   args.shiftOrDie()   );
  
  std::vector<std::string> stringVector;

  for( std::string curString = args.shift();
       curString.size();
       curString = args.shift() ){
    stringVector.push_back( curString );
  }


  args.dieIfUnusedArgs();
  cbrc::tryStringBinaryIO_readWrite_stringVector( outputFilename, stringVector );
  return 1;
}

