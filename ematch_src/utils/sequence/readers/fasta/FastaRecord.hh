/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2009/05/22 12:22:16 $
 *
 *  Description: Class to hold contents of a fasta sequence record.
 *
 *               The classical definition is slightly expanded to allow
 *               an array of strings associated with the sequence instead
 *               of just an name (id)
 *
 *               This class should be decoupled from the I/O format
 *               See Also: FastaReader, FastaWriter
 */
#ifndef _FASTARECORD_HH
#define _FASTARECORD_HH
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

namespace cbrc{


class FastaRecord{
public:

  /* ********** TYPEDEFS ********** */
  typedef  std::vector<std::string>  stringVecT;


  /* ********** CONSTRUCTORS ********** */

  FastaRecord() :
    _seq( nullString() )
  {}

  FastaRecord( const std::string& seq, const std::string name = defaultName() )
    : _seq(seq){
    _labels.push_back( name );
  }

  FastaRecord( const std::string& seq, std::vector< std::string > labels )
    : _seq(seq){
    _labels.assign( labels.begin(), labels.end() );
  }

      

  /* ********** ACCESSORS ********** */
  const std::string&  seq() const   {  return _seq;  }

  // caution allows sequence to be changed from outside this class!
  /***/ std::string&  seqRef()      {  return _seq;  }

  void setSeq( const std::string& seq )  {  _seq = seq;  }

  const std::string& name() const  {  return label(0);  }

  const std::string& label( const size_t& i ) const  {  return labels().at(i);  }

  const stringVecT& labels() const  {  return _labels;  }

  void setLabels( const stringVecT&  labels ){
    _labels.assign( labels.begin(), labels.end() );
  }
  
  void setLabel( const size_t& i, const std::string& label ){
    assert( labels().size() > i );
    _labels[i] = label;
  }

  const stringVecT&   commentLinesBeforeHead() const  {  return _commentLinesBeforeHead;  }
  const std::string&  headLine()               const  {  return _headLine;                }
  const stringVecT&   bodyLines()              const  {  return _bodyLines;               }


  /* ********** CLASS CONSTANTS ********** */
  static const std::string& defaultName(){
    static const std::string _defaultName( "unnamed" );
    return _defaultName;
  }

  // nullSeq used to denote uninitialized records
  static const std::string& nullString(){
    static const std::string _nullString( "" );
    return _nullString;
  }


  /* ********** OTHER METHODS ********** */

  // clear record contents. Frees memory
  void clear();

  friend class FastaRecordReader;

private:
  // object data.
  std::string                  _seq;
  std::vector< std::string >   _labels;
  std::vector< std::string >  _commentLinesBeforeHead;
  std::string                 _headLine;
  std::vector< std::string >  _bodyLines;

};


std::ostream& operator<<( std::ostream& os, const FastaRecord& fastaRecord );



} // end namespace cbrc

#endif // _FASTARECORD_HH
