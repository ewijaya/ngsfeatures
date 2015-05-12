/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2005.12.28
 *  Last Modified: $Date: 2006/01/05 10:15:18 $
 *
 *  Description: try out getline function.
 *
 */
#include <iostream>
#include <stdio.h>
#include "utils/argvParsing/ArgvParser.hh"
#ifdef CBRC_ARCHITECTURE_SOLARIS
#include "../getline.hh"
#endif // defined CBRC_ARCHITECTURE_SOLARIS
#ifdef JUST_TESTING_GETLINE
#include "../getline.hh"
#endif // defined JUST_TESTING_GETLINE

namespace cbrc{

  void dumpLines( FILE* ifp ){
    size_t bufferSize;
    char* bufferPtr = NULL;
    char** bufferPtrPtr = &bufferPtr;

    std::string retVal;
    ssize_t getlineRetVal;

    // Achtung this is not std::getline!
#ifdef JUST_TESTING_GETLINE
    while(  (getlineRetVal = getlineTestVersion( bufferPtrPtr, &bufferSize, ifp )) != -1  ){
#else // really use getline
    while(  (getlineRetVal = getline( bufferPtrPtr, &bufferSize, ifp )) != -1  ){
#endif // JUST_TESTING_GETLINE
      std::cout << "size: " << getlineRetVal << " line: " << bufferPtr;
    }
    free( *bufferPtrPtr );
  }

}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args, pipe text input into standard in" );
  args.dieIfUnusedArgs();
  cbrc::dumpLines( stdin );
  return 1;
}
