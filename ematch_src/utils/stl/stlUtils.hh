/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.5
 *  Last Modified: $Date: 2009/06/25 02:30:47 $
 *
 *  Description: Some convenience functions for working with the stl.
 *
 */
#ifndef _STLUTILS_HH_
#define _STLUTILS_HH_
#include <algorithm>
#include <iostream>
#include <vector>


namespace cbrc{


namespace stlUtils{
  
  typedef  unsigned char  uchar;

  inline uchar  max(  const std::vector<uchar>& vec  ){
    const std::vector<uchar>::const_iterator where
      =  std::max_element( vec.begin(), vec.end() );
    return  *where;
  }

  template <typename T, typename F>
  void transformAll( T& container, F op ){
    std::transform( container.begin(), container.end(), container.begin(), op );
  }


  template<typename T1, typename T2>
  bool pairSecondLess( const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2 ){
    return( p1.second < p2.second );
  }

} // end namespace stlUtils

} // end namespace cbrc
#endif // defined _STLUTILS_HH_
