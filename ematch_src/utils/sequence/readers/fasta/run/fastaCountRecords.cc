/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.1.12
 *  Last Modified: $Date: 2009/05/22 14:36:15 $
 *
 *  Description: Count number of records of a multifasta format file.
 *
 *               See main for details.
 *     
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "../fastafmtFunctions.hh"



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage:\n\n  $0 --help\n  $0 [FILE]..." );
  args.setDoc( "--help",
 "$0: Count number of records of a multifasta format file.\n"\
 "No error checking is done.\n"\
 "\n"\
 "    $ $0 foo.fasta\n"\
 "\n"\
 "is simple a faster version of\n"\
 "\n"\
 "    $ grep \"^>\" foo.fasta | wc -l\n"\
 "\n"\
 "Command line arguments are more or less defined to act like\n"\
 "the unix command \"wc\".\n"
	       );
  args.printDoc();

  std::ifstream fastaStream;
  unsigned int numRecords;

  if( args.unusedArgs().size() == 0 ){
    numRecords = cbrc::fastafmtFunctions::countRecords( std::cin );
    std::cout << numRecords << std::endl;
  }else{
    const char* filename;
    while(   (  filename = args.setFromNextUnused( fastaStream )  )   ){
      numRecords = cbrc::fastafmtFunctions::countRecords( fastaStream );
      fastaStream.close();
      std::cout  <<  filename  <<  " "  <<  numRecords  <<  std::endl;
    }
  }

  return 1;
}

