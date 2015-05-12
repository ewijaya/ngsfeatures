/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Creation Date: 2006/01/05
 *  Last Modified: $Date: 2009/05/15 04:18:49 $
 *  Description: Hand-coded version of getline for lame platforms such as solaris
 *               which lack this non-POSIX function.
 *
 *               The version is not well tested and probably is buggy. I will apologize
 *               in advance for the core dumps.
 *
 *               One known problem is the definition of fgets which uses "int" type
 *               for the size read in. This will limit the line size to no more than 2^31 bytes.
 *               Okay, that is a pretty big line.
 *
 *               To test on a platform which has its own getline, compile with -DJUST_TESTING_GETLINE
 */
#ifndef _GETLINE_HH_
#define _GETLINE_HH_
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#if defined(__sparc__) || defined(__APPLE__) || defined(JUST_TESTING_GETLINE)
namespace cbrc{

#ifdef JUST_TESTING_GETLINE
  inline ssize_t getlineTestVersion( char **lineptr, size_t *n, FILE *ifp ){
#else // really define getline
  inline ssize_t getline( char **lineptr, size_t *n, FILE *ifp ){
#endif // JUST_TESTING_GETLINE
    size_t& bufferSize = *n;
    char*& buffer = *lineptr;
    if( buffer == NULL ){ // buffer has not been pre-allocated by caller.
      bufferSize = 256;
      buffer = (char*) malloc( bufferSize );
      assert( buffer );
    }
    ssize_t numBytesRead = 0;
    const char notOverwrittenMark = 'A'; // anything by '\0' should do.
    char* bufferCurPos = buffer;
    buffer[ bufferSize - 1 ] = notOverwrittenMark;
    char* fgetsRetVal = fgets( bufferCurPos, bufferSize, ifp );
    if( fgetsRetVal == NULL ){ // no characters read from ifp
      *lineptr = NULL;
      return -1;
    }

    numBytesRead += strlen( bufferCurPos );
    if( buffer[ bufferSize -1 ] == notOverwrittenMark ){ // line fit into original buffer.
      return numBytesRead;
    }

    for( ;; ){ // keep reallocating with doubled size until the line fits.
      bufferSize *= 2;
      assert(  (buffer = (char*) realloc( buffer, bufferSize ))  );
      buffer[ bufferSize - 1 ] = notOverwrittenMark;
      bufferCurPos = buffer + numBytesRead;
      if(  fgets( bufferCurPos, bufferSize - numBytesRead, ifp ) == NULL  )  break;
      numBytesRead += strlen( bufferCurPos );
      if( buffer[ bufferSize -1 ] == notOverwrittenMark )  break;
    }
    return numBytesRead;
  }

}
#endif // defined(__sparc__) || defined(JUST_TESTING_GETLINE)
#endif // not defined _GETLINE_HH_
