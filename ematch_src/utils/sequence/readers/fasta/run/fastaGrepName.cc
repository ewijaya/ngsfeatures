/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.18
 *  Last Modified: $Date: 2009/06/02 08:48:37 $
 *
 *  Description: Grep fastamt records by record name.
 *
 *  Purpose: Created while updated WoLF PSORT dataset.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FastaRecordReader.hh"


struct argT{
  boost::regex*  nameRegexPtr;
  std::istream* fastaIstreamPtr;
} args;


namespace cbrc{
  void fastaGrepName(){
    FastaRecordReader fr( *args.fastaIstreamPtr );

    while(   const FastaRecord* const  curRecordPtr  =  fr.nextRecord()   ){
      if(  boost::regex_match( curRecordPtr->name(), *args.nameRegexPtr )  ){
	std::cout  <<  *curRecordPtr;
      }
    }
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "nameRegex [fastaFile]" );

  std::string nameRegexString;
  argP.setOrDie( nameRegexString, 1 );

  args.nameRegexPtr = new boost::regex( nameRegexString );

  args.fastaIstreamPtr = argP.getIstreamPtr( 2 );

  argP.dieIfUnusedArgs();

  cbrc::fastaGrepName();

  return 1;
}

