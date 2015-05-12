/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.16
 *  Last Modified: $Date: 2009/05/10 08:10:45 $
 *
 *  Purpose: try out StringUtils methods startsWith and endsWith
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../stringUtils.hh"


struct argsT{
  std::string text;
  std::string query;
} args;


namespace cbrc{

  void tryStringUtils_startsEndsWith(){

    /* ***** try out startsWith ***** */
    bool isPrefix = stringUtils::startsWith( args.text, args.query );

    std::cout << "\"" << args.query << "\" is"
	      << ( isPrefix ? "" : " not" )
	      << " a prefix of "
	      << "\"" << args.text << "\"\n";


    /* ***** try out endsWith ***** */
    bool isSuffix = stringUtils::endsWith( args.text, args.query );

    std::cout << "\"" << args.query << "\" is"
	      << ( isSuffix ? "" : " not" )
	      << " a suffix of "
	      << "\"" << args.text << "\"\n";
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argP( argc, argv, "text query" );

  argP.setOrDie( args.text,  1 );
  argP.setOrDie( args.query, 2 );
  
  argP.dieIfUnusedArgs();
  cbrc::tryStringUtils_startsEndsWith();
  return 1;
}

