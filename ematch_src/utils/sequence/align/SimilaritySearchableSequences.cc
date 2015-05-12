/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2004.12.8
 *  Last Modified: $Date: 2009/06/20 23:57:29 $
 *
 *  Description: See header files.
 */
#include <cmath>
#include <limits>
#include <boost/foreach.hpp>
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "SimilaritySearchableSequences.hh"

namespace cbrc{


std::vector<size_t>
SimilaritySearchableSequences::identicalMatches( const LabeledSequence& query ) const{
  std::vector<size_t> retVal;
  for( size_t i = 0; i < seqs.size(); ++i ){
    if( query.residueIndices() == seqs[i].residueIndices() )  retVal.push_back( i );
  }
  return retVal;
}


std::vector<size_t>
SimilaritySearchableSequences::withinHammingDistance( const LabeledSequence& query,
							const double&          threshold ) const{
  std::vector<size_t> retVal;

  for( size_t curSeqIdx = 0; curSeqIdx < seqs.size(); ++curSeqIdx ){
    if(  HammingDistanceComputer::computeBounded( seqs[curSeqIdx], query, threshold )
	 < threshold  ){
      // num mismatches < threshold so push current sequence onto return list
      retVal.push_back( curSeqIdx );
    }
  }

  return retVal;
}



bool
SimilaritySearchableSequences
::wouldSkipAll(  const LabeledSequence&               query,
		 const LabeledSequencePairPredicate&  skipPairPredicate  )  const
{

  BOOST_FOREACH(  const LabeledSequence&  seq,  seqs  ){

    if(  !skipPairPredicate( seq, query )  ){
      return false;
    }
  }

  return true;
}



SimilaritySearchableSequences::indexScorePairT
SimilaritySearchableSequences::
bestMatch( const LabeledSequence&                     query,
	   const alignScoreT                          loosenessFactor,
	   const LabeledSequencePairPredicate&        skipPairPredicate
	   ) const{

  GDB_ASSERTF( seqs.size(),  "Attempted to search against empty sequence set" );

  GDB_ASSERTF(  !wouldSkipAll( query, skipPairPredicate ),
		"For query %s, all sequence pairs would be skipped",
		query.name().c_str()  );

    
  const SeqGlobalAffineAligner gas( resSubScore(), query.residueIndices() );


  /* ***** Calculate quick lower bounds and compute order in which to compare seqs ***** */

  
  const alignScoreT  lowestAcceptableScore  =  
    loosenessFactor
    ?  gas.bestPossibleScore() - loosenessFactor * static_cast<alignScoreT>( sqrt( query.length()) )
    :  std::numeric_limits<alignScoreT>::min();


  alignScoreT  lowerBoundToUse  =  lowestAcceptableScore;

  // first get rough feasible bound for each sequence.
  alignScoreT bestLowerBound  =  std::numeric_limits<alignScoreT>::min();

  // Compute lower bounds for each sequence aligned to anything.
  for(  size_t i = 0;  i  <  seqs.size();  ++i  ){

    seqBounds[i]  =  gas.feasibleBound(  seqs[i].residueIndices()  );

    bestLowerBound  =  std::min( bestLowerBound, seqBounds[i] );
      
  }

  if(  lowerBoundToUse  <  bestLowerBound  ){
    lowerBoundToUse  =  bestLowerBound;
  }


  /* sort (indirectly) seqs by bound, this heuristically improves the chance of finding a match
   * early in the list.
   */
  const FLEArray<size_t>  seqsByBound  =  FLEArrayIdentityPermutation( seqs.size() );

  std::stable_sort( seqsByBound.begin(), seqsByBound.end(), 
		    FLEArrayByIndexSortingPredicate< alignScoreT, std::greater_equal<alignScoreT> >(seqBounds) );


  /* ***** Do the actual alignment calculations ***** */

  size_t  bestMatchIdx  =  seqs.size();  // initialize to invalid value.
  for(  size_t  permutationIndex  =  0;
	permutationIndex < seqsByBound.size();
	++permutationIndex ){

    const size_t  seqIdx  =  seqsByBound[permutationIndex];

    if(  skipPairPredicate( seqs[seqIdx], query )  ){
      continue;  // skip this pair of sequences
    }

    const alignScoreT scoreApprox
      = gas.scoreSuffixBounded( seqs[seqIdx].residueIndices(), bestLowerBound );

    if(  scoreApprox >  bestLowerBound  ){  // in this case scoreApprox = real score
      bestLowerBound  =  scoreApprox;
      bestMatchIdx    =  seqIdx;
    }
  } // next permutationIndex

  GDB_ASSERTF(  ( bestMatchIdx < seqs.size() ),
		"bestMatchIdx value (%zu) invalid. Perhaps never set?",
		bestMatchIdx  );

  indexScorePairT retVal;

  if(  bestLowerBound  >=  lowestAcceptableScore  ){  // bestLower is exact.
    retVal.first  = bestMatchIdx;
    retVal.second = bestLowerBound;
  }else{
    // bestLower is not exact and may not even be a valid lower bound.
    retVal.first = seqs.size(); // indicates invalid match
  }

  return retVal;

} // end SimilaritySearchableSequences::bestMatch( const LabeledSequence& query,...


    
}; // end namespace cbrc
