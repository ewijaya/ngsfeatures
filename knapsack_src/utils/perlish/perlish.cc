/*
 *  Author: Paul Horton
 *  Creation Date: 2003.4.10
 *  Last Modification: $Date: 2009/06/22 20:42:08 $
 *  Copyright (C) 2003-2009, Paul B. Horton, All rights reserved.
 *  
 *  Description: See header files.
 */
#define PERLISH_FATAL_ERROR _fatalError( __LINE__ )
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <sstream>
#include "utils/solaris/getline.hh"
#include "utils/perlish/perlish.hh"

namespace cbrc{

namespace perlish{

  // declarations for internal use
  static std::ostringstream _errorStream;
  static void _fatalError( const int lineNo );

  bool isSharpCommentOrEmpty( const std::string& s ){
    if( s.length() <= 0 ) return true;
    if( s[0] == '#' ) return true;
    return false;
  }

  bool alwaysFalse( const std::string& s ){  return false;  }


  bool slurpLine(  std::string& line,
		   std::istream& iStream,
		   bool ignoreLine(const std::string&)  ){

    assert( !iStream.bad() );

    while(   std::getline( iStream, line )  &&  ignoreLine( line )   );
    
    return(  !iStream.eof()  );
  }


  std::string slurpLine( std::istream& is, bool ignoreLine(const std::string&) ){
    std::string line = ""; // return empty string if no more non-comment lines left to read.

    assert( !is.bad() );

    while( std::getline( is, line ) && ignoreLine( line) );
    return line;
  }


  std::string slurpLine( FILE* ifp, bool ignoreLine(const std::string&) ){
    size_t bufferSize = 0;
    char* nullValueMemoryCell = NULL;
    char** bufferPtr = &nullValueMemoryCell;

    std::string retVal;
    ssize_t getlineRetVal;
    while(  ( (getlineRetVal = getline( bufferPtr, &bufferSize, ifp )) != -1 )
	   && ignoreLine(*bufferPtr)  ); // Achtung this is not std::getline!
    if( getlineRetVal == -1 )  return std::string( "" );

    retVal = std::string( *bufferPtr );
    free( *bufferPtr );
    return retVal;
  }

  void slurpLines( stringVecT& lines,
		   std::istream& is,
		   bool ignoreLine(const std::string&) ){
    assert( !is.bad() );

    std::string line;

    while( std::getline( is, line ) ){
      assert( !is.bad() );
      if(  ignoreLine( line )  )  continue;
      lines.push_back( line );
    }

  }


  stringVecT slurpLines( std::istream& is, bool ignoreLine(const std::string&) ){
    stringVecT lines;
    slurpLines( lines, is, ignoreLine );
    return lines;
  }


  std::vector< stringVecT > slurpAndSplit( std::istream& is, const std::string& pattern,
					   bool ignoreLine(const std::string&) ){
    assert( !is.bad() );

    std::vector< std::vector<std::string> > fieldMatrix;

    std::string line;
    while(  (line = slurpLine( is, ignoreLine)) != ""  ){
      fieldMatrix.push_back(  split( pattern, line  )  );
    }
    return fieldMatrix;
  }

  std::vector<std::string> slurpNLines( std::istream& is, bool ignoreLine(const std::string&), int n ){
    assert( !is.bad() );

    std::vector<std::string> lines;
    std::string line;

    for( int k = 0; std::getline( is, line ); ){
      if(  ignoreLine( line )  )  continue;
      ++k;
      lines.push_back( line );
      if( k >= n )  return lines;
    }
    return lines;
  }


  std::string join( const std::string& delimiter,
		    const std::vector<std::string>& sVec ){

    if( !sVec.size() ){  return std::string("");  }

    std::string retval( sVec[0] );
    for( size_t i = 1; i < sVec.size(); ++i )  retval += delimiter + sVec[i];
    return retval;
  }


  /*******************************
   * Split and related functions *
   *******************************/
  std::vector<std::string> split( const boost::regex& pattern, const std::string& s ){

    const std::string::const_iterator  textBeg  =  s.begin();
    const std::string::const_iterator  textEnd  =  s.end  ();

    return split( pattern, textBeg, textEnd );
  }



  std::vector<std::string> split( const boost::regex&                 pattern,
				  const std::string::const_iterator&  textBeg,
				  const std::string::const_iterator&  textEnd
				  ){
    const bool patternMatchesStringStart  = matchesPrefixOf( textBeg, textEnd, pattern );

    std::vector<std::string> retVal;


    boost::sregex_token_iterator curPart( textBeg, textEnd,
					  pattern,
					  -1 // iterate over non-matching parts
					  );

    const boost::sregex_token_iterator partsEnd;


    /* ***** Process first token specially, to skip leading delimiters ***** */
    if( curPart == partsEnd ){
      return retVal;
    }

    const std::string token = *curPart;
    if( token.size() || !patternMatchesStringStart ){
      retVal.push_back( token );
    }

    ++curPart; // first token now processed.


    /* ***** Process remaining tokens ***** */
    for( /* no init */; curPart != partsEnd; ++curPart ){
      retVal.push_back( *curPart );
    }
    
    return retVal;
  }



  std::vector<std::string> split(  const boost::regex&  pattern,
				   const std::string&   s,
				   const size_t&        limit  ){
    GDB_ASSERTF( limit > 1,
		 "split with limit, only supported for limit > 1" );

    std::vector<std::string> retVal;

    std::string::const_iterator beg = s.begin();
    std::string::const_iterator end = s.end();

    boost::sregex_token_iterator curFieldItr( beg, end,
					      pattern,
					      -1 // iterate over non-matching parts
					      );

    const boost::sregex_token_iterator fieldsEnd;


    for(  size_t curNumFields = 2;  curFieldItr != fieldsEnd;  ++curFieldItr, ++curNumFields  ){

      // if number of fields passed limit, push the result of S, starting from the
      //    start of this field, onto the results and EXIT
      if( curNumFields > limit ){
	retVal.push_back(   s.substr(  curFieldItr->first - s.begin()  )   );
	return retVal;
      }

      // otherwise push current field onto results and iterate
      retVal.push_back( *curFieldItr );
    }
    
    return retVal;
  }




  std::vector<std::string> split( const std::string pattern, const std::string s ){
    return _split( pattern, s );
  }


  std::vector<std::string> _split( std::string pattern, const std::string s ){
    std::string patternCopy( pattern );

    // die if "\\" found in pattern
    if( pattern.find( "\\" ) != std::string::npos ){
      _errorStream << "Programming Error: escaping with \"\\\" not supported currently split patterns:"
		   << " pattern is \"" << patternCopy << "\"\n";
      _split_diePatternTypeNotImplemented( patternCopy );
    }

    // split trailing "+" in pattern, or warn if not present
    bool matchOneOrMore = _split_trailingPlusCharFoundAndRemoved( pattern );
    if( !matchOneOrMore ){
#ifdef PERLISH_SPLIT_DEBUG
      std::cout << "Pattern \"" << patternCopy << "\" passed to split, does not end in \"+\","
		<< " are you sure that is what you want?\n";
#endif // defined PERLISH_SPLIT_DEBUG
    }

    // pick up char set by removing surrounding "[]"
    bool patternIsCharSet = _split_surroundingSquareBracketsFoundAndRemoved( pattern );

    if( patternIsCharSet && !matchOneOrMore ){
      _errorStream << "Error: in regex given to split, \"[]\" without \"+\" is not yet implemented";
      _split_diePatternTypeNotImplemented( patternCopy );
    }

    if( patternIsCharSet ){
      return splitOnOneOrMoreCharsInCharSet( s, pattern );
    }

    return splitOnOneMatch( s, pattern );
  }

  
  std::vector<std::string> split( std::string pattern, const std::string s,
				  const size_t limit /* split into up to limit fields */ ){

    std::string patternCopy( pattern );

    // die if "\\" found in pattern
    if( pattern.find( "\\" ) != std::string::npos ){
      _errorStream << "Programming Error: escaping with \"\\\" not supported currently split patterns:"
		   << " pattern is \"" << pattern << "\"\n";
      _split_diePatternTypeNotImplemented( patternCopy );
    }

    // split trailing "+" in pattern, or warn if not present
    bool matchOneOrMore = _split_trailingPlusCharFoundAndRemoved( pattern );
    if( !matchOneOrMore ){
#ifdef PERLISH_SPLIT_DEBUG
      std::cout << "Pattern \"" << patternCopy << "\" passed to split, does not end in \"+\","
		<< " are you sure that is what you want?\n";
#endif // defined PERLISH_SPLIT_DEBUG
    }

    // pick up char set by removing surrounding "[]"
    bool patternIsCharSet = _split_surroundingSquareBracketsFoundAndRemoved( pattern );

    if( patternIsCharSet && !matchOneOrMore ){
      _errorStream << "Error: in regex given to split, \"[]\" without \"+\" is not yet implemented";
      _split_diePatternTypeNotImplemented( patternCopy );
    }

    if( patternIsCharSet ){
      return splitOnOneOrMoreCharsInCharSet( s, pattern, limit );
    }

    return splitOnOneMatch( s, pattern, limit );
  }


  /* split auxillary function: if pattern has just one character, return false
   *                           o.t. if pattern ends in +, remove it and return true
   *                           o.t. return false.  */
  bool _split_trailingPlusCharFoundAndRemoved( std::string& pattern ){
    if( pattern.size() <= 1 )                return false;
    if( pattern[pattern.size()-1] != '+' )   return false;
    chop( pattern );
    return true;
  }

  /* split auxillary function: if pattern has ¡å 3 characters, return false
   *                           o.t. if pattern is surrounded by '[' and ']' remove them and return true
   *                           o.t. return false.  */
  bool _split_surroundingSquareBracketsFoundAndRemoved( std::string& pattern ){
    if( pattern.size() <= 2 )                           return false;
    if(   ( pattern[0] != '[' )  
	  || ( pattern[pattern.size()-1] != ']' )   )   return false;
    pattern.replace( 0,                1, "" );
    pattern.replace( pattern.size()-1, 1, "" );
    return true;
  }

  // dump error message describing currently implemented patterns, then exit.
  void _split_diePatternTypeNotImplemented( const std::string pattern ){
    _errorStream << " Programming error: pattern \""
		 << pattern
		 << "\" given to split, but this type is not yet implemented\n"\
"\n"\
"Examples of implemented patterns are:\n"\
"        \"\\t+\"\n"\
"        \"[ \\t]+\"\n"\
"\n"\
"Note that these all end in \'+\' preceding by either a single character,\n"\
"or a char set delimited by \"[]\"\n";
    PERLISH_FATAL_ERROR;
  }



  std::vector<std::string> splitByLength( const std::string toBeSplit, const size_t segmentLength ){
    std::vector<std::string> segments;

    for( unsigned int curStart = 0; curStart < toBeSplit.size(); curStart += segmentLength ){
      segments.push_back(  toBeSplit.substr( curStart, segmentLength )  );
    }
    return segments;
  }

  /* Fields separated by any char in splitChars with length at least 1 are returned. */
  std::vector<std::string> splitOnOneOrMoreCharsInCharSet( const std::string s,
							   const std::string splitChars ){
    std::vector<std::string> fields;
    if( s.size() == 0 ) return( fields );
    bool prevMatched;
    std::string curField = "";
    int i;

    // skip leading delimiter chars.
    for(  i = -1; (i < (int) s.size()-1) && (splitChars.find(s[++i]) != std::string::npos);  );
    // at this point s[i] is either the first non delim char,
    // or if no such char exists, s[i] is the last character.
    if( i >= (int) s.size() - 1 ){ // if s[i] is the last character
      if( splitChars.find(s[i]) == std::string::npos ){
	fields.push_back(  s.substr( i, s.size() )  );
      }
      return( fields );
    }
    prevMatched = false;
    curField = s[i];
    for( ++i; i < (int) s.size(); ++i ){
      if( prevMatched ){
	if( splitChars.find(s[i]) == std::string::npos ){
	  curField = s[i];
	  prevMatched = false;
	}
      }else{
	if( splitChars.find(s[i]) != std::string::npos ){
	  fields.push_back( curField );
	  curField = "";
	  prevMatched = true;
	}else{
	  curField += s[i];
	}
      }
    }
    if( !prevMatched )  fields.push_back( curField );

    return fields;
  }


  // Split S into fields separated by one occurrence of delimiterString
  std::vector<std::string> splitOnOneMatch( const std::string s,
					    const std::string delimiterString,
					    int limit ){
    size_t limitPosValue = 0;
    if( limit > 0 )  limitPosValue = (size_t) limit;
    std::vector<std::string> fields;
    if( s.size() == 0 ) return( fields );

    /* ****************************************
       main loop to push fields onto list.
       Terminates early if LIMIT has a positive value
                                 and LIMIT-1 number of fields have been pushed
       **************************************** */
    std::string::size_type fieldStartPos = 0;
    size_t fieldNum = 1;
    for(  /* noop */ ;
	  (limitPosValue == 0) || (fieldNum < limitPosValue);
	  ++fieldNum ){
      std::string::size_type delimiterStartPos = s.find( delimiterString, fieldStartPos );

      // **TERMINATE AT END OF STRING**
      if( delimiterStartPos == std::string::npos ){
	// reached end of string s, so push (possibly empty) substring between last delimiter position and
	// end of string onto fields, and exit loop
	fields.push_back(   s.substr( fieldStartPos, s.size() )   );
	break;
      }

      // else if delimiter found, 
      // push field onto back of list and update fieldStartPos to start of next field
      fields.push_back(   s.substr( fieldStartPos, delimiterStartPos - fieldStartPos )  );
      fieldStartPos = delimiterStartPos + delimiterString.size();
    }

    // if loop terminated due to reaching limit-1 fields, then push the rest of s onto the field list.
    if( (limitPosValue != 0) && (fieldNum >= limitPosValue) ){ 
      fields.push_back(   s.substr( fieldStartPos )   );
    }

    if( limit != 0 )                              return fields;
    
    // else remove trailing null fields
    size_t firstTrailingEmptyStringPos = fields.size() - 1;
    for( size_t curFieldNum = fields.size()-1;
	 /* true */
	 ; --curFieldNum ){
      // if curFieldNum decremented past zero, all fields are empty
      if( curFieldNum >= fields.size() ){  
	firstTrailingEmptyStringPos = 0;
	break;
      }
      std::string field = fields[curFieldNum];
      if( field.size() ){
	firstTrailingEmptyStringPos = curFieldNum + 1;
	break;
      }
    }
    fields.resize( firstTrailingEmptyStringPos );
    return fields;
  }


  /* Up to limit fields, separated by one or more occurrences of any char in splitChars are returned. */
  std::vector<std::string> splitOnOneOrMoreCharsInCharSet( const std::string s,
							   const std::string splitChars,
							   const int limit ){
    size_t limitPosValue = 0;
    if( limit > 0 )  limitPosValue = (size_t) limit;
    size_t curFieldNum = 0;
    std::vector<std::string> fields;
    if( s.size() == 0 ) return( fields );
    if( !limit ) return( fields );
    bool prevMatched;
    std::string curField = "";
    int i;

    // skip leading delimiter chars.
    for(  i = -1; (i < (int) s.size()-1) && (splitChars.find(s[++i]) != std::string::npos);  );
    // at this point s[i] is either the first non delim char,
    // or if no such char exists, s[i] is the last character.
    if( i >= (int) s.size() - 1 ){ // if s[i] is the last character
      if( splitChars.find(s[i]) == std::string::npos ){
	fields.push_back(  s.substr( i, s.size() )  );
      }
      return( fields );
    }
    prevMatched = false;
    curField = s[i];
    for( ++i; i < (int) s.size(); ++i ){
      if( prevMatched ){
	if( splitChars.find(s[i]) == std::string::npos ){
	  curField = s[i];
	  prevMatched = false;
	}
      }else{
	if( splitChars.find(s[i]) != std::string::npos ){
	  if( ++curFieldNum >= limitPosValue ){
	    fields.push_back( s.substr( i - curField.size() ) );
	    return fields;
	  }
	  fields.push_back( curField );
	  curField = "";
	  prevMatched = true;
	}else{
	  curField += s[i];
	}
      }
    }
    if( !prevMatched )  fields.push_back( curField );

    return fields;
  }

  
  void chop( std::string& s ){ // if string has last character, remove it.
    if( s.size() == 0 ) return;
    s.resize( s.size()-1 );
  }

  void chomp( std::string& s ){ // if string ends in '\n', remove it.
    if( s.size() == 0 ) return;
    if( s[ s.size()-1 ] == '\n' )  s.resize( s.size()-1 );
  }

  bool canStatFile( const std::string s ){
    struct stat dummyStat;
    return(  !stat( s.c_str(), &dummyStat )  );
  }

  bool isDirectory( const std::string& s ){
    struct stat statStruct;
    stat( s.c_str(), &statStruct );
    return S_ISDIR( statStruct.st_mode );
  }


  std::ifstream::pos_type size( std::ifstream& ifs ){
    if (!ifs.good() || ifs.eof() || !ifs.is_open()) return 0;

    // save current position to restore later.
    std::ifstream::pos_type origPos = ifs.tellg();

    ifs.seekg( 0, std::ios_base::beg );
    std::ifstream::pos_type begPos = ifs.tellg();

    ifs.seekg( 0, std::ios_base::end );
    std::ifstream::pos_type endPos = ifs.tellg();

    // restore position.
    ifs.seekg( origPos, std::ios_base::beg );
    return( endPos - begPos );
  }


  size_t numLines( std::ifstream& ifs ){
    std::ios::pos_type curPos = ifs.tellg();
    ifs.seekg( 0 );
    std::string line;
    size_t numLines = 0;
    for( /* */; std::getline( ifs, line ); ++numLines );
    ifs.seekg( curPos );
    return numLines;
  }    


  // open input file stream IFS using pathname PATHNAME
  // 
  void openOrDie( std::ifstream& ifs, const std::string& pathname){
    ifs.open( pathname.c_str() );
    GDB_ASSERTF(  ifs.is_open(),
		  "Error: failed to open file \"%s", pathname.c_str()  );
  }



  bool looksLikeRealCstring( const char* const s ){
    const char* r = s;
    bool eSeen = false;
    bool eJustSeen = false;
    if( !*r ) return false; // empty string.
    if( *r == '-' ) ++r; // skip leading minus sign.
    for( ; *r; ++r ){ // loop until '.' or 'e' seen.
      if( isdigit(*r) ) continue;
      if( *r == '.' ) break;
      //Old before 19Oct06
      //if( *r == 'e' ){
      //Modified by hajime at 19Oct06
      if( *r == 'e' || *r == 'E' ){
	eSeen = true;
	eJustSeen = true;
	break;
      }
      return false;
    }
    if( !*r ){
      if( eJustSeen ) return false;
      return true;
    }
    for( ++r; *r; ++r ){
      if( eJustSeen ){
	eJustSeen = false;
	if( (*r == '-') || (*r == '+') ) continue; // '-' or '+' okay directly following an exponentiation "e", as in: "-8.88178419700125e-16"
      }
      if( isdigit(*r) ) continue;
       //Modified by Hajime at 19Oct06
       if( (*r == 'e' || *r == 'E') && !eSeen ){ // for example "-8.88178419700125e-16"
	eSeen = true;
	eJustSeen = true;
	continue;
      }
      return false;
    }
    return true;
  }

  bool looksLikeReal( const std::string& s ){ return looksLikeRealCstring( s.c_str() ); }

  double toDouble( const char* const s ){
    if( !looksLikeRealCstring(s) ){
      _errorStream << "expected double but got: \"" << s << "\"";
      PERLISH_FATAL_ERROR;
    }
    return atof( s );
  }

  double toDouble( const std::string s ){ return toDouble( s.c_str() ); }

  std::string toString( const double val, const char* format ){
    size_t len = snprintf( NULL, 0, format, val );
    char cString[len];
    sprintf( cString, format, val );
    std::string s( cString );
    return s;
  }
  std::string toString( const double val, const std::string& format ){
    return toString( val, format.c_str() );
  }
  std::string toString( const int val, const char* const format ){
    size_t len = snprintf( NULL, 0, format, val );
    char cString[len];
    sprintf( cString, format, val );
    std::string s( cString );
    return s;
  }
  std::string toString( const int val, const std::string& format ){
    return toString( val, format.c_str() );
  }
  std::string toString( const unsigned int val, const char* const format ){
    size_t len = snprintf( NULL, 0, format, val );
    char cString[len];
    sprintf( cString, format, val );
    std::string s( cString );
    return s;
  }

  std::string toString( const long unsigned int val, const char* const format ){
    size_t len = snprintf( NULL, 0, format, val );
    char cString[len];
    sprintf( cString, format, val );
    std::string s( cString );
    return s;
  }
  std::string toString( const long unsigned int val, const std::string& format ){
    return toString( val, format.c_str() );
  }

  std::string toString( const unsigned int val, const std::string& format ){
    return toString( val, format.c_str() );
  }


  void fromString( unsigned int& lhs, const char* valueCString ){
    sscanf( valueCString, "%u", &lhs );
  }
  void fromString( unsigned int& lhs, const std::string& valueString ){
    fromString( lhs, valueString.c_str() );
  }
  void fromString( long unsigned int& lhs, const char* valueCString ){
    sscanf( valueCString, "%lu", &lhs );
  }
  void fromString( long unsigned int& lhs, const std::string& valueString ){
    fromString( lhs, valueString.c_str() );
  }
  void fromString( double& lhs, const char* valueCString ){
    sscanf( valueCString, "%lf", &lhs );
  }
  void fromString( double& lhs, const std::string& valueString ){
    fromString( lhs, valueString.c_str() );
  }
    



  bool looksLikeUnsignedIntCstring( const char* const s ){
    const char* r = s;
    if( !*r ) return false; // empty string.
    if( !isdigit( *r ) ) return false;
    for( ++r; *r; ++r )  if( !isdigit(*r) ) return false;
    return true;
  }

  bool looksLikeUnsignedInt( const std::string& s, const size_t startIndex ){
    assert( s.size() > startIndex );
    return looksLikeUnsignedIntCstring( s.c_str() + startIndex );
  }

  bool looksLikeUnsignedInts( const std::vector<std::string>& sVec ){
    for( size_t i = 0; i < sVec.size(); ++i ){
      if(   !looksLikeUnsignedInt( sVec[i] )   )  return false;
    }
    return true;
  }

  bool looksLikeIntCstring( const char* const s ){
    const char* r = s;
    if( !*r ) return false; // empty string.
    if( (*r == '-') || (*r == '+') ) ++r; // skip minus sign if present.
    if( !isdigit( *r ) ) return false;
    for( ++r; *r; ++r )  if( !isdigit(*r) ) return false;
    return true;
  }

  bool looksLikeInt( const std::string& s, const size_t startIndex ){
    assert( s.size() > startIndex );
    return looksLikeIntCstring( s.c_str() + startIndex );
  }


  unsigned int toUnsignedInt( const char* const s ){
    GDB_ASSERTF( looksLikeUnsignedIntCstring(s),
		 "expected unsigned int, but got: \"%s\"", s );
    return atoi( s );
  }


  unsigned int toUnsignedInt( const std::string s, const size_t startIndex ){
    if( s.size() <= startIndex ){
      _errorStream << "startIndex = " 
		   << startIndex 
		   << " should be less than s.size() = " 
		   << s.size()
		   << std::endl;
      PERLISH_FATAL_ERROR;
    }
    return toUnsignedInt( s.c_str() + startIndex );
  }

  std::vector<unsigned int> toUnsignedInts( const std::vector<std::string>& sVec ){
    assert(   looksLikeUnsignedInts( sVec )  );
    std::vector<unsigned int> retval;
    retval.reserve( sVec.size() );
    for( size_t i = 0; i < sVec.size(); ++i ){
      retval.push_back(   toUnsignedInt( sVec[i] )   );
    }
    return retval;
  }
    

  int toInt( const char* const s ){
    if( !looksLikeIntCstring(s) ){
      _errorStream << "expected int but got: \"" << s << "\"";
      PERLISH_FATAL_ERROR;
    }
    return atoi( s );
  }

  int toInt( const std::string s, const size_t startIndex ){
    if( s.size() <= startIndex ){
      _errorStream  << "startIndex = " 
		    << startIndex 
		    << " should be less than s.size() = " 
		    << s.size()
		    << std::endl;
      PERLISH_FATAL_ERROR;
    }
    return toInt( s.c_str() + startIndex );
  }


  void removeCharFromString( std::string& s, const char c ){
    size_t newLen = s.size() - std::count( s.begin(), s.end(), c );
    std::string r( newLen, ' ' ); // any character will do.
    size_t ri = 0;
    for( size_t si = 0; si < s.size(); ++si ){
      if( s[si] != c ) r[ri++] = s[si];
    }
    s = r;
  }


  void removeLeadingWhiteSpace( std::string& s ){
    unsigned int posFirstNonWhiteSpace = 0;
    for( ; 
	 posFirstNonWhiteSpace < s.size() 
	   && ( s[posFirstNonWhiteSpace] == ' '
		|| s[posFirstNonWhiteSpace] == '\t' );
	 ++posFirstNonWhiteSpace );
    s.erase( 0, posFirstNonWhiteSpace );
  }



  double toSeconds( const std::string& timeString ){

    size_t len = timeString.size();

    if( len < 2 ){
      _errorStream << "Error: time string: \""
		   << timeString
		   << "\" passed to Perlish::toSeconds"
		   << " is too short to possible be correct";
      PERLISH_FATAL_ERROR;
    }

    const std::string timeUnitCodes( "smhdwy" );

    std::string timeCodeString = timeString.substr( len-1 );
    char timeCode = timeCodeString[0];
    
    if( timeUnitCodes.find( timeCode ) ==  std::string::npos ){
      _errorStream << "Error: expected time string to end in one of {"
		   << timeUnitCodes
		   << "}, but time string passed was: \""
		   << timeString;
      PERLISH_FATAL_ERROR;
    }

    std::string timeValString = timeString.substr( 0, len-1 );
    if(  !looksLikeReal( timeValString )  ){
      _errorStream << "Error: time string: \""
		   << timeString
		   << "\" passed to Perlish::toSeconds"
		   << " does not consist of a real number followed by one of {"
		   << timeUnitCodes
		   << "}";
      PERLISH_FATAL_ERROR;
    }
    
    double timeVal = toDouble( timeValString );

    double secsPerUnit = ( timeCode == 's' )  ?         1
      :                  ( timeCode == 'm' )  ?        60
      :                  ( timeCode == 'h' )  ?      3600
      :                  ( timeCode == 'd' )  ?     43200
      :                  ( timeCode == 'w' )  ?    302400
      :                  ( timeCode == 'y' )  ?  15778476

      :  /* default value should never be reached */   0;
    assert( secsPerUnit != 0 ); // 

    return( timeVal * secsPerUnit );
  }

    

  // return a string consisting of space and tab character.
  // The more simple: std::string( " \t" ) does not work because \t is not interpolated.
  const std::string& spaceTabString(){
    static std::string spaceTabString( " " );
    if( spaceTabString.size() != 2 ){
      spaceTabString += '\t';
    }
    return spaceTabString;
  }


  std::vector<std::string> emptyVectorOfStringTypes(){
    static std::vector<std::string> retval;
    return retval;
  }


  /* Print fatal error message and exit. The error is printed to both standard out
   * and standard in, and the alert character (which sometimes beeps the speaker,
   * but unfortunately just prints "^G" from an emacs shell).
   *
   * This may seem excessive, but when program output is redirected to pipes it
   * can be easy to miss error messages.
   *
   * Usage: set the global ostringstream _errorStream
   * and call this function (from this file only) through the macro PERLISH_FATAL_ERRORR;
   */
  static void _fatalError( const int lineNo ){
    std::cout << std::endl << '\a' << __FILE__ << ":" << lineNo 
	      << " Error(cout): " << _errorStream.str() << std::endl;
    std::cerr << std::endl << '\a' << __FILE__ << ":" << lineNo 
	      << " Error(cerr): " << _errorStream.str() << std::endl;
    gdbExit(-1);
  }
}; // end namespace perlish

}; // end namespace cbrc
