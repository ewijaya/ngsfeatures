/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.7.28
 *
 *  Last Modified: $Date: 2005/10/25 06:51:42 $
 *  
 *  Description: Functions related to the Spearman rank correlation coefficient.
 *
 */

namespace cbrc{

  namespace mathUtils{

    // for numerical reasons n is type double, but in principle it should be a positive integer.
    inline double rankCorrelation( const double n, const double d2 ){
      return( 1 - 6.0 * d2 / ( n * (n*n - 1) ) );
    }

    inline double rankCorrelationTStatistic( const double n, const double rankCorrelation ){
      return(  rankCorrelation * sqrt( (n-2) / (1-rankCorrelation*rankCorrelation) )  );
    }
  }; // end namespace mathUtils.
}; // end namespace cbrc
