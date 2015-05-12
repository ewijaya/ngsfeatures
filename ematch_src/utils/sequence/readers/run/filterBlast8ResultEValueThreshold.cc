/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.8.31
 *
 *  Last Modified: $Date: 2006/11/23 14:53:24 $
 *  
 *  Description: filters Blast8Results by Evalue threshold
 *
 *  Purpose: Originally designed to filter out high sequence similarity pairs when training WoLF PSORT
 *
 */
#include <cmath>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/readers/Blast8ResultReader.hh"

namespace cbrc{

void filterBlast8ResultEValueThreshold( double eValueThreshold ){
  std::string line;
  for( ;; ){
    line = perlish::slurpLine( std::cin, Blast8ResultReader::isBlastCommentLine );
    if( !line.size() )  break;
    std::vector<std::string> fields = perlish::split( line, " \t\n" );
    Blast8Result b8r( fields );
    if( b8r.eValue() <= eValueThreshold ){
      std::cout << line << std::endl;
    }
  }
}

}; // end namespace cbrc

int main( int argc, const char** argv ){
  cbrc::ArgvParser args( argc, argv, "maxEValueThreshold [lg]" );
  double eValueThreshold = cbrc::perlish::toDouble( args[1] );
  if( args.has( "lg" ) ){
    eValueThreshold = pow( 2, eValueThreshold );
  }
  args.dieIfUnusedArgs();
  cbrc::filterBlast8ResultEValueThreshold( eValueThreshold );
  return 1;
}
