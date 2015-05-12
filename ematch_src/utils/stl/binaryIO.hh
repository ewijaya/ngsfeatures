/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.14
 *  Last Modified: $Date: 2009/05/12 02:32:19 $
 *
 *  Description: Functions for doing binary IO with std::string
 *               binary format is: size of string followed by contents.
 */
#ifndef _BINARYIO_HH
#define _BINARYIO_HH
#include <iostream>
#include <string>
#include "utils/stl/stlTypes.hh"
#include "utils/perlish/perlish.hh"

namespace cbrc{

namespace binaryIO{

/* ############################################################
 *           Function Declarations
 * ############################################################ */


/* ********** Binary input functions ************            *
 * each I/O method prints error message and exists on
 * failure, at some point an alternative class which 
 * throws exceptions might be created 
 */

// reads string S, including size information, from istream
void read( std::string& s, std::istream& iStream );

// read SIZE characters from istream iStream into string S
void read( std::string& s, std::istream& iStream, const size_t& size );

// read length s.size() string from iStream, and confirm it equals S
inline void traceOrDie( const std::string& s, std::istream& iStream );

// read stringVector, including size information, from iStream
void read( stringVecT& stringVector, std::istream& iStream );


// write string S, including size information, to ostream OS
void write( const std::string& s, std::ostream& os );

// write contents of string S to ostream OS
void writeContentsOnly( const std::string& s, std::ostream& os );

// write stringVector, including size information, to istream OS
void write( const stringVecT& stringVector, std::ostream& os );



/* ############################################################
 *           Function Definitions
 *   I put these in a separate .cc file at one point, but it
 *   was inconvenient to have to link the .cc file, so now
 *   all function definitions are included below.
 * ############################################################ */


// read stringVector, including size information, from istream IS
inline void read( stringVecT& stringVector, std::istream& iStream ){
  
  if( iStream.peek() == EOF ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream at EOF\n" );
  }

  // read size
  size_t size;
  iStream.read( (char*) &size, sizeof(size) );

  stringVector.reserve( size );
  for( size_t i = 0; i < size; ++i ){
    std::string curString;
    read( curString, iStream );
    stringVector.push_back( curString );
  }
}

    

// read size and then contents of string in from istream
inline void read( std::string& s, std::istream& iStream ){
  
  if( iStream.peek() == EOF ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream at EOF\n" );
  }

  // read size
  size_t size;
  iStream.read( (char*) &size, sizeof(size) );

  if(  iStream.fail()  ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream read failed\n" );
  }

  // read string contents
  read( s, iStream, size );
}



// read size elements from istream
inline void read( std::string& s, std::istream& iStream, const size_t& size ){
  
  // allocate space
  if( size > s.max_size() ){
    perlish::die( __FILE__, __LINE__, "IO Error; string size read in too large" );
  }

  s.reserve( size );
  s.resize( size );

  // read contents
  iStream.read( (char*) &s[0], size*sizeof(s[0]) );

  if(  iStream.fail()  ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream read failed\n" );
  }
}


// read size elements from istream
inline void traceOrDie( const std::string& s, std::istream& iStream ){
  
  // allocate space
  std::string tmp;

  tmp.resize( s.size() );

  // read contents
  iStream.read(   (char*) &tmp[0],  s.size() * sizeof( tmp[0] )   );

  GDB_ASSERTF(  !iStream.fail(),
	       "Input error, while trying to trace over string \"%s\"", s.c_str()  );

  GDB_ASSERTF( tmp == s,
	       "Input error, expected to trace over string \"%s\", but read \"%s\"",
	       s.c_str(), tmp.c_str() );
}


// write stringVector, including size information, to istream OS
inline void write( const stringVecT& stringVector, std::ostream& os ){

  // write number of strings
  size_t size = stringVector.size();
  os.write(  (char*) &size,  sizeof(size)  );

  for( size_t i = 0; i < stringVector.size(); ++i ){
    write( stringVector[i], os );
  }
}
  


// write string size and contents of string to ostream
inline void write( const std::string& s, std::ostream& os ){

  // write element count
  size_t size = s.size();
  os.write(  (char*) &size,  sizeof(size)  );

  writeContentsOnly( s, os );
}


inline void writeContentsOnly( const std::string& s, std::ostream& os ){
    os.write(  (char*) &s[0],  s.size() * sizeof( s[0] )  );
    if( os.fail() ){
      perlish::die( __FILE__, __LINE__, "IO Error: string write failed\n" );
    }
}


} // end namespace binaryIO

} // end namespace cbrc
#endif // _BINARYIO_HH
