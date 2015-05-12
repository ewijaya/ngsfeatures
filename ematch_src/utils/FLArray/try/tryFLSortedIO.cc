/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005-2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.6.8
 *  Last Modified: $Date: 2007/07/16 00:48:51 $
 *
 *  Purpose: run code involving FLSortedIO.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLSortedFunctions.hh"



namespace cbrc{
  void runFLSortedIO( const ArgvParser& args, std::string tempFilename ){

    // make list of string for testing
    std::vector<std::string> testList;
    testList.push_back( "ted" );
    testList.push_back( "larry" );
    testList.push_back( "harry" );
    testList.push_back( "zona" );

    // create FLSorted object with it.
    FLSorted<std::string> sortedList( testList );

    // write FLSorted to temp file
    std::ofstream os;
    args.openCautiously( os, tempFilename );
    FLSortedFunctions::write( sortedList, os );
    os.close();

    // read new FSorted object in from same file
    FLSorted<std::string> newSorted;
    std::ifstream is( tempFilename.c_str() );
    FLSortedFunctions::read( newSorted, is );
    std::cout << "Array newSorted:" << std::endl;
    std::cout << newSorted << std::endl;
  }
}; // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "tempFile" );

  std::string tempFilename = args[1];
  
  args.dieIfUnusedArgs();
  cbrc::runFLSortedIO( args, tempFilename );
  return 1;
}

