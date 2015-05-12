/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.18
 *  Last Modified: $Date: 2008/11/19 10:06:30 $
 *
 *  Description: Grep-like filter for organism classification of
 *               records in a uniprot .dat text file.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../SprotReader.hh"


struct argsT{
  std::string   query;
  std::istream* sprotIstreamPtr;
} args;


namespace cbrc{

  void sprotGrepOC(){

    SprotReader sprotReader;


    while(  sprotReader.readRecord( *args.sprotIstreamPtr )  ){

      if(  sprotReader.organismClassification().find( args.query )
	   != std::string::npos ){
	std::cout << sprotReader;
      }
    }
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "query [sprotFile]" );

  argP.setOrDie( args.query, 1 );

  args.sprotIstreamPtr = argP.getIstreamPtr( 2 );
  
  argP.dieIfUnusedArgs();
  cbrc::sprotGrepOC();
  return 1;
}

