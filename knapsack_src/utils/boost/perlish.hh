/*
 *  Author: Paul Horton
 *  Creation Date: 2003.4.10
 *  Last Modified: $Date: 2009/06/22 14:23:19 $
 *  Copyright (C) 2003-2007 Paul Horton, All rights reserved.
 *  
 *  Description: Collection of useful functions that seem perlish.
 *               Generally speaking call by value is used in these routines; sacrificing
 *               speed for robustness against memory use bugs.
 *
 */
#ifndef _PERLISH_HH_
#define _PERLISH_HH_
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <boost/regex.hpp>
#include "utils/gdb/gdbUtils.hh"
#include "utils/stl/stlTypes.hh"
#include "utils/boost/boostUtils.hh"

namespace cbrc {
  namespace perlish{
    const std::string nullString("");

    // empty line or starts with #
    bool isSharpCommentOrEmpty( const std::string& s );
    bool alwaysFalse( const std::string& s );

    // Slurp line of file and assign it to LINE.
    // Ignores lines if ignoreLine(line) returns true.
    // returns false if no more non-comment lines are left to read.
    bool slurpLine(  std::string&   line, 
		     std::istream&  is = std::cin, 
		     bool ignoreLine(const std::string&) = isSharpCommentOrEmpty );

    // Slurp line of file. Ignores lines if ignoreLine(line) returns true.
    // returns empty string if no more non-comment lines are left to read.
    std::string slurpLine( std::istream& is = std::cin, 
			   bool ignoreLine(const std::string&) = isSharpCommentOrEmpty );

    // return empty string if getline fails, e.g. if at end of file.
    std::string slurpLine( FILE* ifp,
			   bool ignoreLine(const std::string&) = isSharpCommentOrEmpty );

    stringVecT slurpLines( std::istream& is = std::cin, 
			   bool ignoreLine(const std::string&) = isSharpCommentOrEmpty );

    // slurp lines and assign to LINES
    void slurpLines( stringVecT& lines,
		     std::istream& is = std::cin, 
		     bool ignoreLine(const std::string&) = isSharpCommentOrEmpty );

    // Slurp up to n lines. Ignores lines if ignoreLine(line) return true.
    // If only k < n lines are available a list of k strings is returned.
    stringVecT slurpNLines( std::istream& is, bool ignoreLine(const std::string&), int n );

    std::string join( const std::string& delimiter, const stringVecT& sVec );


    /* ***** split functions ***** */
    stringVecT split( const boost::regex& pattern, const std::string& s );

    stringVecT split(  const boost::regex&                 pattern,
		       const std::string::const_iterator&  textBeg,
		       const std::string::const_iterator&  textEnd
		       );

    // like Perl, except limit must be > 1.
    stringVecT split(  const boost::regex&  pattern,
		       const std::string&   s,
		       const size_t&        limit  );


    /* 
     * split functions which do not require linking to boost::regex
     * Also only a small subset of regular expressions are currently supported for PATTERN
     */
    stringVecT split( const std::string pattern, const std::string s );
    stringVecT split( const std::string pattern, const std::string s,
		      const size_t limit /* split into up to limit fields */ );


    // delimiter is a block of one or more characters found in the string splitChars.
    // e.g. split( s, std::string(" \t\n") ) splits on blocks of space, tab, or new line.
    stringVecT splitOnOneOrMoreCharsInCharSet( const std::string s,
					       const std::string splitChars );

    stringVecT splitOnOneOrMoreCharsInCharSet( const std::string s,
					       const std::string splitChars,
					       const int limit  );

    // Split S into fields separated by one occurrence of delimiterString
    // Behaviour with respect to limit is intended to be the same as perl.
    stringVecT splitOnOneMatch( const std::string s,
				const std::string delimiterString,
				int limit = 0 );

    stringVecT _split( std::string pattern, const std::string s );
    bool _split_trailingPlusCharFoundAndRemoved( std::string& pattern );
    bool _split_surroundingSquareBracketsFoundAndRemoved( std::string& pattern );
    void _split_diePatternTypeNotImplemented( const std::string pattern );


    stringVecT splitByLength( const std::string toBeSplit, const size_t segmentLength );

    std::vector<stringVecT>
    slurpAndSplit( std::istream& is,
		   const std::string& pattern,
		   bool ignoreLine(const std::string&) = isSharpCommentOrEmpty );

    void chomp( std::string& s );
    void chop( std::string& s );

    bool canStatFile( const std::string s );
    bool exists( const std::string filename );
    bool isDirectory( const std::string& s );
    std::ifstream::pos_type size( std::ifstream& ifs );
    size_t numLines( std::ifstream& ifs );

    void openOrDie     ( std::ifstream& ifs, const std::string& pathname  );
    void openCautiously( std::ofstream& ofs, const std::string& pathname );

    // for errors generated in perlish.cc See PERLISH_FATAL_ERROR in perlish.cc

    void die( const std::string& filename,
              const int&         line,
	      const std::string& errorMessage );

    inline void die( const std::string& filename,
		     const int&         line,
		     const std::ostringstream& errorMessageStringStream ){
      die( filename, line, errorMessageStringStream.str() );
    }
    

    // ***** The following functions do not have close Perl counterparts *****
    inline bool isNumeric( const char& c ){  return(  (c >= 0) && (c <= 9)  );  }

    // number string functions. The reason "Cstring" is appended to the name rather than
    // overloading is that at this point (2006/1) I do not know how to use an overloaded
    // predicate with the stl. For example std::count( li.begin(), li.end(), looksLikeInt ),
    // does not seem to work if looksLikeInt is overloaded.
    bool looksLikeUnsignedIntCstring( const char* const s ); // must be null terminated!
    bool looksLikeUnsignedInt( const std::string& s, const size_t startIndex = 0 );
    bool looksLikeUnsignedInts( const stringVecT& sVec ); // true if all strings look like unsigned ints.

    bool looksLikeIntCstring( const char* const s ); // must be null terminated!
    bool looksLikeInt( const std::string& s, const size_t startIndex = 0 );

    bool looksLikeRealCstring( const char* const s ); // must be null terminated!
    bool looksLikeReal( const std::string& s );


    // checks for errors but aborts instead of throwing an error.
    unsigned int toUnsignedInt( const char* const s );
    unsigned int toUnsignedInt( const std::string s, size_t startIndex = 0 );
    std::vector<unsigned int> toUnsignedInts( const stringVecT& sVec );

    // checks for errors but aborts instead of throwing an error.
    size_t toSizeT( const char* const s );
    size_t toSizeT( const std::string s, size_t startIndex = 0 );
    std::vector<size_t> toSizeTs( const stringVecT& sVec );

    int toInt( const char* const s );
    int toInt( const std::string s, size_t startIndex = 0 );

    // checks for errors but aborts instead of throwing an error.
    double toDouble( const char* const s );
    double toDouble( const std::string s );

    std::string toString( const double val, const char* format="%g" );
    std::string toString( const double val, const std::string& format );
    std::string toString( const int val, const char* const format="%d" );
    std::string toString( const int val, const std::string& format );
    std::string toString( const unsigned int val, const char* const format="%u" );
    std::string toString( const long unsigned int val, const char* const format="%lu" );
    std::string toString( const long unsigned int val, const std::string& format );
    std::string toString( const unsigned int val, const std::string& format );

    void fromString(       unsigned int  &lhs,  const char*         valueCString  );
    void fromString(       unsigned int  &lhs,  const std::string&  valueString   );
    void fromString(  long unsigned int  &lhs,  const char*         valueCString  );
    void fromString(  long unsigned int  &lhs,  const std::string&  valueString   );
    void fromString(             double  &lhs,  const char*         valueCString  );
    void fromString(             double  &lhs,  const std::string&  valueString   );

    // more string functions.
    void removeCharFromString( std::string& s, const char c );

    // remove leading spaces or tabs.
    void removeLeadingWhiteSpace( std::string& s );

    // substituteString not currently implemented.
    std::string substituteString( std::string& s, 
				  const std::string& stringToReplace, // string, not regex!
				  const std::string modifiers = "" // supports "g", "i", "ig".
				  );

    // convert strings like "10s", "20m", "4h", "3d", "9w", "2y" to seconds.
    // format is double followed by one letter time code from {s,m,h,d,w,y}
    // 'm' means minutes NOT months
    double toSeconds( const std::string& timeString );

    const std::string& spaceTabString();
    stringVecT emptyVectorOfStringTypes();


    // numeric functions
    template <typename T>
    T min( const T& a, const T& b ){
      return(  std::min( a, b )  );
    }

    template <typename T>
    T min( const T& a, const T& b, const T& c ){
      return(   std::min(  a, std::min( b, c )  )   );
    }

    template <typename T>
    T min( const T& a, const T& b, const T& c, const T& d ){
      return(   std::min(  std::min( a,b ), std::min( c,d )  )   );
    }

    /* ******* begin inline definitions ******* */
    inline bool exists( const std::string filename ){
      return !access(filename.c_str(), F_OK);
    }


    /* open output file string OFS using new pathname PATHNAME
     *
     * die if PATHNAME points to an existing file
     */
    inline void openCautiously( std::ofstream& ofs, const std::string& pathname ){

      GDB_ASSERTF(  !exists( pathname ), "file: %s, already exists", pathname.c_str()  );

      ofs.open( pathname.c_str() );
    }


    // output errorMessage to both std::cout and std::cerr and then exit with gdbExit
    inline void die( const std::string& filename,
		   const int& line,
		   const std::string& errorMessage ){
      std::cout << filename << ":" << line << "; " << errorMessage;
      std::cerr << filename << ":" << line << "; " << errorMessage;
      gdbExit( -1 );
    }


} // end namespace perlish


} // end namespace cbrc
#endif // defined _PERLISH_HH_
