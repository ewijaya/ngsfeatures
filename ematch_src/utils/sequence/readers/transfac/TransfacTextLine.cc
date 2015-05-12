/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.11
 *  Last Modified: $Date: 2006/09/12 01:13:47 $
 *  Description: See header file.
 */
#include "utils/perlish/perlish.hh"
#include "TransfacTextLine.hh"


namespace cbrc{

// Set TransfacTextLine object by parsing a transfac style line string.
// The string should have a two character label, optionally
// followed by white space and some content.
// If the line has not content,  _content is set to the empy string.
void TransfacTextLine::set( const std::string& line ){
  std::vector<std::string> fields = perlish::split( line, perlish::spaceTabString(), 2 );
  if( !fields.size() ){
    std::cout << __FILE__ << ":" << __LINE__
	      << " Error: expected some text on transfac line.\n"
	      << "String recieved:\"" << line << "\".\n";
    exit( -1 );
  }
  _label = fields[0];
  if( _label.size() != 2 ){
    std::cout << __FILE__ << ":" << __LINE__
	      << " Error: expected length 2 label string at start of tranfac line.\n"
	      << "String recieved:\"" << line << "\".\n";
    exit( -1 );
  }
  _content = (fields.size() > 1 ) ? fields[1] : "";
}

std::ostream& operator<<( std::ostream& os, const TransfacTextLine& ttl ){
  if( ttl.empty() ){
    os << "EMPTY";
  }else{
    os << "label: " << ttl.label() << " content: " << ttl.content() << std::endl;
  }
  return os;
}

}; // end namespace cbrc

