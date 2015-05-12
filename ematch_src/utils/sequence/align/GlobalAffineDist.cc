/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.10.20
 *
 *  Last Modified: 2003.10.20
 *  
 *  Copyright: All rights reserved
 *  
 *  General Description: See header files.
 *
 *  Definitions: dpTable(i,j) is the minimum cost alignment of prefixes that aligns s0[i] with s0[j].
 *               gpTable(i,j) is the minimum cost alignment of prefixes that ends in a "gap".
 *
 *               In addition to gaps made by extending existing gaps, a gap may be a length 0 "initiator gap".
 *               an initiator gap consumes no characters but costs gapInitiation.
 *
 *               thus gpTable(i,j) <= dpTable(i,j) + gapInitiation, because an initiator gap can be added
 *               to the alignment represented by dpTable(i,j).
 */

#include "utils/sequence/align/GlobalAffineDist.hh"
#include "limits.h"

namespace cbrc{

alignDistT GlobalAffineDist::dist( const FLArray<residueIndex>& s1 ){
  alignDistT adjustedDistTMax = alignDistTMax - ad.maxDistance(); // to prevent overflow when adding a term before comparing

  dpTable(0,0) = 0;
  gpTable(0,0) = ad.gapInitiation();

  // *debug*  std::cout << "so.size = " << s0.size() << std::endl;
  for( size_t j  = 1; j <= s0.size(); ++j ){
    dpTable(0,j) = adjustedDistTMax;
    gpTable(0,j) = gpTable(0,j-1) + ad.gapExtension();
  }

  dpTable(1,0) = adjustedDistTMax;
  gpTable(1,0) = gpTable(0,0) + ad.gapExtension();

  size_t i = 0; // initialized to suppress compiler warning
  for( size_t index0 = 1; index0 <= s1.size(); ++index0 ){
    i = index0 % 2;
    dpTable( i, 0 ) = adjustedDistTMax;
    gpTable( i, 0 ) = gpTable( !i, 0 ) + ad.gapExtension();
    for( size_t j = 1; j <= s0.size(); ++j ){
      dpTable(i,j) = min( dpTable(!i,j-1), gpTable(!i,j-1) )
	                 + ad.distance( s0[j-1], s1[index0-1] );
      gpTable(i,j) = min( dpTable(i,j)   + ad.gapInitiation(),
                          gpTable(i,j-1) + ad.gapExtension(),
			  gpTable(!i,j)  + ad.gapExtension() );
      // *debug*     std::cout <<  dpTable( i, j ) << " ";
    }
  }
  alignDistT dist = dpTable( i, s0.size() );
  if( dist > gpTable( i, s0.size() ) )  dist = gpTable( i, s0.size() );
  return dist;
}

Alignment GlobalAffineDist::alignment( const FLArray<residueIndex>& s1 ){
  FLMatrix<alignDistT> dpFullTable( s1.size()+1, s0.size()+1 );
  FLMatrix<alignDistT> gpFullTable( s1.size()+1, s0.size()+1 );
  dpFullTable(0,0) = 0;
  gpFullTable(0,0) = ad.gapInitiation();
  alignDistT adjustedDistTMax = alignDistTMax - ad.maxDistance(); // to prevent overflow when adding a term before comparing

  // *debug*  std::cout << "so.size = " << s0.size() << std::endl;
  for( size_t i = 1; i <= s0.size(); ++i ){
    dpFullTable(0,i) = adjustedDistTMax;
    gpFullTable(0,i) = gpFullTable(0,i-1) + ad.gapExtension();
    // *debug*  std::cout << dpFullTable(0,i) << " ";
  }
  // *debug*  std::cout << "\n";

  dpFullTable(1,0) = adjustedDistTMax;
  gpFullTable(1,0) = gpFullTable(0,0) + ad.gapExtension();

  for( size_t i = 1; i <= s1.size(); ++i ){
    dpFullTable( i, 0 ) = adjustedDistTMax;
    gpFullTable( i, 0 ) = gpFullTable( i-1, 0 )   + ad.gapExtension();
    // *debug*   std::cout << dpFullTable( i, 0 ) << " ";
    for( size_t j = 1; j <= s0.size(); ++j ){
      dpFullTable(i,j) = min( dpFullTable(i-1,j-1), gpFullTable(i-1,j-1) )
	                 + ad.distance( s0[j-1], s1[i-1] );
      gpFullTable(i,j) = min( dpFullTable(i,j)   + ad.gapInitiation(),
                              gpFullTable(i,j-1) + ad.gapExtension(),
			      gpFullTable(i-1,j) + ad.gapExtension() );
      // *debug*     std::cout <<  dpFullTable( i, j ) << " ";
    }
    // *debug*   std::cout << std::endl;
  }
  bool gapState = false;
  alignDistT dist = dpFullTable( s1.size(), s0.size() );
  std::cout << "dist not ending in gap is: " << dist << std::endl;
  std::cout << "dist ending in gap is: " << gpFullTable( s1.size(), s0.size() ) << std::endl;
  if( dist > gpFullTable( s1.size(), s0.size() ) ){
    dist = gpFullTable( s1.size(), s0.size() );
    gapState = true;
  }
  std::cout << "dist is: " << dist << std::endl;

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
      assert( (size_t) s0[cr0] < ad.getRim().sigma() );
      assert( (size_t) s1[cr1] < ad.getRim().sigma() );
#endif // defined CBRC_DEBUG
      as0.push_back( s0[cr0] );
      as1.push_back( s1[cr1] );
      gapState = ( gpFullTable.getVal(cr1, cr0, adjustedDistTMax) < dpFullTable.getVal(cr1, cr0, adjustedDistTMax) );
      //      std::cout << ad.getRim().getResidue( s0[cr0] ) << ad.getRim().getResidue( s1[cr1] ) << std::endl;
      cr0--;
      cr1--;
      continue;
    }
    // else gapState
    alignDistT minVal = min(  dpFullTable.getVal( cr1+1, cr0+1, adjustedDistTMax ) + ad.gapInitiation(),
		       gpFullTable.getVal( cr1+1, cr0,   adjustedDistTMax ) + ad.gapExtension(),
		       gpFullTable.getVal( cr1,   cr0+1, adjustedDistTMax ) + ad.gapExtension() );
    assert( minVal < adjustedDistTMax );
    if( (dpFullTable.getVal(cr1+1,cr0+1,adjustedDistTMax) + ad.gapInitiation()) == minVal ){
      gapState=false;
      continue;
    }
    if( (gpFullTable.getVal( cr1+1, cr0, adjustedDistTMax ) + ad.gapExtension()) == minVal ){
#ifdef CBRC_DEBUG
      assert( cr0 >= 0 );
      assert( (size_t) s0[cr0] < ad.getRim().sigma() );
#endif // defined CBRC_DEBUG
      as0.push_back( s0[cr0] );
      as1.push_back( Alignment::indelChar() );
      cr0--;
      continue;
    }
    assert(  (gpFullTable.getVal( cr1, cr0+1, adjustedDistTMax ) + ad.gapExtension()) == minVal  );
#ifdef CBRC_DEBUG
    assert( cr1 >= 0 );
    assert( (size_t) s1[cr1] < ad.getRim().sigma() );
#endif // defined CBRC_DEBUG
    as0.push_back( Alignment::indelChar() );
    as1.push_back( s1[cr1] );
    cr1--;
  } //  end while( (cr0 >= 0) || (cr1 >= 0 ) ){
  std::cout << std::endl << std::flush;
  std::string alignmentString0( &as0[0], as0.size() );
  std::string alignmentString1( &as1[0], as1.size() );
  //  std::cout << "string0: " << ad.getRim().getResidues( alignmentString0 ) << std::endl;
  //  std::cout << "string1: " << ad.getRim().getResidues( alignmentString1 ) << std::endl;
  //  reverse( alignmentString0.begin(), alignmentString0.end() );
  //  reverse( alignmentString1.begin(), alignmentString1.end() );
  return Alignment( ad.getRim(), alignmentString0, alignmentString1 );
} // end method::alignment


  // modify boundary conditions so that one may skip a prefix of one of the two sequences without penalty 
  // i.e. with a dist of 0.
alignDistT GlobalAffineDist::distFreeStartGap( const FLArray<residueIndex>& s1 ){
  dpTable(0,0) = 0;
  gpTable(0,0) = 0;

  // allow a prefix of one of the two sequences to be skipped for free.
  const alignDistT firstGapInitiation = 0;
  const alignDistT firstGapPenalty = 0;


  // *debug*  std::cout << "so.size = " << s0.size() << std::endl;
  for( size_t i = 1; i <= s0.size(); ++i ){
    dpTable(0,i) = dpTable(0,i-1) + firstGapPenalty;
    gpTable(0,i) = dpTable(0,i) + firstGapInitiation;
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
      dpTable(i,j) = min( dpTable(!i,j-1) + ad.distance( s0[j-1], s1[index0-1] ),
			  gpTable(i,j-1)   + ad.gapExtension(),
			  gpTable(!i,j)   + ad.gapExtension() );

      gpTable(i,j) = min( dpTable(i,j)   + ad.gapInitiation(),
			  gpTable(i,j-1) + ad.gapExtension(),
			  gpTable(!i,j) + ad.gapExtension() );
      // *debug*     std::cout <<  dpTable( i, j ) << " ";
    }
    // *debug*   std::cout << std::endl;
  }
  alignDistT dist = dpTable( i, s0.size() );
  if( dist > gpTable( i, s0.size() ) ){
    dist = gpTable( i, s0.size() );
  }
  return dist;
}


alignDistT GlobalAffineDist::prefixDistBounded( const FLArray<residueIndex>& s1, int maxDistance ){
  dpTable(0,0) = 0;
  gpTable(0,0) = ad.gapInitiation();

  // *debug*  std::cout << "so.size = " << s0.size() << std::endl;
  for( size_t i = 1; i <= s0.size(); ++i ){
    dpTable(0,i) = dpTable(0,i-1) + ad.gapExtension();
    gpTable(0,i) = dpTable(0,i) + ad.gapInitiation();
    // *debug*  std::cout << dpTable(0,i) << " ";
  }
  // *debug*  std::cout << "\n";

  dpTable(1,0) = ad.gapExtension();
  gpTable(1,0) = dpTable(1,0) + ad.gapInitiation();

  size_t i = 0; // initialized to suppress compiler warning
  for( size_t index0 = 1; index0 <= s1.size(); ++index0 ){
    i = index0 % 2;
    dpTable( i, 0 ) = dpTable( !i, 0 ) + ad.gapExtension();
    gpTable( i, 0 ) = dpTable( i, 0 )   + ad.gapInitiation();
    // *debug*   std::cout << dpTable( i, 0 ) << " ";

    alignDistT minDistance = gpTable(i,0);
    for( size_t j = 1; j <= s0.size(); ++j ){
      dpTable(i,j) = min( dpTable(!i,j-1) + ad.distance( s0[j-1], s1[index0-1] ),
			   gpTable(i,j-1)   + ad.gapExtension(),
			   gpTable(!i,j)   + ad.gapExtension() );
      if( dpTable(i,j) < minDistance ) minDistance = dpTable(i,j);
      gpTable(i,j) = min( dpTable(i,j)   + ad.gapInitiation(),
			   gpTable(i,j-1) + ad.gapExtension(),
			   gpTable(!i,j) + ad.gapExtension() );
      if( gpTable(i,j) < minDistance ) minDistance = gpTable(i,j);
      // *debug*     std::cout <<  dpTable( i, j ) << " ";
    }
    if( minDistance > maxDistance ) return alignDistTMax;
    // *debug*   std::cout << std::endl;
  }
  alignDistT dist = dpTable( i, s0.size() );
  if( dist > gpTable( i, s0.size() ) ){
    dist = gpTable( i, s0.size() -1 );
  }
  return dist;
}


alignDistT GlobalAffineDist::feasibleBound( const FLArray<residueIndex>& s1 ){
  alignDistT retVal = 0;
  if( s0.size() == s1.size() ){
    for( size_t i = 0; i < s0.size(); ++i )  retVal += ad.distance( s0[i], s1[i] ); // sum of diagonal
    return retVal;
  }
  alignDistT curDist = 0;
  if( s0.size() < s1.size() ){
    for( size_t i = 0; i < s0.size(); ++i )  curDist += ad.distance( s0[i], s1[i] );
    curDist += ad.gapInitiation();
    curDist = curDist + ( s1.size() - s0.size() ) * ad.gapExtension(); // distance with all gaps at end of s0
    retVal = curDist;
    size_t s0last = s0.size() - 1;
    size_t s1last = s1.size() - 1;
    for( size_t i = 0; i < s0.size(); ++i ){
      if( s1last-i < 0 ) break;
      curDist -= ad.distance( s0[s0last-i], s1[s0last-i] );
      curDist += ad.distance( s0[s0last-i], s1[s1last-i] );
      if( curDist < retVal ) retVal = curDist;
    }
    return retVal;
  }
  // s0.size() > s1.size()
  for( size_t i = 0; i < s1.size(); ++i )  curDist += ad.distance( s1[i], s0[i] );
  curDist += ad.gapInitiation();
  curDist = curDist + ( s0.size() - s1.size() ) * ad.gapExtension();  // distance with all gaps at end of s1
  retVal = curDist;
  size_t s1last = s1.size() - 1;
  size_t s0last = s0.size() - 1;
  for( size_t i = 0; i < s1.size(); ++i ){
    if( s0last-i < 0 ) break;
    curDist -= ad.distance( s1[s1last-i], s0[s1last-i] );
    curDist += ad.distance( s1[s1last-i], s0[s0last-i] );
    if( curDist < retVal ) retVal = curDist;
  }
  return retVal;
}

  // This function is not sufficiently tested. Currently not known to be buggy but has been a problem in the past.
alignDistT GlobalAffineDist::distBounded( const FLArray<residueIndex>& s1, const FLArray<alignDistT>& s1BestPossibleSuffixDists,
				     const alignDistT maxDistance ){
  alignDistT adjustedDistTMax = alignDistTMax - ad.maxDistance(); // to prevent overflow when adding a term before comparing

  dpTable(0,0) = 0;
  gpTable(0,0) = ad.gapInitiation();

  alignDistT lowerBound = bestPossibleDist( ad, s1 );
  if( lowerBound < bestPossibleDistVar ) lowerBound = bestPossibleDistVar;
  int maxStepsFromDiag;
  if( lowerBound > maxDistance ) return alignDistTMax;
  maxStepsFromDiag = (maxDistance - lowerBound) / ad.gapExtension();
  //  std::cout << "maxStepsFromDiag:" << maxStepsFromDiag << std::endl;

  for( size_t j  = 1; j <= s0.size(); ++j ){
    dpTable(0,j) = adjustedDistTMax;
    gpTable(0,j) = gpTable(0,j-1) + ad.gapExtension();
    // *debug*  std::cout << dpTable(0,i) << " ";
  }
  // *debug*  std::cout << "\n";

  dpTable(1,0) = adjustedDistTMax;
  gpTable(1,0) = gpTable(0,0) + ad.gapExtension();

  size_t i = 0; // initialized to suppress compiler warning

  int lengthDiff = s0.size() - s1.size();
  int jStart;
  int jEnd = 0; // initialized to suppress compiler warning
  
  // *debug*   std::cout << std::endl;
  // *debug*   std::cout << "==============================\n";

  alignDistT minValueThisRow = 0;
  for( size_t index0 = 1; index0 <= s1.size(); ++index0 ){
    i = index0 % 2;
    dpTable( i, 0 ) = adjustedDistTMax;
    gpTable( i, 0 ) = gpTable( !i, 0 ) + ad.gapExtension();

    // using minValueThisRow value from previous iteration
    assert( minValueThisRow < alignDistTMax );
    alignDistT newLowerBound = minValueThisRow + s1BestPossibleSuffixDists[index0-1];
    if( newLowerBound > lowerBound ){
      lowerBound = newLowerBound;

      if( lowerBound > maxDistance ){
	//	std::cout << " minValueThisRow: " << minValueThisRow << " s1BestPossibleSuffixDists[" << index0-1 << "] = " << s1BestPossibleSuffixDists[index0-1] << std::endl;
	// std::cout << "returning alignDistTMax\n";
	return alignDistTMax;
      }
      maxStepsFromDiag = (maxDistance - lowerBound) / ad.gapExtension();
    }

    // std::cout << " maxDistance " << maxDistance << " lowerBound " << lowerBound << " newLowerBound: " << newLowerBound << " minValueThisRow: " << minValueThisRow << " bestSuffix: " << s1BestPossibleSuffixDists[index0-1] << " index0: " << index0 << " maxStepsFromDiag: " << maxStepsFromDiag << std::endl;

    minValueThisRow = alignDistTMax;

    // std::cout << "jStart, jEnd are: " << jStart << ", " << jEnd;
    int jDiagonal = lengthDiff + index0;
    jStart = jDiagonal - maxStepsFromDiag;
    if( jStart < 1 )  jStart = 1;

    jEnd = jDiagonal + maxStepsFromDiag;

    if( jEnd < 0 ){
#ifdef CBRC_DEBUG
      std::cout << __FILE__ << ":" << __LINE__ << " jStart = " << jStart << " jEnd = " << jEnd << " returning alignDistTMax\n";
#endif // defined CBRC_DEBUG
      return alignDistTMax;
    }

    if(  jEnd < static_cast<int>( s0.size() )  ){
      gpTable( !i, jEnd ) = adjustedDistTMax;
    }else{
      jEnd = s0.size();
    }

    if( jEnd < jStart ){
#ifdef CBRC_DEBUG
      std::cout << __FILE__ << ":" << __LINE__ << " jStart = " << jStart << " jEnd = " << jEnd << " returning alignDistTMax\n";
#endif // defined CBRC_DEBUG
      return alignDistTMax;
    }

    //    std::cout << " jStart, jEnd are: " << jStart << ", " << jEnd << std::endl;
    if( jStart > 1 )  dpTable( i, jStart-1 ) = adjustedDistTMax;

    for( int j = jStart; j <= jEnd; ++j ){
      dpTable(i,j) = min( dpTable(!i,j-1), gpTable(!i,j-1) )  + ad.distance( s0[j-1], s1[index0-1] );
      if( minValueThisRow > dpTable(i,j ) )  minValueThisRow = dpTable(i,j);
      gpTable(i,j) = min( dpTable(i,j)   + ad.gapInitiation(),
			  gpTable(i,j-1) + ad.gapExtension(),
			  gpTable(!i,j)  + ad.gapExtension() );
      if( minValueThisRow > gpTable(i,j) )  minValueThisRow = gpTable(i,j);
    }
  } // next index0

  alignDistT dist = dpTable( i, jEnd ); // note: loop variable i should fall through to here.
  if( dist > gpTable( i, jEnd ) )  dist = gpTable( i, jEnd );
  return dist;
}
			
}; // end namespace
