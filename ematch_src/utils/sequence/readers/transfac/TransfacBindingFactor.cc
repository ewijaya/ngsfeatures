/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton 2006, All rights reserved.
 *  Creation Date: 2006.9.9
 *  Last Modified: $Date: 2006/09/18 04:41:45 $
 *  Description: See header file.
 */
#include "utils/perlish/perlish.hh"
#include "TransfacBindingFactor.hh"

namespace cbrc{

void TransfacBindingFactor::set( const std::string& transfacBFString ){
  std::vector<std::string> fields = perlish::split( transfacBFString, ";" );
  if( fields.size() != 3 ){
    std::cout << "Error: expected 3 \";\" separated fields in transfacBFString. String passed was: "
	      << transfacBFString << std::endl;
    exit( -1 );
  }
  _id = fields.at(0);
  _name = fields.at(1);
  perlish::removeLeadingWhiteSpace( _name );

  std::string speciesString = fields.at(2);
  const std::string speciesFieldMarker( "Species:" );
  std::string::size_type speciesPos = speciesString.find( speciesFieldMarker );
  if( speciesPos == std::string::npos ){
    std::cout << "Error: did not find speciesFieldMarker: \""
	      << speciesFieldMarker
	      << "\" in transfacBFString. String passed was: " 
	      << transfacBFString << std::endl;
      exit( -1 );
  }
  speciesString.erase( speciesPos, speciesFieldMarker.size() );
  perlish::removeLeadingWhiteSpace( speciesString );
  
  fields = perlish::split( speciesString, ',' );
  if( fields.size() != 2 ){
    std::cout << "Error: expected 2 \",\" separated fields in species part of transfacBFString.\n";
    exit( -1 );
  }
  _speciesCommonName = fields.at(0);
  std::string genusSpeciesName = fields.at(1);
  perlish::removeCharFromString( genusSpeciesName, '.' );
  perlish::removeLeadingWhiteSpace( genusSpeciesName );
  
  fields = perlish::split( genusSpeciesName, ' ' );
  if( fields.size() != 2 ){
    std::cout << "Error: expected genus and species names separated by a space but that is apparently not what I got\n";
    exit( -1 );
  }
  _genusName = fields[0];
  _speciesName = fields[1];
}


std::ostream& operator<<( std::ostream& os, const TransfacBindingFactor& tbf ){
    os << tbf.id() << "; ";
    os << tbf.name() << "; ";
    os << "Species: " << tbf.speciesCommonName() << ", ";
    os << tbf.genusName() << " " << tbf.speciesName() << ".";
    return os;
}

}; // end namespace cbrc

