/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.16
 *  Last Modified: $Date: 2009/06/02 08:37:33 $
 *
 *  Description: This file defines constants common to fasta format files
 *
 */
#ifndef FASTAFMT_HH_
#define FASTAFMT_HH_
#include <iostream>
#include <boost/regex.hpp>

namespace cbrc{

namespace fastafmt{

  // The record start marker is of type char. Some functions rely on this
  // by using peek() on istream's to see it coming.
  inline const char& recordStartMarker() {
    const static char  _recordStartMarkerChar( '>' );
    return _recordStartMarkerChar;
  }

  inline const std::string::size_type& recordStartMarkerLength() {
    const static std::string::size_type  _recordStartMarkerLength( 1 );
    return _recordStartMarkerLength;
  }

  // regex to match initial ">" and any following white space
  inline const boost::regex& recordStartRegex() {
    const static boost::regex  _recordStartRegex( "^>\\s*" );
    return  _recordStartRegex;
  }

  // optional sequence terminating character
  inline const char& seqTerminationMarker() {
    static const char  _recordMarker( '*' );
    return _recordMarker;
  }

} // end namescpe fastafmt

} // end namespace cbrc
#endif // FASTAFMT_HH_
