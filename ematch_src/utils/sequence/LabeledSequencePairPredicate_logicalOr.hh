/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.13
 *  Last Modified: $Date: 2009/06/21 00:04:41 $
 *
 *  Description: Computes short-circuit logical or for
 *               a vector of LabeledSequencePairPredicate's
 *
 */
#ifndef LABELEDSEQUENCEPAIRPREDICATE_LOGICALOR_HH_
#define LABELEDSEQUENCEPAIRPREDICATE_LOGICALOR_HH_
#include <boost/foreach.hpp>
#include "LabeledSequencePairPredicate.hh"


namespace cbrc{

class LabeledSequencePairPredicate_logicalOr : public LabeledSequencePairPredicate{
public:
  typedef std::vector<LabeledSequencePairPredicate*>  predicatePtrVecT;

  /* ********** CONSTRUCTORS ********** */
  LabeledSequencePairPredicate_logicalOr(  const predicatePtrVecT&  predicates  )
    :  _predicates( predicates )
  {}

  // return true iff: for some predicate $p in predicates(),  $p( seq0, seq1 ) returns true.
  bool operator()( const LabeledSequence& seq0, const LabeledSequence& seq1 ) const;

  void print() const;

  /* ********** ACCESSORS ********** */
  const predicatePtrVecT&  predicates() const  {  return _predicates;  }

private:
  // object data
  const predicatePtrVecT  _predicates;
};



} // end namespace cbrc
#endif // LABELEDSEQUENCEPAIRPREDICATE_LOGICALOR_HH_
