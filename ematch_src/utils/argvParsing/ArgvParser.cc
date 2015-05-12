/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2005,2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.6.10
 *  Last Modified: $Date: 2008/05/19 09:28:42 $
 *  
 *  Description: See header files.
 *
 */
#include <cstdlib>
#include "utils/perlish/perlish.hh"
#include "ArgvParser.hh"

namespace cbrc{



  bool ArgvParser::yesNoFlag( const std::string opt ){
    for( int i = 1; i < _argc-1; ++i ){
      if(  !strcmp( opt.c_str(), _argv[i] )  ){
	argUsed[i] = argUsed[i+1] = true;
	if( _argv[i+1][0] == 'y' ) return true;
	if( _argv[i+1][0] == 'n' ) return false;
	errorOut << "\nError: expected \'y\' or \'n\' for yes/no option: " << opt << std::endl;
	die();
      }
    }
    errorOut << "\nError: mandatory option: \"" << opt << "\" missing.\n";
    die();
    return false; // just to silence compiler warning.
  }



  std::string ArgvParser::shiftUsedOk(){
    if( argc() < 2 ) return std::string("");
    std::string retVal( _argv[1] );
    for( int i = 1; i < argc()-1; ++i ){
      _argv[i] = _argv[i+1];
      argUsed[i] = argUsed[i+1];
    }
    --_argc;
    return retVal;
  }
  
  std::string ArgvParser::shiftMandatoryUsedOk(){
    if( argc() < 2 ) die( "Error: expected more command line arguments" );
    std::string retVal( _argv[1] );
    for( int i = 1; i < argc()-1; ++i ){
      _argv[i] = _argv[i+1];
      argUsed[i] = argUsed[i+1];
    }
    --_argc;
    return retVal;
  }


  bool ArgvParser::isLongSwitch( const std::string& s ) const{
    static const std::string longSwitchPrefix( "--" );
    if( s.size() < 3 )  return false;
    return( s.substr(0, 2) == longSwitchPrefix );
  }



  void ArgvParser::_assertHasOpt( const std::vector<std::string>& flags ){

    if(  hasOpt( flags )  )    return;

    errorOut << "Command line parsing error: expected option marked by one of these flags:\n\n    "
	     << joinStringList( "\n    ", flags ) << "\n\n";

    if(   hasFlag( flags )   ){
      std::string matchingFlag = getFlag( flags );
      errorOut << "\n\nflag: \""
	       << matchingFlag
	       << "\" found but has no option value following it\n";
    }
    die();
  }


    
}; // end namespace cbrc
