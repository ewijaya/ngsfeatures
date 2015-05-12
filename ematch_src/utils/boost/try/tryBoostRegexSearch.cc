/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 14:04:24 $
 *
 *  Purpose: try out code involving boost::regex.regex_search
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include <boost/regex.hpp>

namespace cbrc{
  void tryBoostRegexSearch(){
    boost::regex whiteSpaceRegex( "\\s+" );

    std::string text( ">  \ta fox\tran down the  year" );

    std::string::const_iterator curPos = text.begin() + 1;

    std::string::const_iterator textEnd = text.end();

    boost::match_results<std::string::const_iterator> whatMatched;

    boost::regex_search( 
			curPos, textEnd, 
			whatMatched,
			whiteSpaceRegex,
			boost::match_default | boost::match_continuous
			 );

    curPos += whatMatched.length(0);

    std::cout << "description is: \"" << std::string( curPos, textEnd ) << "\"" << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryBoostRegexSearch();
  return 1;
}

