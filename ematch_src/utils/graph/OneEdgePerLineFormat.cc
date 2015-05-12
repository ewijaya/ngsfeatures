/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.2.2
 *  Last Modified: $Date: 2008/03/18 03:15:44 $
 *  Description: See header file.
 */

#include "utils/perlish/perlish.hh"
#include "utils/SetWrapper/SetWrapper.hh"
#include "OneEdgePerLineFormat.hh"

namespace cbrc{

nodeIndexT OneEdgePerLineFormat::numNodesInStream( std::istream& is ){
  SetWrapper<std::string> nodeNames;

  for( std::string line = perlish::slurpLine( is ); line.size() > 1; line = perlish::slurpLine( is ) ){

    std::vector< std::string > fields
      = perlish::split( OneEdgePerLineFormat::inputDelimiter(), line );

    if( fields.size() > 2 ){
      std::cout << "Error: expected 1 or 2 fields per line: " << line << std::endl;
      exit( -1 );
    }
    
    for( size_t i = 0; i < fields.size(); ++i ){
      nodeNames.insert( fields[i] );
    }
  }

  return nodeNames.size();
}


}; // end namespace
