/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.13
 *  Last Modified: $Date: 2009/09/13 07:16:21 $
 *
 *  Description: some functions useful when sorting
 *
 */
#ifndef SORTINGUTILS_HH_
#define SORTINGUTILS_HH_
#include <iostream>

namespace cbrc{


namespace sortingUtils{

  // Compare A to B, returning -1, 1, 0 if A is less than, greater than or equal to B respectively.
  // Reminiscent of Perl <=> operator, but return values conform to memcmp.
  template <typename T>
  int  cmp(  const T&  a,  const T&  b  ){
    return(
	   /**/ a < b     ?  -1
	   :    a > b     ?   1
	   : /* a = b */      0
	   );
  }
  
} // end namespace sortingUtils.

} // end namespace cbrc
#endif // defined SORTINGUTILS_HH_

