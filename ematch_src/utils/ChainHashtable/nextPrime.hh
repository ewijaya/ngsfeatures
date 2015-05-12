/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.22
 *  Last Modified: $Date: 2008/11/22 08:31:49 $
 *
 *  Description: function to return a prime number near a given number.
 *
 *  Purpose: Created for use with hash tables
 *
 */
#ifndef NEXTPRIME_HH_
#define NEXTPRIME_HH_
#include <iostream>

namespace cbrc{

// if NUM >= max, output max.
// else  output the first number p from the prime list: p ≧ NUM.
inline size_t nextPrime( const size_t& num ){ 

  // NOTE: I should add some bigger primes for 64 bit architectures.
  const size_t max =  1796359771;
  const size_t
    somePrimes[] = {         11,        19,        37,        73,       109,
                            163,       251,       367,       557,       823,
		           1237,      1861,      2777,      4177,      6247, 
                           9371,     14057,     21089,     31627,     47431,
                          71143,    106721,    160073,    240101,    360163, 
	                 540217,    810343,   1215497,   1823231,   2734867,
                        4102283,   6153409,   9230113,  13845163,  20767751,
                       31151623,  46727449,  70091159, 105136753, 157705109,
                      236557751, 354836507, 532254761, 798382121,1197573203, max };
  if( num >= max ) return max;
  size_t i;
  for( i = 0; somePrimes[i] < num; ++i );
  return somePrimes[i];
}

} // end namespace cbrc
#endif // NEXTPRIME_HH_
