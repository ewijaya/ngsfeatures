/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.10
 *  Last Modified: $Date: 2009/06/03 03:24:20 $
 *
 *  Description: Compute Hamming distance between to sequences
 *
 */
#ifndef _HAMMINGDISTANCECOMPUTER_HH
#define _HAMMINGDISTANCECOMPUTER_HH
#include <iostream>
#include "../LabeledSequence.hh"

namespace cbrc{

namespace HammingDistanceComputer{

  /* 
   * Compute hammingDistance allowing seq0, seq1 to have unequal length
   *
   * if $d$ = hammingDistance( seq0, seq1 ) < THRESHOLD
   *   return $d$
   * else
   *   return smallest integer ≧ THRESHOLD
   *
   * if seq0, seq1 have uneven length, the extra characters are treated as
   * mismatches at the end of the alignment
   */
  size_t computeBounded( const LabeledSequence& seq0,
			 const LabeledSequence& seq1,
			 const double&          threshold );


  size_t computeBoundedGapAtEnd( const std::string&  seq0,
				 const std::string&  seq1,
				 const double&       threshold );

  /* 
   * Same as computeBounded, except that when seq0, seq1 have uneven
   * length, the extra characters are treated as mismatches at the
   * *beginning* of the alignment.
   */
  size_t computeBoundedGapAtBeg( const LabeledSequence& seq0,
				 const LabeledSequence& seq1,
				 const double&          threshold );

  size_t computeBoundedGapAtBeg( const std::string&  seq0,
				 const std::string&  seq1,
				 const double&       threshold );

};

} // end namespace cbrc
#endif // _HAMMINGDISTANCECOMPUTER_HH
