/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.9
 *  Last Modified: $Date: 2008/09/04 07:30:30 $
 *
 *  Purpose: try or run code involving ArgvParser set functions
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "../ArgvParser.hh"
#define  BOOL_FLAGS  -f|--flag


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "--stringOpt string --infile infile ["Q(BOOL_FLAGS)"]" );

  /* ** parse arguments ** */
  std::string stringOpt;
  args.set( stringOpt, "--stringOpt" );

  std::ifstream ifstream;
  args.set( ifstream, "--infile" );

  bool boolFlag;
  std::string boolFlagString(  args.set( boolFlag, Q(BOOL_FLAGS) )  );

  args.dieIfUnusedArgs();


  /* ** dump read in arguments ** */
  std::cout << "stringOpt: " << stringOpt << std::endl;

  if( boolFlag ){
    std::cout << "boolFlagString: " << boolFlagString << std::endl;
  }else{
    std::cout << "boolFlag not set!\n";
  }


  std::vector<std::string> ifstreamContent = cbrc::perlish::slurpLines( ifstream );
  std::cout << "Contents of ifstream:\n";
  std::cout << cbrc::perlish::join( "\n", ifstreamContent ) << std::endl;

  return 1;
}

