/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2008/06/26 10:26:22 $
 *  Copyright: All rights reserved
 *  
 *  General Description: See header files.
 *
 *  Definitions:
 *    dpTable(i,j) is the maximum score of alignments of s0[0..i], s1[0..j] that align s0[i] with s1[j].
 *    gpTable(i,j) is the maximum score of alignment of s0[0..i], s1[0..j] that align not s0[i] with s1[j].
 *
 *             In addition to gaps made by extending existing gaps, a gap may be a length 0 "initiator gap".
 *             an initiator gap consumes no characters but costs gapInitiation.
 *
 *             thus gpTable(i,j) <= dpTable(i,j) + gapInitiation, because an initiator gap can be added
 *             to the alignment represented by dpTable(i,j).
 *
 *  Implementation Note: Currently this implementation uses a strange mix of residueIndexMapped
 *                       characters for regular indices, and the non-indexed indel char '-' (= 45)
 *
 *                       Might be good to use the new method ResidueIndexMap::expandAlphabet
 *                       to index '-' as well. This will require coordination with the Alignment class.
 *                         
 */

#include "utils/sequence/align/GlobalAffineIdentityAligner.hh"
#include "limits.h"


namespace cbrc{

alignScoreT 
GlobalAffineIdentityAligner
::purelyLengthDifferenceBasedScoreUpperBound( const size_t& len1 ) const{
  if( len1 == s0.size() ) return len1 * resSim.maxScore();
  if( len1 >  s0.size() ){
    return( resSim.maxScore() * s0.size() 
	    + (len1-s0.size()) * resSim.gapExtension() + resSim.gapInitiation() );
  }
  return( resSim.maxScore()   * len1
	  +   (s0.size()-len1) * resSim.gapExtension() + resSim.gapInitiation() );
}


alignScoreT
GlobalAffineIdentityAligner
::lengthDifferenceBasedScoreUpperBound( const ResidueIndexMap::arrayT& s1 ) const{
  if( s0.size() == s1.size() ) return resSim.maxScore();
  if( s0.size() < s1.size() ){
    return( bestPossibleScore()
	    + (s1.size() - s0.size()) * resSim.gapExtension()
	    + resSim.gapInitiation() );
  }
  return(  singleSeqBasedScoreUpperBound( s1 )
	   + (s0.size() - s1.size()) * resSim.gapExtension() 
	   + resSim.gapInitiation() );
}


alignScoreT GlobalAffineIdentityAligner::score( const ResidueIndexMap::arrayT& s1 ) const{
  alignScoreT adjustedScoreTMin = alignScoreTMin;
  if( resSim.minScore() < 0 ){
    // prevent overflow when adding a term before comparing
    adjustedScoreTMin -= resSim.minScore(); 
  }

  dpTable(0,0) = 0;
  gpTable(0,0) = resSim.gapInitiation();

  for( size_t j  = 1; j <= s0.size(); ++j ){
    dpTable(0,j) = adjustedScoreTMin;
    gpTable(0,j) = gpTable(0,j-1) + resSim.gapExtension();
  }
  
  dpTable(1,0) = adjustedScoreTMin;
  gpTable(1,0) = gpTable(0,0) + resSim.gapExtension();

  size_t i = 0; // initialized to suppress compiler warning
  for( size_t index0 = 1; index0 <= s1.size(); ++index0 ){
    i = index0 % 2;
    dpTable( i, 0 ) = adjustedScoreTMin;
    gpTable( i, 0 ) = gpTable( !i, 0 ) + resSim.gapExtension();
    for( size_t j = 1; j <= s0.size(); ++j ){
      dpTable(i,j) = max( dpTable(!i,j-1), gpTable(!i,j-1) )
	                 + resSim.score( s0[j-1], s1[index0-1] );
      gpTable(i,j) = max( dpTable(i,j)   + resSim.gapInitiation(),
                          gpTable(i,j-1) + resSim.gapExtension(),
			  gpTable(!i,j)  + resSim.gapExtension() );
    }
  }
  alignScoreT score = dpTable( i, s0.size() );
  if( score < gpTable( i, s0.size() ) )  score = gpTable( i, s0.size() );
  return score;
}


Alignment GlobalAffineIdentityAligner::alignment( const ResidueIndexMap::arrayT& s1 ){
  FLEMatrix<alignScoreT> dpFullTable( s1.size()+1, s0.size()+1 );
  FLEMatrix<alignScoreT> gpFullTable( s1.size()+1, s0.size()+1 );

  dpFullTable(0,0) = 0;
  gpFullTable(0,0) = resSim.gapInitiation();

  alignScoreT adjustedScoreTMin = alignScoreTMin;
  if( resSim.minScore() < 0 ){
    // prevent overflow when adding a term before comparing
    adjustedScoreTMin -= resSim.minScore(); 
  }

#ifdef GLOBAL_AFFINE_SCORE_DUMP_TABLE
  std::cout << "so.size = " << s0.size() << std::endl;
#endif
  for( size_t i = 1; i <= s0.size(); ++i ){
    dpFullTable(0,i) = adjustedScoreTMin;
    gpFullTable(0,i) = gpFullTable(0,i-1) + resSim.gapExtension();
#ifdef GLOBAL_AFFINE_SCORE_DUMP_TABLE
    std::cout << dpFullTable(0,i) << " ";
#endif
  }
#ifdef GLOBAL_AFFINE_SCORE_DUMP_TABLE
  std::cout << "\n";
#endif

  for( size_t i = 1; i <= s1.size(); ++i ){
    dpFullTable( i, 0 ) = adjustedScoreTMin;
    gpFullTable( i, 0 ) = gpFullTable( i-1, 0 ) + resSim.gapExtension();
#ifdef GLOBAL_AFFINE_SCORE_DUMP_TABLE
    std::cout << dpFullTable( i, 0 ) << " ";
#endif
    for( size_t j = 1; j <= s0.size(); ++j ){
      dpFullTable(i,j) = max( dpFullTable(i-1,j-1), gpFullTable(i-1,j-1) )
	                 + resSim.score( s0[j-1], s1[i-1] );
      gpFullTable(i,j) = max( dpFullTable(i,j)   + resSim.gapInitiation(),
                              gpFullTable(i,j-1) + resSim.gapExtension(),
			      gpFullTable(i-1,j) + resSim.gapExtension() );
#ifdef GLOBAL_AFFINE_SCORE_DUMP_TABLE
      std::cout <<  dpFullTable( i, j ) << " ";
#endif
    }
#ifdef GLOBAL_AFFINE_SCORE_DUMP_TABLE
    std::cout << std::endl;
#endif
  }
  bool gapState = false;
  alignScoreT score = dpFullTable( s1.size(), s0.size() );
  if( score < gpFullTable( s1.size(), s0.size() ) ){
    score = gpFullTable( s1.size(), s0.size() );
    gapState = true;
  }

  // trace back through table to recover alignment
  std::vector<char> as0;
  std::vector<char> as1;
  int cr0 = s0.size()-1; // cr for current residue.
  int cr1 = s1.size()-1;
  while( (cr0 >= 0) || (cr1 >= 0 ) ){
    if( !gapState ){
#ifdef CBRC_DEBUG
      assert( cr0 >= 0 );
      assert( cr1 >= 0 );
      assert( (size_t) s0[cr0] < resSim.residueIndexMap().sigma() );
      assert( (size_t) s1[cr1] < resSim.residueIndexMap().sigma() );
#endif // defined CBRC_DEBUG
      as0.push_back( s0[cr0] );
      as1.push_back( s1[cr1] );
      gapState = ( /**/ gpFullTable.getVal(cr1, cr0, adjustedScoreTMin)
		      > dpFullTable.getVal(cr1, cr0, adjustedScoreTMin)  );
      cr0--;
      cr1--;
      continue;
    }
    // else gapState
    alignScoreT
      maxVal
      = max(  dpFullTable.getVal( cr1+1, cr0+1, adjustedScoreTMin ) + resSim.gapInitiation(),
	      gpFullTable.getVal( cr1+1, cr0,   adjustedScoreTMin ) + resSim.gapExtension(),
	      gpFullTable.getVal( cr1,   cr0+1, adjustedScoreTMin ) + resSim.gapExtension()  );
#ifdef CBRC_DEBUG
    assert( maxVal > adjustedScoreTMin );
#endif // defined CBRC_DEBUG
    if( (dpFullTable.getVal(cr1+1, cr0+1, adjustedScoreTMin) + resSim.gapInitiation())
	== maxVal ){
      gapState=false;
      continue;
    }
    if( (gpFullTable.getVal( cr1+1, cr0, adjustedScoreTMin ) + resSim.gapExtension())
	== maxVal ){
#ifdef CBRC_DEBUG
      assert( cr0 >= 0 );
      assert( (size_t) s0[cr0] < resSim.residueIndexMap().sigma() );
#endif // defined CBRC_DEBUG
      as0.push_back( s0[cr0] );
      as1.push_back( Alignment::indelChar() );
      cr0--;
      continue;
    }

    assert(  (gpFullTable.getVal( cr1, cr0+1, adjustedScoreTMin ) + resSim.gapExtension())
	     == maxVal  );
#ifdef CBRC_DEBUG
    assert( cr1 >= 0 );
    assert( (size_t) s1[cr1] < resSim.residueIndexMap().sigma() );
#endif // defined CBRC_DEBUG
    as0.push_back( Alignment::indelChar() );
    as1.push_back( s1[cr1] );
    cr1--;
  } //  end while( (cr0 >= 0) || (cr1 >= 0 ) )
  std::string alignmentString0( &as0[0], as0.size() );
  std::string alignmentString1( &as1[0], as1.size() );
  std::reverse( alignmentString0.begin(), alignmentString0.end() );
  std::reverse( alignmentString1.begin(), alignmentString1.end() );
  Alignment retVal( resSim.residueIndexMap(), alignmentString0, alignmentString1 );
#if CBRC_OPTIMIZE <= 2
  retVal.assertStringsValid();
#endif // CBRC_OPTIMIZE <= 2
  return retVal;
} // end method::alignment


  // modify boundary conditions so that one may skip a prefix of one of the two sequences without penalty 
  // i.e. with a score of 0.
alignScoreT GlobalAffineIdentityAligner::scoreFreeStartGap( const ResidueIndexMap::arrayT& s1 ){
  dpTable(0,0) = 0;
  gpTable(0,0) = 0;

  // allow a prefix of one of the two sequences to be skipped for free.
  const alignScoreT firstGapInitiation = 0;
  const alignScoreT firstGapPenalty = 0;


  // *debug*  std::cout << "so.size = " << s0.size() << std::endl;
  for( size_t i = 1; i <= s0.size(); ++i ){
    dpTable(0,i) = dpTable(0,i-1) + firstGapPenalty;
    gpTable(0,i) = dpTable(0,i)   + firstGapInitiation;
    // *debug*  std::cout << dpTable(0,i) << " ";
  }
  // *debug*  std::cout << "\n";

  dpTable(1,0) = firstGapPenalty;
  gpTable(1,0) = dpTable(1,0);

  size_t i = 0; // initialized to suppress compiler warning
  for( size_t index0 = 1; index0 <= s1.size(); ++index0 ){
    i = index0 % 2;
    dpTable( i, 0 ) = dpTable( !i, 0 );
    gpTable( i, 0 ) = dpTable( i, 0 );
    // *debug*   std::cout << dpTable( i, 0 ) << " ";
    for( size_t j = 1; j <= s0.size(); ++j ){
      dpTable(i,j) = max( dpTable(!i,j-1) + resSim.score( s0[j-1], s1[index0-1] ),
			  gpTable(i,j-1)  + resSim.gapExtension(),
			  gpTable(!i,j)   + resSim.gapExtension() );

      gpTable(i,j) = max( dpTable(i,j)   + resSim.gapInitiation(),
			  gpTable(i,j-1) + resSim.gapExtension(),
			  gpTable(!i,j)  + resSim.gapExtension() );
      // *debug*     std::cout <<  dpTable( i, j ) << " ";
    }
    // *debug*   std::cout << std::endl;
  }
  alignScoreT score = dpTable( i, s0.size() );
  if( score < gpTable( i, s0.size() ) ){
    score = gpTable( i, s0.size() );
  }
  return score;
} // end alignScoreT GlobalAffineIdentityAligner::scoreFreeStartGap( const ResidueIndexMap::arrayT& s1 )



alignScoreT GlobalAffineIdentityAligner::feasibleBound( const ResidueIndexMap::arrayT& s1 ){

  // if sequence lengths are equal, return score of alignment with no gaps
  if( s0.size() == s1.size() ){
    alignScoreT retVal = 0;
    for( size_t i = 0; i < s0.size(); ++i ){
      retVal += resSim.score( s0[i], s1[i] ); // sum of diagonal
    }
    return retVal;
  }

  /* if sequences are of unequal length, try with the shorter sequence flushed
   * left (leftScore) or right (rghtScore) and return the maximum of the two.
   */
  alignScoreT leftScore = 0, rghtScore = 0;

  if( s0.size() < s1.size() ){
    const size_t lenDiff  =  s1.size() - s0.size();

    // compute leftScore.
    for( size_t i = 0; i < s0.size(); ++i ){
      leftScore += resSim.score( s0[i], s1[i] );
    }
    leftScore += resSim.gapInitiation();
    leftScore += lenDiff * resSim.gapExtension();

    // compute rghtScore
    for( size_t i = 0; i < s0.size(); ++i ){
      rghtScore += resSim.score( s0[i], s1[i+lenDiff] );
    }
    rghtScore += resSim.gapInitiation();
    rghtScore += lenDiff * resSim.gapExtension();
    
    return  max( leftScore, rghtScore );
  }

  // s1.size() < s0.size()
  const size_t lenDiff  =  s0.size() - s1.size();

    // compute leftScore.
    for( size_t i = 0; i < s1.size(); ++i ){
      leftScore += resSim.score( s0[i], s1[i] );
    }
    leftScore += resSim.gapInitiation();
    leftScore += lenDiff * resSim.gapExtension();

    // compute rghtScore
    for( size_t i = 0; i < s1.size(); ++i ){
      rghtScore += resSim.score( s0[i+lenDiff], s1[i] );
    }
    rghtScore += resSim.gapInitiation();
    rghtScore += lenDiff * resSim.gapExtension();
    
    return  max( leftScore, rghtScore );
} // end method feasibleBound
    

  // This function is not sufficiently tested. Currently not known to be buggy but has been a problem in the past.
alignScoreT
GlobalAffineIdentityAligner
::scoreBounded( const ResidueIndexMap::arrayT& s1, 
		const FLEArrayFast<alignScoreT>& s1BestPossibleSuffixScores,
		const alignScoreT givenLowerBound ){
  alignScoreT adjustedScoreTMin = alignScoreTMin;
  if( resSim.minScore() < 0 ){
    // prevent underflow when adding a number to this before comparing
    adjustedScoreTMin -= resSim.minScore();
  }

  dpTable(0,0) = 0;
  gpTable(0,0) = resSim.gapInitiation();

  alignScoreT upperBound = resSim.maxScore();
  if( upperBound > resSim.maxScore() ) upperBound = resSim.maxScore();
  // 'upperBound' is an upper bound on the contribution of match states to the alignment score.

  int maxStepsFromDiag;
  if( upperBound < givenLowerBound )  return alignScoreTMin;
  maxStepsFromDiag = (givenLowerBound - upperBound) / resSim.gapExtension();  // gapExtension is normally < 0.

#ifdef CBRC_DEBUG
      assert( maxStepsFromDiag >= 0 );
#endif // defined CBRC_DEBUG

  for( size_t j  = 1; j <= s0.size(); ++j ){
    dpTable(0,j) = adjustedScoreTMin;
    gpTable(0,j) = gpTable(0,j-1) + resSim.gapExtension();
  }

  dpTable(1,0) = adjustedScoreTMin;
  gpTable(1,0) = gpTable(0,0) + resSim.gapExtension();
  size_t i = 0; // initialized to suppress compiler warning

  int lengthDiff = s0.size() - s1.size();
  int jStart;
  int jEnd = s0.size(); // initialized to suppress compiler warning
  int prevJend = s0.size(); // It is important that prevJend is initialized to >= s0.size().
  
  // technically should be max( dpTable(0,0), gpTable(0,0) ).
  alignScoreT maxValueThisRow = dpTable( 0, 0 );

  for( size_t index0 = 1; index0 <= s1.size(); ++index0 ){
    i = index0 % 2;
    dpTable( i, 0 ) = adjustedScoreTMin;
    gpTable( i, 0 ) = gpTable( !i, 0 ) + resSim.gapExtension();

    // using maxValueThisRow value from previous iteration
    assert( maxValueThisRow > alignScoreTMin );
    alignScoreT newUpperBound = maxValueThisRow + s1BestPossibleSuffixScores[index0-1];
    if( upperBound > newUpperBound ){
      upperBound = newUpperBound;

      if( upperBound < givenLowerBound )  return alignScoreTMin;
      maxStepsFromDiag = (givenLowerBound - upperBound) / resSim.gapExtension();  // gapExtension is normally < 0.
#ifdef CBRC_DEBUG
      assert( maxStepsFromDiag >= 0 );
#endif // defined CBRC_DEBUG
    }

    maxValueThisRow = alignScoreTMin;

    int jDiagonal = lengthDiff + index0;
    jStart = jDiagonal - maxStepsFromDiag;

    jEnd = jDiagonal + maxStepsFromDiag;

    if( jEnd < 0 )  return alignScoreTMin;

    // initialize the cell to the left of index0, jStart. New code 2004.11.24
    if( jStart < 1 )  jStart = 1;
    else              gpTable( i, jStart-1 ) = adjustedScoreTMin;


    // Initialize cells skipped to the right of the cutoff while computing the previous row.
    if( jEnd > static_cast<int>( s0.size() ) ) jEnd = s0.size();
    for( int uninitiatedCellJ = prevJend+1; uninitiatedCellJ <= jEnd; ++uninitiatedCellJ ){
      gpTable( !i, uninitiatedCellJ ) = adjustedScoreTMin;
    }
    prevJend = jEnd;

    if( jEnd < jStart )  return alignScoreTMin;

    if( jStart > 1 )  dpTable( i, jStart-1 ) = adjustedScoreTMin;

    for( int j = jStart; j <= jEnd; ++j ){
      dpTable(i,j) = max( dpTable(!i,j-1), gpTable(!i,j-1) ) + resSim.score( s0[j-1], s1[index0-1] );
      if( maxValueThisRow < dpTable(i,j ) )  maxValueThisRow = dpTable(i,j);
      gpTable(i,j) = max( dpTable(i,j)   + resSim.gapInitiation(),
			  gpTable(i,j-1) + resSim.gapExtension(),
			  gpTable(!i,j)  + resSim.gapExtension() );
      if( maxValueThisRow < gpTable(i,j) )  maxValueThisRow = gpTable(i,j);
    }
  } // next index0

  alignScoreT score = dpTable( i, jEnd ); // note: loop variable i should fall through to here.
  if( score < gpTable( i, jEnd ) )  score = gpTable( i, jEnd );
  return score;
} // end method scoreBounded




void
GlobalAffineIdentityAligner
::bestPossibleSuffixScore( const ResidueIndexMap::arrayT& s0,
			   /***/ FLEArray<alignScoreT>&   suffixScores ) const{
  assert( s0.size() == suffixScores.size() );

  if( !s0.size() ) return;

  suffixScores[ s0.size()-1 ] = resSim.maxScore();
  for( size_t i = s0.size()-2; i < s0.size() /* while i >= 0 */ ; --i ){
    suffixScores[ i ] = suffixScores[ i+1 ] + resSim.maxScore();
  }
  return;
}


			
}; // end namespace
