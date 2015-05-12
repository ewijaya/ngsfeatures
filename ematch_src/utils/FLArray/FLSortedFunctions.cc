/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.6.8
 *
 *  Last Modified: $Date: 2005/07/29 17:08:19 $
 *  
 *  Description: See header file.
 *
 */
#include "utils/FLArray/FLSortedFunctions.hh"

namespace cbrc{

namespace FLSortedFunctions{

void read( FLSorted<std::string>& ss, std::istream& is ){
  size_t aSize;
  is.read( (char*) &aSize, sizeof(aSize) );
  std::vector<std::string> labels;
  size_t bufferSize = 1024;
  char* curStringBuffer = (char*) malloc( bufferSize );
  for( size_t i = 0; i < aSize; ++i ){
    size_t stringLen;
    is.read( (char*) &stringLen, sizeof(stringLen) );
    if( stringLen*sizeof(curStringBuffer[0]) > bufferSize ){
      bufferSize = (stringLen*sizeof(curStringBuffer[0])/256 + 1) * 256;
      curStringBuffer = (char*) realloc( curStringBuffer, bufferSize );
    }
    is.read( (char*) curStringBuffer, stringLen*sizeof(curStringBuffer[0]) );
    labels.push_back( std::string( curStringBuffer, 0, stringLen) );
  }
  free( curStringBuffer );
  ss.assign( labels.begin(), labels.end() );
}

void write( const FLSorted<std::string>& ss, std::ostream& os ){
  size_t aSize = ss.size();
  os.write( (char*) &aSize, sizeof(aSize) );
  for( size_t i = 0; i < aSize; ++i ){
      size_t stringLen = ss[i].size();
      os.write( (char*) &stringLen, sizeof(stringLen) );
      os.write( (char*) &ss[i][0], stringLen*sizeof(ss[i][0]) );
  }
}

}; // end namespace FLSortedFunctions.
}; // end namespace cbrc.
