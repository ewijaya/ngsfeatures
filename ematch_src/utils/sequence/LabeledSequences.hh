/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005,2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.1.6
 *  Last Modified: $Date: 2008/06/18 01:54:56 $
 *
 *  Description: Container for a collection of LabeledSequence's
 *
 *  Purpose: Originally created for adding alignments of the query to neighbors in WoLFPSORT
 *
 */

#ifndef LABELEDSEQUENCES_HH_
#define LABELEDSEQUENCES_HH_
#include "utils/ChainHashtable/StringKeyHashtable.hh"
#include "utils/sequence/LabeledSequence.hh"

namespace cbrc{

class LabeledSequences{
public:
  /* ***** CONSTRUCTORS ***** */
  LabeledSequences( const std::vector<LabeledSequence>& seqVector );

  /* ***** ACCESSORS ***** */
  bool has( const std::string& id ) const{ return idToIndex.has( id ); }

  const LabeledSequence& operator()( const size_t& i ) const{
    return seqs[i];
  }

  const LabeledSequence& operator()( const std::string& id ) const{
    return seqs[idToIndex(id)];
  }  
private:
  // object data
  StringKeyHashtable<size_t> idToIndex;
  std::vector<LabeledSequence> seqs;
};

}; // end namespace cbrc
#endif // defined LABELEDSEQUENCES_HH_

