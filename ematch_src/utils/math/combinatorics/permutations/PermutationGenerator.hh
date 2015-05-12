/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.30
 *  Last Modified: $Date: 2006/04/30 13:45:46 $
 *
 *  Description: Compute all permutations of numbers from 0..n
 *
 *  Purpose: Originally created for computing p-values for
 *           rank correlation coefficient
 */

#ifndef _PERMUTATIONGENERATOR_HH_
#define _PERMUTATIONGENERATOR_HH_
#include "utils/FLArray/FLEArray.hh"
#include "RecursivePermutationGenerator.hh"

namespace cbrc{


class PermutationGenerator{
public:
  PermutationGenerator( const size_t& _size ) 
    : a( _size ), mask( _size, (unsigned char) false ){
    recursivePermPtr = new RecursivePermutationGenerator( a, mask, _size-1 );
  }
  const size_t& operator()( const size_t& index ) const{
    return a[index];
  }
  size_t size() const{
    return a.size();
  }
  bool next(){
    return recursivePermPtr->next();
  }
  friend void tryPermutationGenerator();
  const FLENumArray<size_t>& arrayRef() const{ return a; }
  void printMask( std::ostream& os = std::cout ) const{
    for( size_t i = 0; i < mask.size(); ++i ){
      if( i ) os << " ";
      os << mask[i];
    }
    os << std::endl;
  }
private:
  RecursivePermutationGenerator* recursivePermPtr;
  FLENumArray<size_t> a;
  FLEArray<unsigned char> mask;
};

std::ostream& operator<<( std::ostream& os, const PermutationGenerator& pg ){
  for( size_t i = 0; i < pg.size(); ++i ){
    if( i ) os << " ";
    os << pg(i);
  }
  return os;
}

}; // end namespace cbrc
#endif // defined _PERMUTATIONGENERATOR_HH_

