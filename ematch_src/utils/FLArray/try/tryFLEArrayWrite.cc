/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.10.26
 *
 *  Last Modified: 2004.10.26
 *  
 *  Purpose: try FLEArrayWrite.
 */
#include <iostream>
#include <fstream>
#include "../FLEArray.hh"


namespace cbrc{
  void tryFLEArrayWrite(){
    int a[] = { 5, 4, 3, 2, 1 };
    FLEArray<int> flea( 5, a );
    std::ofstream os( "binaryFile" );
    flea.write( os );
    os.close();
    std::ifstream is( "binaryFile" );
    FLEArray<int> readInArray( is );
    std::cout << "readInArray: " << readInArray << std::endl;
  }
}; // end namescape cbrc

int main( int argc, char* argv[] ){
  std::string usage( "Usage: " );
  usage += argv[0];

  cbrc::tryFLEArrayWrite();
  return 1;
}

