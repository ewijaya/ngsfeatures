/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2004.12.3
 *  Last Modified: $Date: 2006/05/20 15:42:00 $
 *  
 *  Description: Functions relating to rank correlation.
 *
 *  Purpose: Orginally made for Cell Montage.
 */

#ifndef _RANKCORRELATION_HH_
#define _RANKCORRELATION_HH_

namespace cbrc{

namespace rankCorrelationFunctions{
  inline double rankCorrelation( const unsigned int n,
				 const double d2 /* sum of squared rank differences */ ){
    double denominator = n * ( n * n - 1 );
    return( 1 - 6.0 * d2 / denominator );
  }
  double significance( const unsigned int n, const double rankCorrelation );
}; // end namespace rankCorrelationFunctions

}; // end namespace cbrc
#endif // defined _RANKCORRELATION_HH_

