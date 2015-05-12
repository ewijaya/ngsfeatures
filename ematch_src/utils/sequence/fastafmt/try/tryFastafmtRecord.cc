/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 09:13:54 $
 *
 *  Purpose: try code involving FastafmtRecord
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastafmtRecord.hh"

namespace cbrc{
  void tryFastafmtRecord(){
    FastafmtRecord fastafmtRecord1( "MTYHTHHVDUFDFKDVN" );

    std::cout << "first test record is: " << fastafmtRecord1;

    std::vector< std::string > labels;
    labels.push_back( "genomeRegion1" );
    labels.push_back( "mammalian" );

    FastafmtRecord fastafmtRecord2( "gatgctcgctgagctgatcgaatgcta", labels );

    std::cout << "second test record is: " << fastafmtRecord2;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "No args!" );
  args.dieIfUnusedArgs();
  cbrc::tryFastafmtRecord();
  return 1;
}

