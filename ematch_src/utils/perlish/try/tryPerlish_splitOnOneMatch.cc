/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.4.8
 *  Last Modified: $Date: 2007/09/16 23:34:12 $
 *
 *  Purpose: try code involving perlish::splitOnOneMatch
 *
 *  Note: perlish::splitOnOneMatch is not really intended to be
 *        part of the perlish API. It would normally be called
 *        as a subroutine by perlish::split.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"

namespace cbrc{

void tryPerlishSplitOnOneMatch( const std::string& pattern,
				const std::string& expression,
				const bool& limitGiven,
				const int limit ){

  // print parameters and call splitOnOneMatch
  std::cout << "pattern is: " << pattern << std::endl;
  std::cout << "expression is: " << expression << std::endl;

  std::vector<std::string> fields;

  if( limitGiven ){
    std::cout << "limit is: " << limit << std::endl;
    fields = perlish::splitOnOneMatch( pattern, expression, limit );
  }
  else{
    fields = perlish::splitOnOneMatch( pattern, expression );
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
  args.set( limit, 1 );

  bool limitGiven = args.size();

  args.dieIfUnusedArgs();
  cbrc::tryPerlishSplitOnOneMatch( pattern, expression, limitGiven, limit );
  return 1;
}

