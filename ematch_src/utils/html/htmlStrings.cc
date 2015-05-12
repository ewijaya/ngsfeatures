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
 *  Description: See header files.
 *
 *  Purpose:
 */

#include "utils/perlish/perlish.hh"
#include "htmlStrings.hh"

namespace cbrc{

  namespace htmlStrings{
    std::string equalWidthFontStyle( const unsigned int fontSize ){
      std::string retVal = 
	"<style type=\"text/css\">\n"
	"tt {\n"
	"	font-family: courier;\n"
	"}\n"
	"td {\n"
	"	font-family: courier;\n"
	"}\n"
	"caption {\n"
	"	font-family: helvetica, sans-serif;\n"
	"	font-size:";
      retVal += perlish::toString( fontSize );
      retVal += 
	"pt;\n"
	"text-align: center;\n"
	"}\n"
	"</style>\n";
      return retVal;
    }
    std::string tagString( const std::string& tagName, const std::string& contents, 
			   const std::string& tagExtraText ){
      std::string retVal = "<" + tagName;
      if( tagExtraText.size() ){ retVal += " "; retVal += tagExtraText; }
      retVal += ">";
      retVal += contents;
      retVal += "</";
      retVal += tagName;
      retVal += ">";
      return retVal;
    }
    std::string anchorHrefBasic( const std::string& nameBase, const std::string& contents, 
				 const std::string& nameExtension ){
      std::string anchorName( nameBase );
      if( nameExtension.size() )  anchorName += std::string( "#" ) + nameExtension;
      return(  tagString( "A", contents, std::string( "href=\"" ) + anchorName + endQuote )  );
    }
    std::string header( const std::string& title ){
      std::string retVal( "<HTML><HEAD>\n" );
      if( title.size() )  retVal += tagString( "TITLE", title );
      retVal += "</HEAD><BODY>\n";
      return retVal;
    }
    std::string end(){
      return std::string( "</HTML>\n" );
    }
  }; // end namespace htmlStrings
}; // end namespace cbrc
