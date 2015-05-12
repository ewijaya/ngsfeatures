/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, 2008 Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/09/01 01:20:54 $
 *
 *  Description: Class for reading fasta format sequence files.
 *
 *  Caveats: Comment lines are currently only partially supported.
 *           a commentMarker string (defaults to "#") can be
 *           supplied and any lines starting with that string are ignored.
 *
 *           In the future, I intend to save the comments in an array and
 *           provide accessor functions for them, similar to the Perl module
 *           I recently wrote. PH. 2007/01/30.
 *
 */

#ifndef _FASTAREADER_HH_
#define _FASTAREADER_HH_
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include "utils/stl/stringUtils.hh"
#include "utils/perlish/perlish.hh"
#include "fastafmt.hh"
#include "FastaRecord.hh"
#include "FastaInputOptions.hh"

namespace cbrc{

class FastaRecordReader {
public:

  /* ********** CONSTRUCTORS ********** */
  FastaRecordReader
  (
   /***/ std::istream&       defaultIstream  =  std::cin,
   const FastaInputOptions&  inputOptions    =  defaultFastaInputOptions
   ) :
    _defaultIstream( defaultIstream ),
    _inputOptions  ( inputOptions   )
  {}


  /* ********** METHODS WHICH READ FROM THE STREAM ********** */

  // return pointer to next record in stream, or pointer to NULL if no more records remaining.
  const FastaRecord*  nextRecord();
  const FastaRecord*  nextRecord(  std::istream& iStream  );

  // return pointer to next method or die with error based on errorMessage
  const FastaRecord*  nextRecordOrDie(  const char* const  errorMessage  =  ""  );

  const FastaRecord*
  nextRecordOrDie(  /***/ std::istream& iStream,
		    const char* const  errorMessage  =  ""  );



  /* ***************ACCESSORS *************** */
  const FastaInputOptions&  inputOptions() const{  return _inputOptions;  }

  const FastaRecord& curRecord() const  {  return _curRecord;  }

  /* ***** Accessors To Read Current Record Data Variables ***** */
  const std::string& seq() const  {  return curRecord().seq();  }

  /***/ std::string& seqRef()     {  return _curRecord._seq;    }

  /* ***** Accessors To Parameter Default Values ***** */
  // istream used when istream argument of readRecord and readRecordOrDie is omitted.
  std::istream& defaultIstream() const{  return _defaultIstream;  }

  // name returned when no name was given for the current record.
  const std::string& defaultName(){
    const static std::string _defaultName( "unknown" );
    return _defaultName;
  }


  /* *************** OTHER METHODS *************** */

  // free memory holding current record data.
  void freeCurRecord();


private:

  /* ***** Private Accessors ***** */
  const std::string& curLine() const{  return _curLine;  }

  std::istream& curIstream(){  return *_curIstreamPtr;  }

  
  // compute labels from head by splitting on delimiterRegex
  void computeLabels();

  // attempt to read head from curIstream
  // return false if eof encountered first
  bool readRecordHead();

  // if non-head line follows current position in curIstream,
  // assign it to global _curLine
  // otherwise do nothing and return false.
  bool curLine_readNonHeadLine();

  // read lines from curIstream until next non-comment body line
  // is read, or return false if no such line if found in current record.
  bool curLine_readToNextBodyNonCommentLine();

  // read lines from curIstream until non-comment line is reached
  // read lines are placed in _bodyLinesVerbatim
  // curLine is set to the non-comment line, or false is returned
  // is no more body lines are found in curIstream
  bool curLineReadNextBodyLine();

  // return true iff curLine is a comment line.
  bool curLine_isComment() const{
    return(  inputOptions().isComment( curLine() )  );
  }

  /* ******* object data ******* */
  // default istream
  std::istream& _defaultIstream;

  std::istream* _curIstreamPtr;

  const FastaInputOptions _inputOptions;

  /* ** data for current record. ** */
  std::string  _curLine; // most recently read line

  FastaRecord  _curRecord;
};



} // end namespace cbrc.
#endif // defined _FASTAREADER_HH_
