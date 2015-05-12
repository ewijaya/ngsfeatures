/* 
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2006, Paul B. Horton, All rights reserved.
 *  License: All rights reserved
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/09/01 01:21:32 $
 *  
 *  Description: See header files.
 */
#include <iostream>
#include "boost/foreach.hpp"
#include "utils/gdb/gdbUtils.hh"
#include "utils/stl/stlTypes.hh"
#include "FastaRecordReader.hh"

namespace cbrc{



/* ********** PUBLIC METHODS ********** */

// return pointer to next method in stream, or pointer to NULL if no more records remaining.
const FastaRecord*  FastaRecordReader::nextRecord(){
  
  return nextRecord( defaultIstream() );
}


// return pointer to next method or die with error based on errorMessage
const FastaRecord*
FastaRecordReader::nextRecordOrDie(  const char* const  errorMessage  ){
  return nextRecordOrDie(  defaultIstream(),  errorMessage  );
}


const FastaRecord*
nextRecordOrDie(  /***/ std::istream& iStream,
		  const char* const   errorMessage  );



const FastaRecord*
FastaRecordReader::nextRecordOrDie(
				   /***/ std::istream& iStream,
				   const char* const   errorMessage
				   ){

  const FastaRecord* const  fastaRecordPtr  =  nextRecord( iStream );

  if( fastaRecordPtr )  return fastaRecordPtr;


  /* ***** else output error message and exit ***** */

  std::string message( errorMessage );

  /* ** build error message, if none given ** */
  if(  !message.size()  ){
    message  = 	"Error: in FastaRecordReader; failed when attempting to read fasta record";

    if(  iStream.eof()  ==   std::istream::traits_type::eof()  )
      message += " perhaps stream was already at EOF? ";

    if(  (void*) iStream  ==  (void*) std::cin  )
      message += " (while reading from std::cin)";
  }

  GDB_DIE( message );

  return &_curRecord;
}

					  
  
  
  /* read next record from iStream and assign contents to _curRecord
   * if successful, return pointer to curRecord, otherwise return NULL
   */
const FastaRecord*
FastaRecordReader::nextRecord( std::istream& iStream ){

  _curIstreamPtr = &iStream;  // copy to global variable for convenience.

  _curRecord.clear();

  if( !readRecordHead() )  return NULL; // EXIT: no more records.

  GDB_ASSERTF( curRecord().headLine().size(), "Program Error: unexpected empty head line" );

  computeLabels();


  /* ******* Read body of record ******* */
  while( curLine_readToNextBodyNonCommentLine() ){

    // append sequence characters from this line onto sequence.
    std::remove_copy_if(
			curLine().begin(), curLine().end(),
			back_inserter( _curRecord._seq ),
			std::not1( std::ptr_fun<int,int>(inputOptions().seqCharFilter()) )
			);
  }


  // remove trailing '*' from seq if present
  if(   seq().at(  seq().size() - 1  )   ==   fastafmt::seqTerminationMarker()   ){
    _curRecord._seq.resize(  seq().size() - 1  ); // remove it
  }

  return &_curRecord;

}



/* ********** PRIVATE METHODS ********** */

/* attempt to read head of next record in curIstream
 * return false if end of stream encountered
 *
 * comment lines encountered before the record
 * start line are stored in _commentLinesBeforeHead;
 *
 * head line is stored in _headLineVerbatim
 * 
 * Execution aborts with error, if non-comment line
 * encountered before next record start.
 */
bool FastaRecordReader::readRecordHead(){

  // read lines until EOF or marker start character seen
  while( curLine_readNonHeadLine() ){
    if(  curLine_isComment()  ){
      _curRecord._commentLinesBeforeHead.push_back( curLine() );
    }
    GDB_DIEF( "Could not parse line: \"%s\"", curLine().c_str() );
  }

  if( curIstream().eof() )  return false;  // EXIT: no more records in stream.

  std::getline( curIstream(), _curLine );

  _curRecord._headLine = curLine();


  return true;  // EXIT: record read.

} // end readRecordHead( std::istream& )



// compute curRecord().labels from head by splitting on delimiterRegex
void FastaRecordReader::computeLabels(){
  
  GDB_ASSERTF( !curRecord().labels().size(), "Programming error: expected labels empty" );

  boost::smatch recordStartMatch;

  DO_OR_DIEF(   boost::regex_search(  curRecord().headLine(),
				      recordStartMatch,
				      fastafmt::recordStartRegex()  ),
		"expected record start marker"   );


  _curRecord._labels =
    perlish::split(  inputOptions().betweenLabelsDelimiterRegex(),
		     curRecord().headLine().begin() + recordStartMatch.length(),
		     curRecord().headLine().end()  );

  if(  !curRecord().labels().size()  ){
    _curRecord._labels.push_back( defaultName() );
  }


} // end computeLabels()



bool FastaRecordReader::curLine_readToNextBodyNonCommentLine(){

  while( curLine_readNonHeadLine() ){
    _curRecord._bodyLines.push_back( curLine() );

    if( !curLine_isComment() )  return true;  // EXIT.
  }

  return false; // EXIT, end of record reached before non-comment line.
}



// if non-head line follows current position in curIstream,
// assign it to global _curLine
// otherwise do nothing and return false.
bool FastaRecordReader::curLine_readNonHeadLine(){

  if(  curIstream().eof() ||
       (curIstream().peek() == fastafmt::recordStartMarker())  ){
    return false; // EXIT, at end of record.
  }

  std::getline( curIstream(), _curLine );

  if( curIstream().fail() ){
    return false;  // necessary because getline() reads one empty line before generated an EOF
  }

  return true;
}


  
} // end namespace cbrc
