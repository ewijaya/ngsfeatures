/*
 *  Author: "Paul B. Horton"
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.12.16
 *
 *  Last Modified: 2004.12.16
 *  
 *  Description: See header files.
 *
 *  Purpose:
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include "../pairPredicate.hh"

namespace cbrc{

  bool intSmaller(  const int& i1, const int& i2 ){
    return( i1 < i2 );
  }
};


int main( int argc, const char* argv[] ){  
  std::vector< std::pair<char, int> > v;
  v.push_back( std::make_pair( 'a', 87 ) );
  v.push_back( std::make_pair( 'c', 80 ) );
  v.push_back( std::make_pair( 'b', 90 ) );
  cbrc::pairPredicate::secondPredicate<char, int> pred( cbrc::intSmaller );
  std::sort( v.begin(), v.end(), pred );

  std::cout << "Array v:" << std::endl;
  for( size_t i = 0; i < v.size(); ++i ){
    if( i ) std::cout << " ";
    std::cout << v[i].first << "," << v[i].second;
  }
  std::cout << std::endl;

  return 1;
}

