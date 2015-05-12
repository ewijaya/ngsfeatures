/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004-2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.9.11
 *  Last Modified: $Date: 2007/05/14 03:04:33 $
 *  
 *  Purpose: try out FLSorted2TypePairSet.
 */
#include <iostream>
#include <vector>
#include "utils/argvParsing/ArgvParser.hh"
#include "../FLSorted2TypePairSet.hh"

namespace cbrc{
  void tryFLSorted2TypePairSet(){
    std::string fred( "fred" );
    std::string dan( "dan" );
    std::string ted( "ted" );

    std::cout << "String set is: {\"fred\", \"dan\", \"ted\"}\n";
    std::vector<std::string> lhs;
    lhs.push_back( fred );
    lhs.push_back( dan );
    lhs.push_back( ted );


    // Assign keys to strings    
    std::vector< std::vector<std::string> > rhs;

    // Keys for "fred"
    std::vector<std::string> fredKeys;
    fredKeys.push_back( fred );
    fredKeys.push_back( ted );
    rhs.push_back( fredKeys );

    // Keys for "dan"
    std::vector<std::string> danKeys;
    rhs.push_back( danKeys );

    // "Keys for "ted"
    std::vector<std::string> tedKeys;
    tedKeys.push_back( ted );
    tedKeys.push_back( fred );
    tedKeys.push_back( dan );
    rhs.push_back( tedKeys );

    for( size_t i = 0; i < rhs.size(); ++i ){
      std::cout << "Keys for string: " << lhs[i] << "--> {";
      for( size_t j = 0; j < rhs[i].size(); ++j ){
	std::cout << " " << rhs[i][j] << "";
      }
      std::cout << " }" << std::endl;
    }

    std::cout << "\nSame information printed with FLSorted2TypePairSet operator<<\n";
    FLSorted2TypePairSet<std::string,std::string> pairSet( lhs, rhs );
    std::cout << pairSet;
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage: no args" );
  args.dieIfUnusedArgs();
  cbrc::tryFLSorted2TypePairSet();
  return 1;
}

