/* 
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2006, Paul B. Horton, All rights reserved.
 *  License: All rights reserved
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/05/09 10:29:53 $
 *  
 *  Description: See header files.
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "utils/boost/boostUtils.hh"
#include "FastafmtReader.hh"

namespace cbrc{

bool FastafmtReader::readRecord( std::istream& is ){
  bool retVal = readRecordAux( is );
  if( retVal && seq().size() ){
    std::string& seqRef = _curRecord.seqRef();
    if( seqRef[ seq().size() - 1 ] == FastafmtConstants::seqTerminationMarker() ){
      seqRef.erase( seqRef.end()-1 ); // remove trailing '*'
    }
  }
  return retVal;
}



bool FastafmtReader::readRecordAux( std::istream& is ){

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

    if( labelLine[0] != FastafmtConstants::recordMarker() ){
      std::cerr << __FILE__ << ":" << __LINE__ 
		<< " Input Error: expected line to start with "
		<< FastafmtConstants::recordMarker() << std::endl
		<< "line: " << labelLine << std::endl;
      return false;
    }

    std::string::const_iterator curPos = labelLine.begin() + 1;

    assert(
	   advancePastMatchAtPoint( curPos,
				    labelLine.end(),
				    fastafmtInputOptions().beforeNameDelimiterRegex()
				    )
	   );

    _curRecord.setLabels(
			 cbrc::perlish::split(  fastafmtInputOptions().betweenLabelsDelimiterRegex(),
						curPos,
						std::string::const_iterator(labelLine.end())  )
			 );

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


// std::vector<LabeledAsciiSeq> FastafmtReader::slurpSeqs( std::istream& is ){
							       
//   std::vector<LabeledAsciiSeq> labeledAsciiSeqs;
//   while(  readRecord( is )  )  labeledAsciiSeqs.push_back( getLabeledAsciiSeq() );
//   return labeledAsciiSeqs;
// }


// std::vector<LabeledSequence> FastafmtReader::slurpLabeledSequences( std::istream& is,
// 								 const seqTypes::seqType& seqT ){
//   std::vector<LabeledSequence> labSeqs;
//   while(  readRecord( is )  )  labSeqs.push_back( getLabeledSequence(seqT) );
//   return labSeqs;
// }


// return true iff start of LINE matches commentMarker(), and commentMarker is not empty
bool FastafmtReader::isComment( const std::string& line ) const{
  if( commentMarker().empty() ) return false;
  for( size_t i = 0;
       (i < commentMarker().size()) && (i < line.size());
       ++i ){
    if( commentMarker()[i] != line[i] )  return false;
  }
  return true;
}


bool FastafmtReader::getNoncommentLine( std::istream& is, std::string& line ) const{
  if( is.eof() )  return false;
  while(   std::getline( is, line )   ) {
    if( !isComment(line) )  return true;
  }
  return false;
}



void FastafmtReader::upcaseSequence(){
  std::string& seqRef = _curRecord.seqRef();
  for( std::string::iterator i = seqRef.begin(); i != seqRef.end(); ++i ){
    *i = toupper( *i );
  }
}



std::ostream& operator<<( std::ostream& os, const FastafmtReader& fr ){

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
