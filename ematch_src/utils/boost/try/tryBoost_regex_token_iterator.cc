/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 14:03:08 $
 *
 *  Purpose: try out code involving boost::regex.regex_search
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include <boost/regex.hpp>

namespace cbrc{
  void tryBoost_regex_token_iterator(){
    boost::regex whiteSpaceRegex( "\\s+" );

    std::string text( "a fox\tran down the  year\n" );


    boost::sregex_token_iterator curPart( text.begin(), text.end(),
				    whiteSpaceRegex,
				    -1 // iterate of non-matching parts
				    );
    boost::sregex_token_iterator end;

    std::cout << "fields are:\n";

    for( /* no init */; curPart != end; ++curPart ){
      std::cout << *curPart << std::endl;
    }
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryBoost_regex_token_iterator();
  return 1;
}

