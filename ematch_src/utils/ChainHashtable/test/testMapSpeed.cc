/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.6.25
 *  Last Modified: $Date: 2008/12/11 02:31:03 $
 *  
 *  Purpose: test map speed.
 */
#include <iostream>
#include <map>
#include "utils/perlish/perlish.hh"



namespace cbrc{

  void testMapSpeed(){
    std::vector<std::string> lines = perlish::slurpLines();
    std::map< std::string, size_t> skh;
    for( size_t i = 0; i < lines.size(); ++i ){
      skh[ lines[i] ] = i;
    }
    srand48( getpid() );
    random_shuffle( lines.begin(), lines.end() );
#ifdef CBRC_DEBUG
    for( size_t i = 0; i < lines.size(); ++i ){
      std::cout << lines[i] << std::endl;
    }
#endif // defined CBRC_DEBUG
    for( size_t trial = 0; trial < 100; ++trial ){
      std::cout << "trial: " << trial << std::endl;
      size_t lineNumSum = 0;
      for( size_t i = 0; i < lines.size(); ++i ){
	//	std::pair<std::string,std::string> p = make_pair( lines[i], "fred" );
	lineNumSum += skh[ lines[i] ];
      }
      std::cout << "line number sum: " << lineNumSum << std::endl;
    }
  }
}; // end namescape cbrc



int main( int argc, char** argv ){
  std::string usage( "Usage: " );
  usage += argv[0];
  usage += " No arguments. Just pump lines into stdin.\n";
  if( argc > 1 ){
    std::cout << usage;
    exit( 1 );
  }
  cbrc::testMapSpeed();
  return 1;
}

