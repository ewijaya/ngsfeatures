/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.12
 *  Last Modified: $Date: 2006/09/14 11:24:26 $
 *
 *  Purpose: try or run code involving TransfacReader
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/vectorMatrix.hh"
#include "../TransfacReader.hh"

namespace cbrc{
  void tryTransfacReader(){
    TransfacReader transfacReader( std::cin );
    std::cout << "version: " << transfacReader.version() << std::endl;
    transfacReader.readRecord();
    std::cout << "accession: " << transfacReader.accession() << std::endl;
    std::cout << "description: " << transfacReader.description() << std::endl;
    std::cout << "name: " << transfacReader.name() << std::endl;
    std::cout << "binding factor: " << transfacReader.bindingFactor() << std::endl;
    std::cout << "alphabet: " << transfacReader.alphabet() << std::endl;
    std::cout << "consensus sequence: " << transfacReader.consensusSequence() << std::endl;
    std::cout << "PSSM:\n";
    std::cout << transfacReader.freqMatrix();
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Pipe transfac text type file from standard in." );
  args.dieIfUnusedArgs();
  cbrc::tryTransfacReader();
  return 1;
}

