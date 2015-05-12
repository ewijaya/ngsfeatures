/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.30
 *  Last Modified: $Date: 2006/04/30 01:51:19 $
 *
 *  Description: Class for computing permutation. Does the real work
 *               but is called through PermutationGenerator.
 *
 *  works on the following recursive description of a permutation generator.
 *
 *  for( i = 0; i <= n; ++i ){
 *    1. place n in position i
 *    2. generator all permutations of 0..n-1 in the other positions
 *  }
 *
 *  Caveat: This is a home brewed algorithm and I suspect it is
 *          a bit slow. I am sure that somewhere, perhaps
 *          Knuth's book on permutations one may find a better one.
 *
 *  Purpose: Originally created to compute exact p-values of the
 *           rank correlation statistic for small sample sizes
 *           for which approximations may be poor.
 */

#ifndef _RECURSIVEPERMUTATIONGENERATOR_HH_
#define _RECURSIVEPERMUTATIONGENERATOR_HH_
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

class RecursivePermutationGenerator{
public:
  // Construct with initial permutation n, n-1, ..., 0
  RecursivePermutationGenerator( FLEArray<size_t>& a,
				 FLEArray<unsigned char> mask,
				 const size_t curIntegerToPlaceArg );
  ~RecursivePermutationGenerator(){ delete subPermPtr; }

  // proceed to next permutation or return false if all permutations 
  // have already been generated.
  bool next(); 
private:
  void checkConsistencyRecursively() const{
#ifdef CBRC_DEBUG
    checkMaskConsistencyRecursively();
#endif // defined CBRC_DEBUG
  }
  void resetMaskRecursively();
  bool baseCase() const{ return( !curIntegerToPlace ); }
  size_t numTrueMaskElements() const{
    size_t count = 0;
    for( size_t i = 0; i < mask.size(); ++i )  if( mask[i] ) ++count;
    return count;
  }
  void setMaskRecursively( size_t index, bool value ){
    mask[index] = value;
    if( !baseCase() ) subPermPtr->setMaskRecursively( index, value );
  }
  void setPosAndAToRightmostTrueRecursively(){
    setPosAndAToRightmostTrue();
    if( !baseCase() ) subPermPtr->setPosAndAToRightmostTrueRecursively();
  }
  void setPosAndAToRightmostTrue(){
    pos = mask.size()-1;
    for( ; mask[pos]; --pos );
    a[pos] = curIntegerToPlace;
  }
  bool advancePos();
#ifdef CBRC_DEBUG
  void checkMaskConsistencyRecursively() const;
  void checkMaskConsistency() const;
#endif // defined CBRC_DEBUG
  friend void tryRecursivePermutationGenerator();
private:
  FLEArray<size_t>& a;
  FLEArray<unsigned char> mask;
  const size_t curIntegerToPlace;
  size_t pos;
  RecursivePermutationGenerator* subPermPtr;
};

}; // end namespace cbrc
#endif // defined _RECURSIVEPERMUTATIONGENERATOR_HH_
