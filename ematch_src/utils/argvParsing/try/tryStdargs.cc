/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.29
 *  Last Modified: $Date: 2008/04/06 22:25:29 $
 *
 *  Description: This is the fruit of my heroic efforts to use variadic functions
 *               without explicit use of sentinels or count variables by the caller.
 *
 *  Purpose: I made this while exploring the options available to support combined
 *           binary flags (like the "-nq" of cvs -nq update) for ArgvParser.
 *
  */
#include <iostream>
#include <cstdarg>
#include <cassert>
#include "utils/argvParsing/ArgvParser.hh"
#define tryStdarg( ... ) _tryStdarg( __VA_ARGS__, NULL )  // append sentinel!



namespace cbrc{
  std::vector<std::string> givenFlags;

  bool isFlagPresent( const char* const& flagName ){

    for( size_t i = 0; i < givenFlags.size(); ++i ){
      if( flagName == givenFlags[i] ){
	return true;
      }
    }
    return false;
  }
    


void _tryStdarg( bool* const flagPtr, const char* const& flagName,
		 void* dummySentinel ){
  *flagPtr = isFlagPresent( flagName );
  assert( !dummySentinel );
}


// The end of the variable length argument list must end with a NULL sentinel
void _tryStdarg( bool* const flagPtr0, const char* const& flagName0,
		 bool* const flagPtr1, const char* const& flagName1,
		  ...
		  /* void* sentinel */ ){

  // processed fixed arguments.
  *flagPtr0 = isFlagPresent( flagName0 );
  *flagPtr1 = isFlagPresent( flagName1 );

    
  /* ******* now process optional arguments ******* */
  va_list optionalArgs;
  va_start( optionalArgs, flagName1 );

  bool* flagPtr;
      
  while(   (flagPtr = va_arg( optionalArgs, bool* ))   ){

    const char* flagName = va_arg( optionalArgs, const char* );

    if( !flagName ){
      std::cerr << "Programming error, tryStdarg should be called with an even number of arguments\n";
      exit( -1 );
    }

    *flagPtr = isFlagPresent( flagName );
  }
} // end _tryStdarg

} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "" );

  cbrc::givenFlags = args.argsAsStrings();


  bool justPrint;
  bool xOption;
  bool verbose;
  bool compress;

  cbrc::tryStdarg( 
		   &justPrint, "-n",
		   &xOption,   "-x",
		   &verbose,   "-v",
		   &compress,  "-z" );

  std::cout << "justPrint " << (justPrint ? "true\n" : "false\n");
  std::cout << "xOption   " << (xOption   ? "true\n" : "false\n");
  std::cout << "verbose   " << (verbose   ? "true\n" : "false\n");
  std::cout << "compress  " << (compress  ? "true\n" : "false\n");

  return 1;
}

