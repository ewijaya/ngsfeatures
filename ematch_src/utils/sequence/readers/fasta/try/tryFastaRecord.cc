/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2009/04/30 21:20:28 $
 *
 *  Purpose: try code involving FastaRecord
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastaRecord.hh"

namespace cbrc{
  void tryFastaRecord(){
    FastaRecord fastaRecord1( "MTYHTHHVDUFDFKDVN" );

    std::cout << "first test record is: " << fastaRecord1;

    std::vector< std::string > labels;
    labels.push_back( "genomeRegion1" );
    labels.push_back( "mammalian" );

    FastaRecord fastaRecord2( "gatgctcgctgagctgatcgaatgcta", labels );

    std::cout << "second test record is: " << fastaRecord2;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "No args!" );
  args.dieIfUnusedArgs();
  cbrc::tryFastaRecord();
  return 1;
}

