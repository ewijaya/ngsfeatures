/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.3
 *  Last Modified: $Date: 2008/11/19 10:19:29 $
 *  Description: See header file.
 */
#include "boost/foreach.hpp"
#include "utils/gdb/gdbUtils.hh"
#include "utils/perlish/perlish.hh"
#include "SprotReader.hh"

namespace cbrc{

bool SprotReader::readRecord( std::istream& is ){
  size_t seqLenGivenInRecord;

  // initialize to read new record.
  _istreamPtr = &is;
  clearRecord();

  /* ***** BEGIN: Only Section Exit Returning False ***** */
  if( instream().eof() )  return false;

  readLine(); // should be first line of record or EOF.

  if( instream().eof() )  return false;
  /* ***** END: Only Section Exit Returning False ***** */


  DO_OR_DIEF( !curLineIsEndMarker(),
	      "Unexpected record end line not in SQ section"
	      );

  for(;;){
    std::vector<std::string> lineFields
      = perlish::split( "[ \t]+", curLine() );

    /* ***** BEGIN: Switch on curLinePrefix ***** */
    if( curLinePrefix() == "ID" ){
      _id = lineFields.at(1);
    }
      
    else if( curLinePrefix() == "AC" ){
      _accession = lineFields.at(1);
      DO_OR_DIEF( _accession.at( _accession.size() - 1 ) == ';',
		   "expected to \';\' after accession" );
      perlish::chop( _accession );
    }

    else if( curLinePrefix() == "OC" ){
      _organismClassification += curLineBody();
    }

    else if( curLinePrefix() == "SQ" ){

      DO_OR_DIEF(  curLine().length() > 20,
		    "line to short. line was: \"%s\"", curLine().c_str() );

      DO_OR_DIEF(  curLine().substr( 5, 8 ) == "SEQUENCE",
		    "line is \"%s\"\n", curLine().c_str()  );

      seqLenGivenInRecord = atoi( 13 + curLine().c_str() );

      _sequence.resize( seqLenGivenInRecord );
	
      size_t resNum = 0;

      // read in sequence lines until end of record encountered.
      for(;;){
	readLine(); // sequence lines read here.

	DO_OR_DIEF( !instream().eof(),
		    "Encountered an unexpected EOF in sequence section of record." );


	/* ***** BEGIN: Only Exit Section Returning True ***** */
	if( curLineIsEndMarker() ){

	  DO_OR_DIEF( resNum == seqLenGivenInRecord,
		      " number of residues parsed = %zu, unequal to advertised sequence length = %zu",
		      resNum, seqLenGivenInRecord );

	  return true; // EXIT. end of record encountered.
	}
	/* ***** END: Only Exit Section Returning True ***** */


	DO_OR_DIEF( curLinePrefix() == "  ",
		     "Expected read line to look like an SQ body line,"\
		     " but it looks like:\"%s\"\n", curLine().c_str()
		     );

	// append residues from current line onto sequence
	BOOST_FOREACH( char lineChar, curLine() ){
	  if(  isalpha( lineChar )  )   _sequence[resNum++] = lineChar;
	}


      } // end while loop for reading sequence lines


      GDB_DIEF( "Did not expect this code to be executed" );
      
    } // end curLinePrefix() == SQ

    /* ***** END: Switch on curLinePrefix ***** */

    readLine(); // most lines read in here.
    
    DO_OR_DIEF( !instream().eof(),
		"Encountered an unexpected EOF in middle of record."
		);

    DO_OR_DIEF( !curLineIsEndMarker(),
		"Unexpected record end line not in SQ section"
		);
  } // end readLines outer loop.

  GDB_DIEF( "Did not expect this code to be executed" );

} // end SprotReader::readRecord( std::istream& is )



// read line from INSTREAM.
// if EOF encountered, return immediately, otherwise perform
// reality check on line and push it onto recordLines
void SprotReader::readLine(){
  std::string line;

  std::getline( instream(), line );

  if( instream().eof() )  return; // EXIT (1/2).

  DO_OR_DIEF( line.length() >= 2,
	      "Input file error, line shorter than 2\n" );

  _recordLines.push_back( line ); // this updates curLine() as well.

  // EXIT (2/2).
}



void SprotReader::clearRecord(){
  _recordLines.clear();
  _id = _accession = _sequence = _organismClassification = "";
}



std::ostream& operator<<( std::ostream& os, const SprotReader& sprotReader ){
    
  BOOST_FOREACH( std::string recordLine, sprotReader.recordLines() ){
    os << recordLine << std::endl;
  }

  return os;
}


} // end namespace cbrc
