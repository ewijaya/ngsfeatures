/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/05/06 04:04:56 $
 *
 *  Description: Dump amino acid content of fastafmt amino acid sequences.
 *
 *  Purpose: Originally made for WoLF PSORT
 *
 */
#include <iostream>
#include <ctype.h>
#include <assert.h>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include "../FastaRecordReader.hh"


static std::istream*  arg_fastaIstreamPtr  =  &std::cin;


namespace cbrc{

  void dumpAminoAcidContentWPformat(){

    FastaRecordReader fr( *arg_fastaIstreamPtr );
    const ResidueIndexMap& rim = aminoResidueIndexMap;

    /* *** Print feature name line *** */
    std::cout << "id class ";

    for( std::string::const_iterator resPtr = rim.alphabetProper().begin(); 
	 resPtr != rim.alphabetProper().end();
	 ++resPtr ){
      std::cout << static_cast<char>(toupper( *resPtr )) << "cont ";
    }
    std::cout << "length " << std::endl;


    while(  const FastaRecord* const  curRecordPtr  =  fr.nextRecord()  ){

      /* Print id and sequence class */
      std::cout << curRecordPtr->name() << " ";

      const std::string  className  =  (   ( curRecordPtr->labels().size() > 1 )
					   ?  curRecordPtr->labels().at(1)  :  "unknown"   );

      std::cout << className << " ";

      // read sequence
      std::string seq = curRecordPtr->seq();

      // make sure case of all residue characters is consistent
      rim.canonicalizeCase( seq );


      /* *** print amino acid content of each amino acid */
      double invL = 1 / static_cast<double>( seq.length() );
      double checkSum = 0.0;
      for( std::string::const_iterator resPtr = rim.alphabetProper().begin(); 
	   resPtr != rim.alphabetProper().end();
	   ++resPtr ){
	std::cout << invL * std::count( seq.begin(), seq.end(), *resPtr ) << " ";
	checkSum +=  invL * std::count( seq.begin(), seq.end(), *resPtr );
      }
      assert( checkSum > 0.99 );
      assert( checkSum < 1.01 );

      std::cout << seq.length() << std::endl;
    }
  }
} // end namescape cbrc



int main( const int argc, const char* argv[] ){

  cbrc::ArgvParser argvP( argc, argv,  "[fastaFile]" );

  argvP.set( arg_fastaIstreamPtr, 1 );
  
  argvP.dieIfUnusedArgs();

  cbrc::dumpAminoAcidContentWPformat();

  return 1;
}

