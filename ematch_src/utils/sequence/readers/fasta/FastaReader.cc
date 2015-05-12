/* 
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2006, Paul B. Horton, All rights reserved.
 *  License: All rights reserved
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/04/30 21:43:48 $
 *  
 *  Description: See header files.
 */
#include "utils/perlish/perlish.hh"
#include "FastaReader.hh"
#include <iostream>

namespace cbrc{

bool FastaReader::readRecord( std::istream& is ){
  bool retVal = readRecordAux( is );
  if( retVal && seq().size() ){
    std::string& seqRef = _curRecord.seqRef();
    if( seqRef[ seq().size() - 1 ] == '*' ){
      seqRef.erase( seqRef.end()-1 ); // remove trailing '*'
    }
  }
  return retVal;
}



bool FastaReader::readRecordAux( std::istream& is ){

  _curRecord.setSeq( "" );

  /* ******* set record head line ******* */
  // if first record in stream, read in record head line
  if( nextRecordHead.length() == 0 ){
    if(  is.eof()  )                                 return false;
    if(  !getNoncommentLine( is, curRecordHead )  )  return false;
  }
  // else use line already read.
  else{
    curRecordHead = nextRecordHead;
  }


  { /* ******* Compute labelLine and labels from record head line ******* */
    std::string labelLine = curRecordHead;

    if( labelLine[0] != '>' ){
      std::cerr << __FILE__ << ":" << __LINE__ 
		<< " Input Error: expected line to start with \">\"\n line: " << labelLine << std::endl;
      return false;
    }

    labelLine.erase(0,1); // removed leading ">"

    while( (labelLine[0] == '\t') || (labelLine[0] == ' ') ){
      labelLine.erase(0,1); // remove any white space characters before ID
    } 

    _curRecord.setLabels(   cbrc::perlish::split( delimiterPattern(), labelLine  )   );
  } // end Compute labelLine and labels from record head line


  /* ******* Read (one past) body of record ******* */
  for( std::string curLine;
       getNoncommentLine( is, curLine );
       /* noop */ ){

    // return if first line of next record is encountered
    if( curLine[0] == '>' ){
      nextRecordHead = curLine;
      return true;
    }

    // erase spaces from line
    curLine.erase( 
		  std::remove(  curLine.begin(), curLine.end(), ' '  ),
		  curLine.end()
		  );

    _curRecord.seqRef() += curLine;
  }


  nextRecordHead = "";   // at end of file so there is no 1st line of next record.
  return true;
}



// return true iff start of LINE matches commentMarker(), and commentMarker is not empty
bool FastaReader::isComment( const std::string& line ) const{
  if( commentMarker().empty() ) return false;
  for( size_t i = 0;
       (i < commentMarker().size()) && (i < line.size());
       ++i ){
    if( commentMarker()[i] != line[i] )  return false;
  }
  return true;
}



bool FastaReader::delimiterPatternSupported( const std::string& delimiterPattern ){
  if(  delimiterPattern == std::string( "[ \t]+" )  )  return true;
  if(  delimiterPattern == std::string( "\t+"    )  )  return true;
  if(  delimiterPattern == std::string( " +"     )  )  return true;
  return false;
}



bool FastaReader::getNoncommentLine( std::istream& is, std::string& line ) const{
  if( is.eof() )  return false;
  while(   std::getline( is, line )   ) {
    if( !isComment(line) )  return true;
  }
  return false;
}



void FastaReader::upcaseSequence(){
  std::string& seqRef = _curRecord.seqRef();
  for( std::string::iterator i = seqRef.begin(); i != seqRef.end(); ++i ){
    *i = toupper( *i );
  }
}



std::ostream& operator<<( std::ostream& os, const FastaReader& fr ){

  os << fr.curHead() << std::endl;

  const std::string& seq = fr.seq();

  for( size_t i = 0; i < seq.size(); i += fr.lineLengthForPrintingSequence() ){
    size_t lineLen = seq.size() - i;
    if( lineLen > fr.lineLengthForPrintingSequence() ){
      lineLen = fr.lineLengthForPrintingSequence();
    }
    os << seq.substr( i, lineLen ) << std::endl;
  }
  return os;
}
    

  
}; // end namespace
