/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.11.26
 *
 *  Last Modified: 2004.11.26
 *  
 *  Description: html related string generator.
 *
 */

#ifndef _HTMLSTRINGS_HH_
#define _HTMLSTRINGS_HH_

namespace cbrc{

namespace htmlStrings{
  const std::string endQuote( "\"" );
  std::string equalWidthFontStyle( const unsigned int fontSize = 14 );
  std::string header( const std::string& title = "" );
  std::string end();
  // example tagString( "FONT", "fire engine", "color=\"red\"" )
  std::string tagString( const std::string& tagName, const std::string& contents, 
			 const std::string& tagExtraText = "" );
  inline std::string anchorName( const std::string& name, const std::string& contents, 
			  const std::string& extraText = "" ){
    return tagString( "A", contents, std::string( "name=\"" ) + name + endQuote + extraText );
  }
  // add link to nameBase#nameExtension
  std::string anchorHrefBasic( const std::string& nameBase, const std::string& contents, 
			       const std::string& nameExtension = "" );
};

}; // end namespace cbrc
#endif // defined _HTMLSTRINGS_HH_

