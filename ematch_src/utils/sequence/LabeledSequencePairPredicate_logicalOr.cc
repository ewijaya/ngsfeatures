/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.13
 *  Last Modified: $Date: 2009/06/21 00:06:18 $
 *  Description: See header file.
 */
#include "LabeledSequencePairPredicate_logicalOr.hh"

namespace cbrc{

bool LabeledSequencePairPredicate_logicalOr::
operator()( const LabeledSequence& seq0, const LabeledSequence& seq1 ) const{

  for( predicatePtrVecT::const_iterator itr = predicates().begin();
       itr != predicates().end();
       ++itr ){
    const LabeledSequencePairPredicate* const  curPredicatePtr  =  *itr;
    if(  curPredicatePtr->operator()( seq0, seq1 )  ){
      return true;
    }
  }

  return false;
}



void LabeledSequencePairPredicate_logicalOr::print() const{

  if( !predicates().size() ){
    std::cout  <<  "logicalOr_withEmptyPredicateList-->alwaysFalse";
  }

  for(  size_t i = 0;  i < predicates().size();  ++i  ){
    if( i )   std::cout  <<  ", ";
    ( *_predicates[i] ).print();
  }
} // end method print


} // end namespace cbrc

