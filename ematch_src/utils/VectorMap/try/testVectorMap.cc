/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.7.23
 *
 *  Last cvs check in: $Date: 2004/07/22 20:25:56 $
 *  
 *  Purpose: test VectorMap.
 */
#include <iostream>
#include "../VectorMap.hh"

namespace cbrc{
  void testVectorMap(){
    VectorMap<std::string, double> vm;
    vm["fred"] = 6.4;
    std::cout << "vm[\"fred\"]: " << vm["fred"] << "\n";
    vm["ted"] = 3.2;
    vm["fred"] = 7.8;
    std::cout << "vm[\"ted\"]: " << vm["ted"] << "\n";
    std::cout << "vm[\"fred\"]: " << vm["fred"] << "\n";

    std::vector< std::pair<std::string, double> > mapAsVector = vm.getVector();
    for( size_t i = 0; i < mapAsVector.size(); ++i ){
      std::cout << "mapAsVector[" << i << "] ";
      std::cout << mapAsVector[i].first << " | " << mapAsVector[i].second << "  ";
    }
    std::cout << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::testVectorMap();
  return 1;
}

