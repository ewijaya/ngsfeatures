/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.7
 *  Last Modified: $Date: 2009/05/21 10:55:25 $
 *  Description: See header file.
 */
#include "FastaWriter.hh"

namespace cbrc{

void FastaWriter::print( const LabeledAsciiSeq& labeledAsciiSeq,
			 /***/ std::ostream&    os ){

  /* ***** Print Record Head ***** */
  os << fastafmt::recordStartMarker() << stringBeforeName();

  if( labeledAsciiSeq.numLabels() ){
    os << labeledAsciiSeq.labels().at(0);
  }
  for( size_t i = 1; i < labeledAsciiSeq.numLabels(); ++i ){
    os << delimiter() << labeledAsciiSeq.labels().at(i);
  }

  os << std::endl;

  
  { /* ***** Print Sequence ***** */
    const std::string& seq = labeledAsciiSeq.seq();
  
    for( size_t i = 0; i < seq.size(); i += lineLengthForPrintingSeq() ){
      os << seq.substr( i, lineLengthForPrintingSeq() ) << std::endl;
    }
  }
}

} // end namespace cbrc

