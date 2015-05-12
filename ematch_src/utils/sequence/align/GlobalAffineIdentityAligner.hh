/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2008/06/25 11:00:41 $
 *
 *  Description: Class for computing global affine gap alignment of protein or
 *               nucleic acid sequences.
 *
 *               This implementation also contains a base sequence to which
 *               other sequences are aligned to.
 *
 */
#ifndef GLOBALAFFINEIDENTITYALIGNER_HH
#define GLOBALAFFINEIDENTITYALIGNER_HH
#include "utils/sequence/align/ResidueSimilarityAffineEdit.hh"
#include "utils/sequence/align/Alignment.hh"
#include "utils/sequence/LabeledSequence.hh"
#include "utils/FLArray/FLEArray.hh"
#include "utils/FLArray/FLEMatrix.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMap.hh"


namespace cbrc{

class GlobalAffineIdentityAligner{
public:
  GlobalAffineIdentityAligner( const ResidueSimilarityAffineEdit& residueSimilarityMeasure,
			       const ResidueIndexMap::arrayT&     seq0 ) :
    resSim( residueSimilarityMeasure ),
    s0    ( seq0                     )
    {
      dpTable.setSize( 2, s0.size()+1 );
      gpTable.setSize( 2, s0.size()+1 );
      _bestPossibleScore = singleSeqBasedScoreUpperBound( s0 );
    }

  /* ********** ACCESSORS ********** */
  const ResidueIndexMap::arrayT& residues() const{ return s0; }


  /* best score possible for sequence s0 with any other sequence
   * for reasonable amino acid similarity scores this is the same
   * as the alignment score of s0 with itself.
   */
  const alignScoreT& bestPossibleScore() const{ return _bestPossibleScore; }

  // return maximal possible alignment score with standard dynamic programming
  // in O(n^2) time, O(n) space
  alignScoreT score( const ResidueIndexMap::arrayT& s1 ) const;

  double identity( const ResidueIndexMap::arrayT& s1 ) const{
    return(  score(s1) / (double) max( s0.size(), s1.size() )  );
  }

  /* allowing one free initial gap in either sequence,
   * return maximal possible alignment score with standard dynamic programming
   * in O(n^2) time, O(n) space
   */
  alignScoreT scoreFreeStartGap( const ResidueIndexMap::arrayT& s1 );

  // return an optimal alignment, computed with standard dynamic programming
  // and backtracking in O(n^2) time and space.
  Alignment alignment( const ResidueIndexMap::arrayT& s1 );

  /* if (score >= lowerBound) return score. otherwise return retval < lowerBound.
   * compute with distance from diagonal bounded dynamic programming
   * in O(n^2) time and O(n) space. Depending on input parameters time may be
   * as fast as Ω(n) time.
   */
  alignScoreT scoreBounded( const ResidueIndexMap::arrayT& s1, 
			    const FLEArrayFast<alignScoreT>& s1BestPossibleSuffixScores, 
                            const alignScoreT lowerBound );

  // compute simple upper bound, primarily based on the sequence lengths
  // but also on score upper bound for each sequence considered independently
  alignScoreT lengthDifferenceBasedScoreUpperBound( const ResidueIndexMap::arrayT& s1 ) const;

  // upper bound based purely on different in length of seq0 and len1
  alignScoreT purelyLengthDifferenceBasedScoreUpperBound( const size_t& len1 ) const;

  alignScoreT feasibleBound( const ResidueIndexMap::arrayT& s1 );

  alignScoreT singleSeqBasedScoreUpperBound( const ResidueIndexMap::arrayT& s1 ) const{
    return(  s1.size() * resSim.maxScore()  );
  }

  FLEArray<alignScoreT> bestPossibleSuffixScore( const ResidueIndexMap::arrayT& s0 ) const{
    FLEArray<alignScoreT> suffixScores( s0.size() );
    bestPossibleSuffixScore( s0, suffixScores );
    return suffixScores;
  }

  void bestPossibleSuffixScore( const ResidueIndexMap::arrayT& s0,
				/***/ FLEArray<alignScoreT>& suffixScores ) const;
private:
  inline static alignScoreT max( const alignScoreT a, const alignScoreT b ){ return( (a > b) ? a : b ); }
  inline static alignScoreT max( const alignScoreT a, const alignScoreT b, const alignScoreT c ){
    if( a > b ) return( max( a, c ) );
    else        return( max( b, c ) );
  }

  // object data
  const ResidueSimilarityAffineEdit& resSim;
  const ResidueIndexMap::arrayT s0;
  FLEMatrixFast<alignScoreT> dpTable;
  FLEMatrixFast<alignScoreT> gpTable;
  alignScoreT _bestPossibleScore;
};


} // end namespace
#endif // defined GLOBALAFFINEIDENTITYALIGNER_HH

