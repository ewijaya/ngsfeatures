/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Creation Date: 2003.6.18
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: (high level purpose or motivation)
 *
 *  Usage: (explain or give example of use, not necessary the same as the command line useage message)
 *
 *  Description: (reasonably detailed description of what this file does)
 *
 *  Reference: (Reference to relevant literature, only applicable sometimes)
 */

#ifndef _MATRIXALGORITHM_HH_
#define _MATRIXALGORITHM_HH_

#include <limits>
#include <ctime>
#include <cmath>
#include "Matrix.hh"

namespace cbrc {

template<typename ValType>
inline void log_transform(Matrix<ValType>& M)
{
  size_t rsize = M.row_size();
  size_t csize = M.column_size();

  for(int r = 0; r < rsize; r++) {
    for(int c = 0; c < csize; c++) {
      if(M.check_valid(r, c) == true) {
        if(M[r][c] > 0.0) M[r][c] = (ValType)log10(M[r][c]);
        else M.set_valid(r,c,false);
      }
    }
  }
}


template<typename ValType, typename ResultPrecision>
inline ResultPrecision pearson_corr(Matrix<ValType>& M, const size_t pos1, const size_t pos2, size_t *valid_data, bool ColumnCompare = 0)
{
  size_t rsize = M.row_size();
  size_t csize = M.column_size();

  size_t ivalid = 0;
  ResultPrecision sum1, sum2, ssum11, ssum22, csum12;
  sum1 = sum2 = ssum11 = ssum22 = csum12 = 0.0;
  ResultPrecision resultval = 0.0;

  if(ColumnCompare) { // For the corr. of column1 and column2

    for(size_t r = 0; r < rsize; r++) {
      if(M.check_valid(r, pos1) && M.check_valid(r, pos2)) {
	++ivalid;

	 //convert from ValType to ResultPrecision
	ResultPrecision v1 = M[r][pos1];
	ResultPrecision v2 = M[r][pos2];
	sum1 += v1;
	sum2 += v2;
	ssum11 += v1 * v1;
	ssum22 += v2 * v2;
	csum12 += v1 * v2;
      }
    }

  } else { // For the corr. of row1 and row2

    for(size_t c = 0; c < csize; c++) {
      if(M.check_valid(pos1, c) && M.check_valid(pos2, c)) {
	++ivalid;

	 //convert from ValType to ResultPrecision
	ResultPrecision v1 = M[pos1][c];
	ResultPrecision v2 = M[pos2][c];
	sum1 += v1;
	sum2 += v2;
	ssum11 += v1 * v1;
	ssum22 += v2 * v2;
	csum12 += v1 * v2;
      }
    }
  }

  *valid_data = ivalid;
  // Process the result
  // You need at least two data points, and if two the Pearson's correlation
  // takes one of -1, 0, or 1.
  if(ivalid < 2) return resultval;

  ResultPrecision mean1 = sum1 / ivalid;
  ResultPrecision mean2 = sum2 / ivalid;
  ResultPrecision tmp1 = ssum11 - 2.0 * mean1 * sum1 + ivalid * mean1 * mean1;
  ResultPrecision tmp2 = ssum22 - 2.0 * mean2 * sum2 + ivalid * mean2 * mean2;
  ResultPrecision denom = sqrt(tmp1 * tmp2);
  if(denom <= 0.0) return resultval;

  ResultPrecision numerat = csum12 - sum1 * mean2 - sum2 * mean1 + ivalid * mean1 * mean2;
  resultval = numerat / denom;

  return resultval;
}

}; // end namespace
#endif // defined _MATRIXALGORITHM_HH_

