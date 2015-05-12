/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 09:07:44 $
 *
 *  Description: Class to hold contents of a fasta format sequence record.
 *
 *               The classical definition is slightly expanded to allow
 *               an array of strings associated with the sequence instead
 *               of just an name (id)
 *
 *               This class should be decoupled from the I/O format
 *               See Also: FastafmtReader, FastafmtWriter
 */
#ifndef _FASTAFMTRECORD_HH
#define _FASTAFMTRECORD_HH
#include <iostream>
#include <cassert>

namespace cbrc{

class FastafmtRecord{
public:
  // constructors
  FastafmtRecord() :
    _seq( nullSeq() )
  {}

  FastafmtRecord( const std::string& seq, const std::string name = defaultName() )
    : _seq(seq){
    _labels.push_back( name );
  }

  FastafmtRecord( const std::string& seq, std::vector< std::string > labels )
    : _seq(seq){
    _labels.assign( labels.begin(), labels.end() );
  }
      

  // accessors
  const std::string& seq()    const{ return _seq; }
  std::string&       seqRef()      { return _seq; } // caution allows sequence to be changed from outside!

  void setSeq( const std::string& seq ){ _seq = seq; }

  const std::string& name() const{ return label(0); }
  const std::string& label( const size_t& i ) const{
    return labels().at(i);
  }

  const std::vector< std::string >& labels() const{
    return _labels;
  }

  void setLabels( const std::vector< std::string >&  labels ){
    _labels.assign( labels.begin(), labels.end() );
  }
  
  void setLabel( const size_t& i, const std::string& label ){
    assert( labels().size() > i );
    _labels[i] = label;
  }


  // class constants
  static const std::string& defaultName(){
    static const std::string _defaultName( "unnamed" );
    return _defaultName;
  }

  // nullSeq used to denote uninitialized records
  static const std::string& nullSeq(){
    static const std::string _nullSeq( "" );
    return _nullSeq;
  }

private:
  // object data.
  std::string _seq;
  std::vector< std::string > _labels;
};


// this output function is for debugging. Use FastafmtWriter to output in fastafmt format
inline std::ostream& operator<<( std::ostream& os, const FastafmtRecord& fastafmtRecord ){

  os << "fastafmtRecord:" << std::endl;

  // print labels
  for( size_t i = 0; i < fastafmtRecord.labels().size(); ++i ){
    os << "\t";
    os << "|" << fastafmtRecord.label(i) << "|";
  }
  os << std::endl;

  // print seq
  os << "\tseq: " << fastafmtRecord.seq() << std::endl;
  
  return os;
}

} // end namespace cbrc
#endif // _FASTAFMTRECORD_HH
