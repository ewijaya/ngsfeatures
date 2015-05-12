/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.5
 *  Last Modified: $Date: 2008/01/04 15:48:31 $
 *
 *  Description: Function to make a std::vector<std::string> list
 *               from a literal string list in the source code.
 *
 *  Usage: std::vector<std::string> 
 *         displayTypeList(  toVector( "black and white", "grayscale", "color" )  );
 * 
 *  Issues: Using preprocessor is elegant because it avoids the need for explicit
 *          sentinels -- but it puts "toVector" in the global namespace.
 *
 *  Purpose: Originally created to make ArgvParser
 *
 */
#ifndef _TOVECTOR_HH
#define _TOVECTOR_HH
#include <iostream>
#include <string>
#include <vector>
#include <cstdarg>
#define toVector( ... ) cbrc::_toVector( __VA_ARGS__, NULL )  // append sentinel!


namespace cbrc{

std::vector<std::string> _toVector( const char* cString, const void* dummySentinel ){
  std::vector<std::string> retVal;
  retVal.push_back( cString );
  return retVal;
}

// The end of the variable length argument list must end with a NULL sentinel
std::vector<std::string> _toVector( const char* cString0, ... ){
  std::vector<std::string> retVal;
  retVal.push_back( cString0 );

  /* ******* now process optional arguments ******* */
  va_list optionalArgs;
  va_start( optionalArgs, cString0 );

  const char* cString;
      
  while(   (cString = va_arg( optionalArgs, const char* ))   ){
    retVal.push_back( cString );
  }

  return retVal;
}

} // end namespace cbrc.
#endif // _TOVECTOR_HH
