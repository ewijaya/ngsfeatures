/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.4.8
 *  Last Modified: $Date: 2007/09/17 00:46:28 $
 *
 *  Purpose: try out perlish::split
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"

namespace cbrc{

void tryPerlish_split( const std::string& pattern,
		       const std::string& expression,
		       const bool& limitGiven,
		       const int limit ){

  // print parameters and call splitOnOneMatch
  std::cout << "pattern is: " << pattern << std::endl;
  std::cout << "expression is: " << expression << std::endl;

  std::vector<std::string> fields;

  if( limitGiven ){
    std::cout << "limit is: " << limit << std::endl;
    fields = perlish::split( pattern, expression, limit );
  }
  else{
    fields = perlish::split( pattern, expression );
  }


  // print result.
  for( size_t i = 0; i < fields.size(); ++i ){
    std::cout << "<" << fields[i] << "> ";
  }
  std::cout << std::endl;
} // end tryPerlishSplitOnOneMatch

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "pattern expression [limit]" );

  std::string pattern = args.shiftOrDie();

  std::string expression = args.shiftOrDie();

  int limit;
  bool limitGiven = (args.set( limit, 1 )).size();

  args.dieIfUnusedArgs();
  cbrc::tryPerlish_split( pattern, expression, limitGiven, limit );
  return 1;
}

