/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/06/03 04:33:11 $
 *  Description: See header file.
 */
#include "FastaSeqSlurper.hh"

namespace cbrc{


std::vector<LabeledAsciiSeq> FastaSeqSlurper::slurpSeqs( std::istream& iStream ){

  std::vector<LabeledAsciiSeq> retVal;

  while(  nextRecord()  ){
    retVal.push_back( getLabeledAsciiSeq() );
  }

  return retVal;
}


} // end namespace cbrc
