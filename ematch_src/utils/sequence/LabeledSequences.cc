/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.1.6
 *  Last Modified: $Date: 2008/06/18 01:51:11 $
 *
 *  Description: See header file.
 */
#include "LabeledSequences.hh"


namespace cbrc{
  LabeledSequences::LabeledSequences( const std::vector<LabeledSequence>& seqVector )
  : seqs(seqVector){

    // Place sequence names in idToIndex hash
    for( size_t i = 0; i < seqs.size(); ++i ){
      idToIndex[ seqs[i].name() ] = i;
    }

  }
}; // end namespace cbrc
