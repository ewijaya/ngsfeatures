/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.14
 *  Last Modified: $Date: 2008/01/18 15:10:50 $
 *
 *  Description: Functions for doing binary IO with std::string
 *               binary format is: size of string followed by contents.
 */
#ifndef _STRINGBINARYIO_HH
#define _STRINGBINARYIO_HH
#include <iostream>
#include <string>
#include "utils/perlish/perlish.hh"

namespace cbrc{

namespace stringBinaryIO{

/* ############################################################
 *           Function Declarations
 * ############################################################ */


/* ********** Binary input functions ************            *
 * each I/O method prints error message and exists on
 * failure, at some point an alternative class which 
 * throws exceptions might be created 
 */

// reads string S, including size information, from istream
void read( std::string& s, std::istream& is );

// read SIZE characters from istream IS into string S
void read( std::string& s, std::istream& is, const size_t& size );

// read stringVector, including size information, from istream IS
void read( std::vector<std::string>& stringVector, std::istream& is );


// write string S, including size information, to ostream OS
void write( const std::string& s, std::ostream& os );

// write contents of string S to ostream OS
void writeContentsOnly( const std::string& s, std::ostream& os );

// write stringVector, including size information, to istream OS
void write( const std::vector<std::string>& stringVector, std::ostream& os );


/* ############################################################
 *           Function Definitions
 *   I put these in a separate .cc file at one point, but it
 *   was inconvenient to have to link the .cc file, so now
 *   all function definitions are included below.
 * ############################################################ */


// read stringVector, including size information, from istream IS
inline void read( std::vector<std::string>& stringVector, std::istream& is ){
  
  if( is.peek() == EOF ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream at EOF\n" );
  }

  // read size
  size_t size;
  is.read( (char*) &size, sizeof(size) );

  stringVector.reserve( size );
  for( size_t i = 0; i < size; ++i ){
    std::string curString;
    read( curString, is );
    stringVector.push_back( curString );
  }
}

    

// read size and then contents of string in from istream
inline void read( std::string& s, std::istream& is ){
  
  if( is.peek() == EOF ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream at EOF\n" );
  }

  // read size
  size_t size;
  is.read( (char*) &size, sizeof(size) );

  if(  is.fail()  ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream read failed\n" );
  }

  // read string contents
  read( s, is, size );
}



// read size elements from istream
inline void read( std::string& s, std::istream& is, const size_t& size ){
  
  // allocate space
  if( size > s.max_size() ){
    perlish::die( __FILE__, __LINE__, "IO Error; string size read in too large" );
  }

  s.reserve( size );
  s.resize( size );

  // read contents
  is.read( (char*) &s[0], size*sizeof(s[0]) );

  if(  is.fail()  ){
    perlish::die( __FILE__, __LINE__, "IO Error; string istream read failed\n" );
  }
}


// write stringVector, including size information, to istream OS
inline void write( const std::vector<std::string>& stringVector, std::ostream& os ){

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


} // end namespace stringBinaryIO

} // end namespace cbrc
#endif // _STRINGBINARYIO_HH
