/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2004.12.8
 *  Last Modified: $Date: 2009/06/21 02:41:03 $
 *
 *  Description: A collection of sequences which can be searched to find similar sequences
 *               to a query.
 *
 *  Purpose: Originally made for use with the WoLF PSORT web site.
 *
 */
#ifndef _SIMILARITYSEARCHABLESEQUENCES_HH_
#define _SIMILARITYSEARCHABLESEQUENCES_HH_

#include "utils/FLArray/FLEArray.hh"
#include "utils/sequence/LabeledSequencePairPredicate.hh"
#include "SeqGlobalAffineAligner.hh"
#include "HammingDistanceComputer.hh"

namespace cbrc{


class SimilaritySearchableSequences{
public:
  typedef std::pair<size_t, alignScoreT> indexScorePairT;

  /* ********** CONSTRUCTORS ********** */
  SimilaritySearchableSequences( const AminoScore& resSubScore,
				 const std::vector<LabeledSequence>& seqs )
    :  _resSubScore(resSubScore), seqs(seqs), seqBounds( seqs.size() )
  {}


  /* returns index of best match and score.
   * index == size() indicates no sequence matched within the looseness factor.
   * When comparing query to database sequence seq, pair is ignored if
   *   skipPairPredicatePtr->( seq, query ) returns true
   */
  indexScorePairT
  bestMatch( const LabeledSequence&               query,
	     const LabeledSequencePairPredicate&  skipPairPredicate
	     ) const {
    return(  bestMatch( query, defaultLoosenessFactor(), skipPairPredicate )  );
  }


  indexScorePairT
  bestMatch( const LabeledSequence& query,
	     const alignScoreT      loosenessFactor = defaultLoosenessFactor(),

	     const LabeledSequencePairPredicate&
	     /**/  skipPairPredicate  =  labeledSequencePairPredicate_alwaysFalse

	     ) const;


  std::vector<size_t> identicalMatches( const LabeledSequence& query ) const;

  // return sequences with hamming distance < THRESHOLD
  std::vector<size_t> withinHammingDistance( const LabeledSequence& query,
					     const double&          threshold ) const;


  // return true iff all comparisons between query and seqs would be skipped.
  bool  wouldSkipAll(  const LabeledSequence&               query,
		       const LabeledSequencePairPredicate&  skipPairPredicate  ) const;

  /* ********** ACCESSORS ********** */
  const LabeledSequence&  operator()( const size_t& index ) const{  return seqs.at( index );  }

  size_t  size() const{  return seqs.size();  }

  const std::string&  seqName( const indexScorePairT& indexScorePair ) const{
    return seqs[indexScorePair.first].name();
  }

  const AminoScore& resSubScore() const{
    return _resSubScore;
  }

  /* ***** Class constants ***** */

  // "0" is a special value, which means don't use heuristic score bound.
  static const alignScoreT& defaultLoosenessFactor(){
    static const alignScoreT  _defaultLoosenessFactor  =  0;
    return _defaultLoosenessFactor;
  }
private:
  // object data
  const AminoScore&  _resSubScore;

  const std::vector<LabeledSequence>&  seqs;

  // when aligning to some query, seqBounds[i] holds lower score bound align( query, seq[i] )
  FLEArray<alignScoreT>  seqBounds;

};


}; // end namespace cbrc
#endif // defined _SIMILARITYSEARCHABLESEQUENCES_HH_

