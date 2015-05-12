/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/06/20 23:58:52 $
 *
 *  Description: Class for computing global affine gap alignment of protein or
 *               nucleic acid sequences.
 *
 *               This implementation also contains a base sequence to which
 *               other sequences are aligned to.
 *
 */
#ifndef _SEQGLOBALAFFINEALIGNER_HH_
#define _SEQGLOBALAFFINEALIGNER_HH_
#include "utils/gdb/gdbUtils.hh"
#include "utils/sequence/align/AminoScore.hh"
#include "utils/sequence/align/Alignment.hh"
#include "utils/FLArray/FLEArray.hh"
#include "utils/FLArray/FLEMatrix.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMap.hh"

namespace cbrc{

class SeqGlobalAffineAligner{
public:
  SeqGlobalAffineAligner( const AminoScore& resSubScore, const ResidueIndexMap::arrayT& seq0 ) 
    : _resSubScore( resSubScore ), s0( seq0 )
  {
    GDB_ASSERT( _resSubScore.minScoreIsValid() );
    dpTable.setSize( 2, s0.size()+1 );
    gpTable.setSize( 2, s0.size()+1 );
    _bestPossibleScore  =  singleSeqBasedScoreUpperBound( resSubScore, s0 );
  }

  /* ********** ACCESSORS ********** */
  const ResidueIndexMap::arrayT& residues() const{ return s0; }

  const AminoScore&  resSubScore() const{  return _resSubScore;  }
  
  /* best score possible for sequence s0 with any other sequence
   * for reasonable amino acid similarity scores this is the same
   * as the alignment score of s0 with itself.
   */
  const alignScoreT& bestPossibleScore() const{ return _bestPossibleScore; }
   

  // return maximal possible alignment score with standard dynamic programming
  // in O(n^2) time, O(n) space
  alignScoreT score( const ResidueIndexMap::arrayT& s1 ) const;

  /* allowing one free initial gap in either sequence,
   * return maximal possible alignment score with standard dynamic programming
   * in O(n^2) time, O(n) space
   */
  alignScoreT scoreFreeStartGap( const ResidueIndexMap::arrayT& s1 ) const;

  // return an optimal alignment, computed with standard dynamic programming
  // and backtracking in O(n^2) time and space.
  Alignment alignment( const ResidueIndexMap::arrayT& s1 ) const;

  /* if (score >= lowerBound) return score. otherwise return retval < lowerBound.
   * compute with distance from diagonal bounded dynamic programming
   * in O(n^2) time and O(n) space. Depending on input parameters time may be
   * as fast as Ω(n) time.
   *
   * s1BestPossibleSuffixScores(i) should be an upper bound on the score of
   * aligning suffix s1[i..] to any sequence.
   */
  alignScoreT scoreBounded(  const ResidueIndexMap::arrayT&  s1, 
			     const FLEArray<alignScoreT>&    s1BestPossibleSuffixScores, 
			     const alignScoreT&              lowerBound )
    const;


  // call bestPossibleSuffixScore to compute s1BestPossibleSuffixScores,
  // then use those upper bounds to call scoreBounded and return result.
  alignScoreT scoreSuffixBounded(  const ResidueIndexMap::arrayT&  s1, 
			           const alignScoreT               lowerBound  )
    const;


  /* if no prefix of s1 aligns with optimal score worse than prefixScoreLowerBound,
   * return ordinary global alignment score, otherwise return alignScoreTMin
   * O(n^2) time, O(n) space, but may take less time when some prefix violates
   * the prefixScoreLowerBound constraint.
   */
  alignScoreT prefixScoreBounded( const ResidueIndexMap::arrayT& s1,
				  const alignScoreT& prefixScoreLowerBound ) const;

  // compute simple upper bound, primarily based on the sequence lengths
  // but also on score upper bound for each sequence considered independently
  alignScoreT lengthDifferenceBasedScoreUpperBound( const ResidueIndexMap::arrayT& s1 ) const;

  // upper bound based purely on different in length of seq0 and len1
  alignScoreT purelyLengthDifferenceBasedScoreUpperBound( const size_t& len1 ) const;

  // score of some feasible alignment computed in linear time.
  alignScoreT feasibleBound( const ResidueIndexMap::arrayT& s1 ) const;


  /* ********** CLASS METHODS ********** */
  static alignScoreT singleSeqBasedScoreUpperBound( const AminoScore& resSubScore,
						    const ResidueIndexMap::arrayT& s1 ){
    alignScoreT score = 0;
    for( size_t i = 0; i < s1.size(); ++i )  score += resSubScore.maxScore( s1[i] );
    return score;
  }


  static FLEArray<alignScoreT> bestPossibleSuffixScore( const AminoScore& resSubScore,
							const ResidueIndexMap::arrayT& seq ){
    FLEArray<alignScoreT> suffixScores( seq.size() );
    bestPossibleSuffixScore(  suffixScores, resSubScore, seq );
    return suffixScores;
  }

  static void bestPossibleSuffixScore( /***/ FLEArray<alignScoreT>&    suffixScores,
				       const AminoScore&               resSubScore,
				       const ResidueIndexMap::arrayT&  seq
				       );

private:
  inline static alignScoreT max( const alignScoreT& a,
				 const alignScoreT& b ){
    return( (a > b) ? a : b );
  }
  inline static alignScoreT max( const alignScoreT& a,
				 const alignScoreT& b,
				 const alignScoreT& c ){
    if( a > b ) return( max( a, c ) );
    else        return( max( b, c ) );
  }

  // object data
  const AminoScore&                _resSubScore;
  const ResidueIndexMap::arrayT    s0;
  /***/ FLEMatrixFast<alignScoreT> dpTable;
  /***/ FLEMatrixFast<alignScoreT> gpTable;
  /***/ alignScoreT                _bestPossibleScore;
};


} // end namespace
#endif // defined _SEQGLOBALAFFINEALIGNER_HH_
