/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.30
 *  Last Modified: $Date: 2006/04/30 01:51:34 $
 *  Description: See header file.
 */
#include "RecursivePermutationGenerator.hh"

namespace cbrc{

RecursivePermutationGenerator::RecursivePermutationGenerator( 
  FLEArray<size_t>& a, 
  FLEArray<unsigned char> mask,
  const size_t curIntegerToPlaceArg )
  : a(a), mask(mask), curIntegerToPlace(curIntegerToPlaceArg), pos(0),
    subPermPtr(NULL){
  // Constructed with curIntegerToPlace, the object is responsible
  // for placing curIntegerToPlace at the appropriate place in the
  // permutation.
  //
  // The series of objects reachable recursively through subPermPtr
  // are responsible for the numbers less than curIntegerToPlace.
  //
  // mask elements with true values are unavailable for filling
  // in elements.

  setPosAndAToRightmostTrue();
  if( baseCase() )  return;

  // create copy of mask and set the rightmost false elment to true.
  FLEArray<unsigned char> subMask( mask );
  for( size_t i = mask.size()-1; i < mask.size(); --i ){
    if( !mask[i] ){
      subMask[i] = true;
      break;
    }
  }

  subPermPtr = 
    new RecursivePermutationGenerator( a, subMask, curIntegerToPlaceArg-1 );
  assert( subPermPtr );
  checkConsistencyRecursively();
}

bool RecursivePermutationGenerator::next(){
  if( baseCase() )  return false;
  else{
    if( subPermPtr->next() ){
      checkConsistencyRecursively();
      return true;
    }
    else{
      size_t prevPos = pos;
      if( !advancePos() ){
	checkConsistencyRecursively();
	return false;
      }
      a[pos] = curIntegerToPlace;
      subPermPtr->mask[prevPos] = false;
      subPermPtr->mask[pos]     = true;
      subPermPtr->resetMaskRecursively();
      subPermPtr->setPosAndAToRightmostTrueRecursively();
      checkConsistencyRecursively();
      return true;
    }
  }
}

void RecursivePermutationGenerator::resetMaskRecursively(){
  if( baseCase() )  return;
  subPermPtr->mask = mask;
  for( size_t i = mask.size()-1; i < mask.size(); --i ){
    if( !mask[i] ){
      subPermPtr->mask[i] = true;
      break;
    }
  }
  subPermPtr->resetMaskRecursively();
}

bool RecursivePermutationGenerator::advancePos(){ 
  --pos;
  if( pos >= mask.size() )  return false;
  while( mask[pos] ){
    --pos;
    if( pos >= mask.size() )  return false;
  }
  return true;
}


#ifdef CBRC_DEBUG
void RecursivePermutationGenerator::checkMaskConsistencyRecursively() const{
    checkMaskConsistency();
    if( !baseCase() )  subPermPtr->checkMaskConsistencyRecursively();
  }
void RecursivePermutationGenerator::checkMaskConsistency() const{
    assert( 1 + curIntegerToPlace + numTrueMaskElements() == mask.size() );
    if( !baseCase() ){
      for( size_t i = 0; i < mask.size(); ++i ){
	if( mask[i] ) assert( (subPermPtr->mask[i]) );
      }
    }
  }
#endif // defined CBRC_DEBUG


}; // end namespace cbrc

