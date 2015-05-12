/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2004.6.25
 *  Last Modified: $Date: 2005/12/17 08:57:38 $
 *  
 *  Description: Specific type wrapper for map. the key is an ordered pair of strings.
 *                                              the value is a double.
 *
 *  Purpose: 1. Make code cleaner by packaging some of the make_pair operations
 *           2. Decouple code from details of stl map.
 *
 *  Usage: read with              table( string0, string1 ).
 *         write double x with    table( string0, string1, x ).
 *         check presence with    table.has( string0, string1 ).
 *
 */

#ifndef _STRINGSTRINGDOUBLEMAP_HH_
#define _STRINGSTRINGDOUBLEMAP_HH_
#include <map>

namespace cbrc{


typedef std::map< std::pair<std::string, std::string>, double> StringStringDoubleMapT;


class StringStringDoubleMap{
public:
  bool has( const std::string& s0, const std::string& s1 ){
    return(  table.find( make_pair(s0, s1) ) != table.end() );
  }
  double operator()( const std::string& s0, const std::string& s1 ){
    StringStringDoubleMapT::const_iterator entry = table.find( make_pair(s0, s1) );
    if( entry != table.end() )  return entry->second;
    std::cout << __FILE__ << ":" << __LINE__ << " could not find strings: " << s0 << "," << s1 << std::endl;
    exit( -1 );
  }
  void operator()( const std::string& s0, const std::string& s1, const double x ){
    std::pair< std::string, std::string > p = make_pair( s0, s1 );
    StringStringDoubleMapT::iterator entry = table.find( p );
    if( entry == table.end() ){
      std::cout << "inserting pair with val: " << x << std::endl;
      table.insert( make_pair(p, x) );
    }
    else{
      entry->second = x;
    }
  }
  friend void testStringStringDoubleMap();
  friend std::ostream& operator<<( std::ostream& os, const StringStringDoubleMap& );
private:
  StringStringDoubleMapT table;
};

std::ostream& operator<<( std::ostream& os, const StringStringDoubleMap& ssdm ){
  for( StringStringDoubleMapT::const_iterator i = ssdm.table.begin(); i != ssdm.table.end(); ++i ){
    os << i->first.first << ", " << i->first.second << ": " << i->second << std::endl;
  }
  return os;
}
  
}; // end namespace cbrc
#endif // defined _STRINGSTRINGDOUBLEMAP_HH_

