/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.16
 *  Last Modified: $Date: 2008/11/16 02:48:17 $
 *
 *  Purpose: try code involving StringReplaceAll
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../stringUtils.hh"

namespace cbrc{
  void tryStringReplaceAll( std::string s,
			    const std::string& oldSubstring,
			    const std::string& newSubstring ){

    size_t numReplacementsDone = 
      stringUtils::stringReplaceAll( s, oldSubstring, newSubstring );

    std::cout << "result of " << numReplacementsDone << " replacements is:\n"
	      << s << std::endl;
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "string oldSubstring newSubstring" );
  size_t curArg = 0;
  
  std::string s;
  args.setOrDie( s, ++curArg );

  std::string oldSubstring;
  args.setOrDie( oldSubstring, ++curArg );

  std::string newSubstring;
  args.setOrDie( newSubstring, ++curArg );

  args.dieIfUnusedArgs();
  cbrc::tryStringReplaceAll( s, oldSubstring, newSubstring );
  return 1;
}

