/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.4.27
 *  Last Modified: $Date: 2009/05/16 22:33:24 $
 *
 *  Description: 
 *
 *  Purpose: Created for
 */
#include <iostream>
#include "fastafmtConst.hh"
#include "fastafmtFunctions.hh"



namespace cbrc{



size_t fastafmtFunctions::countRecords( std::istream& iStream ){

  std::string lineBuffer;
  size_t numRecords = 0;

  while( std::getline( iStream, lineBuffer ) ){
      if( lineBuffer.empty() )  continue;
      if( lineBuffer[0] == FASTAFMT_RECORD_MARKER_CHAR )  ++numRecords;
  }

  return numRecords;
}


  
size_t fastafmtFunctions::skipRecords( std::istream& iStream, const size_t& numToSkip ){

  if( numToSkip == 0 )  return 0;

  if( iStream.peek() == std::istream::traits_type::eof() ){
    return 0;
  }


  std::string lineBuffer;

  size_t numRecordsStarted  =  ( iStream.peek() == FASTAFMT_RECORD_MARKER_CHAR );

  while(  std::getline( iStream, lineBuffer )   ){
    if( iStream.peek() == FASTAFMT_RECORD_MARKER_CHAR ){
      if( numRecordsStarted >= numToSkip ){
	return numRecordsStarted;
      }
      ++numRecordsStarted;
    }
  }

  return numRecordsStarted;

} // end fastafmtFunctions::skipRecords

    
    
size_t fastafmtFunctions::printRecords(  std::istream& iStream,
					 std::ostream& oStream  ){

  if( iStream.peek() == std::istream::traits_type::eof() ){
    return 0;
  }


  std::string curLine;

  size_t numRecordsStarted  =  ( iStream.peek() == FASTAFMT_RECORD_MARKER_CHAR );

  while(  std::getline( iStream, curLine )   ){
    oStream << curLine << std::endl;
    if( iStream.peek() == FASTAFMT_RECORD_MARKER_CHAR ){
      ++numRecordsStarted;
    }
  }

  return numRecordsStarted;

} // end printRecords


size_t fastafmtFunctions::printRecords(  /***/ std::istream&  iStream,
					 const size_t&        numToPrint,
					 /***/ std::ostream&  oStream
					 ){

  if( numToPrint == 0 )  return 0;
  
  if(  iStream.peek()  ==  std::istream::traits_type::eof()  ){
    return 0;
  }


  std::string curLine;

  size_t numRecordsStarted  =  ( iStream.peek() == FASTAFMT_RECORD_MARKER_CHAR );

  while(  std::getline( iStream, curLine )   ){
    oStream << curLine << std::endl;
    if( iStream.peek() == FASTAFMT_RECORD_MARKER_CHAR ){
      if( numRecordsStarted >= numToPrint ){
	return numRecordsStarted;
      }
      ++numRecordsStarted;
    }
  }

  return numRecordsStarted;

} // end printRecords



} // end namescape cbrc
