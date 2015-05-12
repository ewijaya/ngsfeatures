/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.7.15
 *  Last Modified: $Date: 2008/07/07 23:56:49 $
 *
 *  Purpose: try out FLSorted.
 */
#include <iostream>
#include "../FLSorted.hh"

namespace cbrc{
  void tryFLSorted(){
    std::vector< std::string > v;
    v.push_back( "fred" );
    v.push_back( "larry" );
    v.push_back( "ted" );
    v.push_back( "dan" );
 
    FLSorted<std::string> ids( v );
    std::cout << ids << std::endl;
    for( size_t i = 0; i < ids.size(); ++i ){
      std::string key = ids(i);
      std::cout << "key: " << key << " ids(key): " << ids(key) << " i: " << i << std::endl;
    }

    FLSorted<std::string> toInsertV;
    toInsertV = v;
    std::cout << " FLSorted with v inserted: " << toInsertV << std::endl;

    std::vector<std::string> w(v);
    w.push_back( "ted" );
    w.push_back( "terry" );
    w.push_back( "fred" );
    std::cout << "Construct ids2 using old vector but with duplicates\n";


    FLSorted<std::string> ids2(w); // create with duplicate on purpose.

    for( size_t i = 0; i < ids2.size(); ++i ){
      std::string key = ids2(i);
      std::cout << "key: " << key << " ids2(key): " << ids2(key) << " i: " << i << std::endl;
    }

    std::cout << "dumping ids2 with iterator\n";
    for( FLArray<std::string>::iterator it = ids2.begin(); it != ids2.end(); ++it ){
      std::cout << *it << std::endl;
    }
    std::cout << " dumped ids2 with iterator\n";
  }
}; // end namescape cbrc


int main(){
  cbrc::tryFLSorted();
  return 1;
}
