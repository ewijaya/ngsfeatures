/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.9.13
 *  Last Modified: $Date: 2009/09/13 23:50:33 $
 *
 *  Description: Some general utils for computation with pointers
 *
 */
#ifndef PTRUTILS_HH_
#define PTRUTILS_HH_
#include <iostream>

namespace cbrc{

namespace ptrUtils{

  /* average pointers together in way which should avoid overflow even
   * if the quantity (p1 + p2) would overflow.
   *
   * return value is floor( p1 + p2 / 2 )
   */

  template <typename T>
  inline
  T*  average(   T* const&  p1,  T* const&  p2   ){

    return   ( p1 >= p2 )  ?  p2 + (p1 - p2)/2  :  p1 + (p2 - p1)/2;
  }

}

} // end namespace cbrc
#endif // PTRUTILS_HH_
