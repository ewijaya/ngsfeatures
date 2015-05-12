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
 *  Description: Class for global affine gap alignment of protein or nucleic acid sequences.
 *
 */

#ifndef _GLOBALAFFINEDIST_HH_
#define _GLOBALAFFINEDIST_HH_
#include "utils/sequence/align/AminoDistance.hh"
#include "utils/sequence/align/Alignment.hh"
#include "utils/FLArray/FLArray.hh"
#include "utils/FLArray/FLMatrix.hh"

namespace cbrc{

class GlobalAffineDist {
public:
  GlobalAffineDist( const AminoDistance& ad, const FLArray<residueIndex>& seq0 ) : ad(ad), s0(seq0){
    dpTable.setSize( 2, s0.size()+1 );
    gpTable.setSize( 2, s0.size()+1 );
    bestPossibleDistVar = bestPossibleDist( ad, s0 );
  }
  alignDistT dist( const FLArray<residueIndex>& s1 );
  Alignment alignment( const FLArray<residueIndex>& s1 );
  alignDistT distFreeStartGap( const FLArray<residueIndex>& s1 );
  alignDistT distBounded( const FLArray<residueIndex>& s1, const FLArray<alignDistT>& s1BestPossibleSuffixDists, 
			   const alignDistT maxDistance );
  alignDistT prefixDistBounded( const FLArray<residueIndex>& s1, alignDistT maxDistance );
  alignDistT oneSequenceBasedUpperBound( const FLArray<residueIndex>& s1 ){
    if( s0.size() == s1.size() ) return bestPossibleDistVar;
    if( s0.size() < s1.size() ){
      return( bestPossibleDistVar + (s1.size() - s0.size()) * ad.gapExtension() + ad.gapInitiation() );
    }
    return(  bestPossibleDist( ad, s1 ) + (s0.size() - s1.size()) * ad.gapExtension() + ad.gapInitiation() );
  }
  alignDistT lengthBasedUpperBound( size_t l1 ){
    if( l1 == s0.size() ) return l1 * ad.minDistance();
    if( l1 > s0.size() ) return( ad.minDistance() * s0.size() 
				   + (l1-s0.size()) * ad.gapExtension() + ad.gapInitiation() );
    return( ad.minDistance() * l1 + (s0.size()-l1) * ad.gapExtension() + ad.gapInitiation() );
  }
  alignDistT feasibleBound( const FLArray<residueIndex>& s1 );
  const FLArrayFast<residueIndex> getResidues() const{ return s0; }
  static alignDistT bestPossibleDist( const AminoDistance& ad, const FLArray<residueIndex>& s1 ){
    alignDistT dist = 0;
    for( size_t i = 0; i < s1.size(); ++i )  dist += ad.minDistance( s1[i] );
    return dist;
  }
  static FLArray<alignDistT> bestPossibleSuffixDist( const AminoDistance& ad, const FLArray<residueIndex>& s0 ){
    FLArray<alignDistT> dists( s0.size() );
    if( !s0.size() ) return dists;
    dists[ s0.size()-1 ] = ad.minDistance( s0[ s0.size()-1 ] );
    for( size_t i = s0.size()-2; i < s0.size() /* while i >= 0 */ ; --i ){
      dists[ i ] = dists[ i+1 ] + ad.minDistance( s0[i] );
    }
    return dists;
  }
  friend void testGlobalAffineDist();
private:
  inline static alignDistT min( const alignDistT a, const alignDistT b ){ return( (a < b) ? a : b ); }
  inline static alignDistT min( const alignDistT a, const alignDistT b, const alignDistT c ){
    if( a < b ) return( min( a, c ) );
    else        return( min( b, c ) );
  }
  const AminoDistance& ad;
  const FLArrayFast<residueIndex> s0;
  FLMatrixFast<alignDistT> dpTable;
  FLMatrixFast<alignDistT> gpTable;
  alignDistT bestPossibleDistVar;
};


}; // end namespace
#endif // defined _GLOBALAFFINEDIST_HH_

