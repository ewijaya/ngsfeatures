/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2003.12.24
 *  Last Modified: $Date: 2005/12/17 08:58:57 $
 *  
 *  Purpose: Provide a wrapper for the stl set class. This provides an interface which I like better and
 *  will allow me to switch set classes later without being commited to the stl set interface.
 *
 */

#ifndef _SETWRAPPER_HH_
#define _SETWRAPPER_HH_
#include <set>

namespace cbrc{

template <typename T>
class SetWrapper{
public:
  friend void testSetWrapper();
  void insert( const T& elem ){ s.insert( elem ); }
  bool operator()( const T& elem ) const{ return( !(s.find(elem) == s.end() ) ); }
  size_t size() const{ return s.size(); }
private:
  std::set<T> s;
};

}; // end namespace
#endif // defined _SETWRAPPER_HH_

