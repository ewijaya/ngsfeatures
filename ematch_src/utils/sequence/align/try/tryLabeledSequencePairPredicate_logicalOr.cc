/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.13
 *  Last Modified: $Date: 2009/06/15 14:00:17 $
 *
 *  Purpose: try code involving LabeledSequencePairPredicate_logicalOr
 *
 */
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "utils/sequence/LabeledSequencePairPredicate_logicalOr.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
static std::istream* arg_fastaIstreamPtr;


namespace cbrc{

  void tryLabeledSequencePairPredicate_logicalOr(){

    
    /* ***** Set up LabeledSequencePairPredicate to filter sequence pairs ***** */
    std::vector<LabeledSequencePairPredicate*> predicatePtrVec;

    predicatePtrVec.push_back( (LabeledSequencePairPredicate*) &labeledSequencePairPredicate_namesEqual );

    const LabeledSequencePairPredicate_2ndSeqClassFilter mitoFilter( "mito" );

    predicatePtrVec.push_back( (LabeledSequencePairPredicate*) &mitoFilter );

    
    /* ***** Read in Sequences ***** */
    FastaSeqSlurper seqSlurper( *arg_fastaIstreamPtr );
    
    const std::vector<LabeledSequence> seqs  =  seqSlurper.slurpLabeledSequences();

    /* ***** Construct and try out LabeledSequencePairPredicate_logicalOr ***** */
    LabeledSequencePairPredicate_logicalOr eliminationFilter( predicatePtrVec );

    BOOST_FOREACH( const LabeledSequence& seq0, seqs ){
      BOOST_FOREACH( const LabeledSequence& seq1, seqs ){
	std::cout  <<  (  eliminationFilter( seq0, seq1 )  ?  "fail"  :  "pass"  )
		   <<  "  "
		   <<  seq0.name()  <<  "\t"  <<  seq1.name()  <<  std::endl;
      }
    }

  }

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "[fastaFile]" );

  argvP.setDoc( "-man|--man|-help|--help",
		"\
Prints sequence pairs in fastaFile which pass the following\n\
Two criteria.\n\
\n\
    1) The sequences have different names\n\
    2) The class name of the first sequence is not \"mito\"\n"
		);

  argvP.printDoc();

  arg_fastaIstreamPtr  =  argvP.getIstreamPtr( 1 );
  
  argvP.dieIfUnusedArgs();
  cbrc::tryLabeledSequencePairPredicate_logicalOr();
  return 1;
}

