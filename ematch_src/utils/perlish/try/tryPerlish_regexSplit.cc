/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.4.8
 *  Last Modified: $Date: 2009/05/06 06:50:01 $
 *
 *  Purpose: try out perlish::split using boost::regex for pattern
 *
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../perlish.hh"


static std::string  arg_pattern;
static std::string  arg_expression;
static bool         arg_limitGiven;
static size_t       arg_limit;

namespace cbrc{

void tryPerlish_regexSplit(){

  // print parameters and call splitOnOneMatch
  std::cout << "pattern is: "    << arg_pattern    << std::endl;
  std::cout << "expression is: " << arg_expression << std::endl;

  const boost::regex regex( arg_pattern );

  std::vector<std::string> fields;

  if( arg_limitGiven ){
    fields = perlish::split( regex, arg_expression, arg_limit );
  }
  else{
    fields = perlish::split( regex, arg_expression );
  }


  // print result.
  for( size_t i = 0; i < fields.size(); ++i ){
    std::cout << "<" << fields[i] << "> ";
  }
  std::cout << std::endl;
} // end tryPerlishSplitOnOneMatch

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "pattern expression [limit]" );
  
  size_t curArg = 0;

  argvP.setOrDie( arg_pattern, ++curArg );

  argvP.setOrDie( arg_expression, ++curArg );

  arg_limitGiven  =  (argvP.set( arg_limit, ++curArg )).size();

  argvP.dieIfUnusedArgs();

  cbrc::tryPerlish_regexSplit();

  return 1;
}

