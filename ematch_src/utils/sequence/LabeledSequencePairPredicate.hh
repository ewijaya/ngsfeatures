/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2004.4.30
 *  Last Modified: $Date: 2009/06/21 00:03:36 $
 *
 *  Description: Some predicates involving pairs of LabeledSequence objects.
 *
 */

#include "utils/sequence/LabeledSequence.hh"

#ifndef _LABELEDSEQUENCEPAIRPREDICATE_HH_
#define _LABELEDSEQUENCEPAIRPREDICATE_HH_

namespace cbrc{

/* ***** Define General Interface ***** */
class LabeledSequencePairPredicate{
public:
  virtual bool operator()( const LabeledSequence& ls1, const LabeledSequence& ls2 ) const = 0;
  virtual void print() const = 0;  //  print self to std::cout
  virtual ~LabeledSequencePairPredicate(){}
};


/* ********** Define Specific Predictes ********** */
class LabeledSequencePairPredicate_alwaysFalse : public LabeledSequencePairPredicate{
public:
  bool operator()( const LabeledSequence& labSeq0, const LabeledSequence& labSeq1 ) const{
    return false;
  }
  void print() const {  std::cout  <<  "alwaysFalse";  }
};



class LabeledSequencePairPredicate_namesEqual : public LabeledSequencePairPredicate{
public:
  bool operator()( const LabeledSequence& labSeq0, const LabeledSequence& labSeq1 ) const{
    return( labSeq0.name() == labSeq1.name() );
  }
  void print() const {  std::cout  <<  "namesEqual";  }
};



/* ***** Define State-full Functors ***** */
class LabeledSequencePairPredicate_1stSeqClassFilter : public LabeledSequencePairPredicate{
public:
  LabeledSequencePairPredicate_1stSeqClassFilter(
      const LabeledSequencePairPredicate_1stSeqClassFilter& orig )
    : _className( orig.className() )
  {}

  LabeledSequencePairPredicate_1stSeqClassFilter(  const std::string&  className  )
    : _className(className)
  {}

  bool operator()( const LabeledSequence& labSeq0, const LabeledSequence& labSeq1 ) const{
    return( labSeq0.className() == className() );
  }

  void print() const {  std::cout  <<  "true_if_1stClass=="  <<  className();  }

  /* ***** Accessors ***** */
  const std::string& className() const{  return _className;  }

  void setClassName( const std::string& className ){ _className = className; }
private:
  // object data
  std::string  _className;
}; // end class LabeledSequencePairPredicate_1stSeqClassFilter.



/* ***** Declare Specific Predicates ***** */
const LabeledSequencePairPredicate_alwaysFalse labeledSequencePairPredicate_alwaysFalse;
const LabeledSequencePairPredicate_namesEqual  labeledSequencePairPredicate_namesEqual;

      
}; // end namespace cbrc
#endif // defined _LABELEDSEQUENCEPAIRPREDICATE_HH_

