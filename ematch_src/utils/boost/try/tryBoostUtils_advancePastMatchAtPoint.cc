/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 14:41:47 $
 *
 *  Purpose: try code involving BoostUtils_advancePastMatchAtPoint
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../boostUtils.hh"

namespace cbrc{
  void tryBoostUtils_advancePastMatchAtPoint(){
    boost::regex whiteSpaceRegex( "\\s+" );

    std::string text( ">  \ta fox\tran down the  year" );

    std::string::const_iterator curPos = text.begin() + 1;

    bool matchWasFound =
      boostUtils::advancePastMatchAtPoint( curPos, text.end(), whiteSpaceRegex );

    assert( matchWasFound );

    std::cout << "description is: \""
	      << std::string( curPos, std::string::const_iterator(text.end()) ) 
	      << "\"" << std::endl;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args" );
  
  args.dieIfUnusedArgs();
  cbrc::tryBoostUtils_advancePastMatchAtPoint();
  return 1;
}

