/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.20
 *  Last Modified: $Date: 2009/05/21 10:55:12 $
 *
 *  Description: Class for outputting (multi)fasta format files
 *
 */
#ifndef _FASTAWRITER_HH
#define _FASTAWRITER_HH
#include "fastafmt.hh"
#include "utils/sequence/LabeledAsciiSeq.hh"
#include <iostream>

namespace cbrc{

class FastaWriter{
public:
  FastaWriter()
  :
    _lineLengthForPrintingSeq(  defaultLineLengthForPrintingSeq()  ),
    _addAsteriskAtEndOfSeq   (  defaultAddAsteriskAtEndOfSeq()     ),
    _stringBeforeName        (  defaultStringBeforeName()          ),
    _delimiter               (  defaultDelimiter()                 )
  {}

  // print as fasta format text
  void print( const LabeledAsciiSeq& labeledAsciiSeq,
	      /***/ std::ostream& os = std::cout );


  /* ***** Parameter default values ***** */
  const static size_t& defaultLineLengthForPrintingSeq(){
    const static size_t retVal = 60;
    return retVal;
  }

  const static bool&         defaultAddAsteriskAtEndOfSeq(){
    const static bool retVal = false;
    return retVal;
  }

  const static std::string&  defaultStringBeforeName(){
    const static std::string retVal( " " );
    return retVal;
  }

  const static std::string&  defaultDelimiter(){
    const static std::string retVal( "\t" );
    return retVal;
  }


  /* ***** ACCESSORS ***** */
  const size_t& lineLengthForPrintingSeq() const{
    return _lineLengthForPrintingSeq;
  }

  const bool&   addAsteriskAtEndOfSeq() const{
    return _addAsteriskAtEndOfSeq;
  }

  const std::string& stringBeforeName() const{
    return _stringBeforeName;
  }

  const std::string delimiter() const{
    return _delimiter;
  }
private:
  size_t      _lineLengthForPrintingSeq;
  bool        _addAsteriskAtEndOfSeq;
  std::string _stringBeforeName;
  std::string _delimiter;
};

} // end namespace cbrc
#endif // _FASTAWRITER_HH
