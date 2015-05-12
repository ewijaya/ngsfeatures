/*
 *  Author: Please set CBRC_AUTHOR environment variable
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.6.25
 *
 *  Last Modified: 2004.6.25
 *  
 *  Purpose: test StringKeyHashtable speed.
 */
#include <iostream>
#include "utils/perlish/perlish.hh"
#include "../StringKeyHashtable.hh"

namespace cbrc{
  void testStringKeyHashtableSpeed(){
    std::vector<std::string> lines = perlish::slurpLines();
    //    StringKeyHashtable<size_t> skh( static_cast<size_t>(lines.size() * 1.3) );
    StringKeyHashtable<size_t> skh;
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
  cbrc::testStringKeyHashtableSpeed();
  return 1;
}

