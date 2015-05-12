/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004-2009, Paul Horton, All rights reserved.
 *  Creation Date: 2004.1.5
 *  Last Modified: $Date: 2009/08/25 13:12:26 $
 *  
 *  Description: Command line argument parser.
 *
 *  Purpose: command line argument parsing code for reuse.
 *
 *  Note: Implementation works on list of C-strings and therefore could
 *        conceivably be applied to input other than command line arguments.
 *
 *  Implementation notes
 *      _argvParserNullString as default argument to methods
 *
 *          Many functions take one or two strings as arguments because flags often
 *          have two names (one short and one long). Since the null string can never
 *          match any arg, the null string "_argvParserNullString" is often used as
 *          the default argument for the second string -- eliminating the need to
 *          have separate code for the cases of one string and two strings passed.
 *
 *      _ARGVPARSER_MAKE_FLAGS_VECTOR
 *
 *          Since many methods accept flags in the form of one, two, or a std::vector
 *          of std:string's, The macro: _ARGVPARSER_MAKE_FLAGS_VECTOR
 *          facilitates supporting both types with a minimum of extra code.
 *          It is used to create a variable std::vector<std::string> FLAGS containing
 *          the flags from std::string flag, std::string flagAlias.
 *          Caveat: the variable names "flags", "flag" and "flagAlias" are hardcoded.
 *  
 */

#ifndef _ARGVPARSER_HH_
#define _ARGVPARSER_HH_
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "utils/gdb/gdbUtils.hh"
#include "utils/stl/stlTypes.hh"
#define QQ(x) #x
#define Q(x) QQ(x)
#define _ARGVPARSER_MAKE_2_FLAGS_VECTOR\
    std::vector<std::string> flags(2);\
    flags[0] = flag;\
    flags[1] = flagAlias



namespace cbrc{

static const std::string _argvParserNullString("");

class ArgvParser{
  typedef std::pair<std::string, std::string> stringPairT;
public:


  /* ********** CONSTANTS ********** */

  static const std::string&  useStdinIndicator(){
    static const std::string  _useStdinIndicator( "-" );
    return  _useStdinIndicator;
  }



  /* ********** CONSTRUCTOR ********** */

  // "$0" in usage is replaced with argv[0].
  // If usage is just one line, "Usage: $0" is prepended to it.
  ArgvParser( const int&          argc,
	      const char*         argv[],
	      const std::string&  usage,
	      std::ostream& errorOut = std::cout )
    : _argc(argc), _argcOrig(argc),
      _argv(argv), _argvOrig(argv),
      _usage(usage),
      argUsed(argc,false), argUsedAsFlagOrOpt(argc,false),
      errorOut(errorOut){

    _pathname = _argv[0];
    substituteVariables( _usage );

    const std::string usageMarker( "Usage: " );

    // if usage is a single line, and usageMarker if not already there.
    if(   (   usage.find( '\n' )        == std::string::npos)
	  && (usage.find( usageMarker ) != 0                )  ){
      _usage = usageMarker + pathname() + std::string( " " ) + usage;
    }

    setDoc( "--usage", _usage  );
  }


  // ************************************************************
  // * functions for dumping or accessing the args as a vector.
  std::string commandLine() const;
  void logCommandLine( const std::string& pathname ) const;
  std::string pathname() const{ return _pathname; }

  std::vector<std::string> argsAsStrings() const;

  // list of non-shifted unused args, _argv[0] (program name) skipped.
  std::vector<std::string> unusedArgs() const;


  // checks if index is too large.
  // marks arg as used.
  std::string operator[]( const int& index );

  int argc()     const{ return _argc; }
  int argcOrig() const{ return _argcOrig; }
  size_t size()  const{ return _argc; }
  int numArgs()  const{ return _argc - 1; }
  int numOpts()  const{
    errorOut << "ArgvParser.hh; Warning: numOpts is decremented, use numArgs()\n";
    return _argc - 1; }



  /* ********** Methods to check for presence of flags or options ********** */

  // Check if arg list has string matching any of the FLAGS
  // If one of them is present, mark first one as used and return true.
  bool hasFlag( const std::vector<std::string>& flags );

  bool hasFlag( const std::string& flagSpec ){
    return hasFlag( splitFlagSpec(flagSpec) );
  }

  bool hasFlag( const std::string& flag,
		const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return hasFlag( flags );
  }


  // Like hasFlag, but delete matching arg from _argv.
  bool hasFlagDelete( const std::vector<std::string>& flags );
  bool hasFlagDelete( const std::string& flagSpec ){
    return hasFlagDelete( splitFlagSpec(flagSpec) );
  }
  bool hasFlagDelete( const std::string& flag,
		      const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return hasFlagDelete( flags );
  }


  // Check if arg list has opt flagged by any of the FLAGS
  // Behavior is like hasFlag, except that the matching flag must have a following arg.
  // The flag, but not the following argument, is marked as used.
  // Both flag and the following argument are marked as usedAsFlagOrOpt.
  //
  // Thus subsequent access to positional arguments will skip both but
  // dieIfUnusedArgs will still complain if the flagged opt is never
  // directly accessed.
  bool hasOpt( const std::vector<std::string>& flags );
  bool hasOpt( const std::string& flagSpec ){
    return hasOpt( splitFlagSpec(flagSpec) );
  }
  bool hasOpt( const std::string& flag,
	       const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return hasOpt( flags );
  }


  // Return true or false for mandatory yes/no option.
  // exits if option not found or flag is not 'y' or 'n'
  // both args are marked as used.
  bool yesNoFlag( const std::string opt );


  /* ********** Methods to get flags or options ********** */

  // If arg list contains any arg matching one of the FLAGS
  //     return the first matching arg and mark it as used.
  // else  return empty string.
  std::string getFlag( const std::vector<std::string>& flags );
  std::string getFlag( const std::string& flag,
		      const std::string& flagAlias = _argvParserNullString ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return getFlag( flags );
  }

  // If arg list has opt flagged by any of the FLAGS
  //     return that opt. Mark flag and opt as used.
  // else  return empty string.
  std::string getOpt( const std::string& flagSpec,
		      const std::string defaultValue = _argvParserNullString ){
    return getOpt( splitFlagSpec(flagSpec), defaultValue );
  }
  std::string getOpt( const std::vector<std::string>& flags,
		      const std::string defaultValue = _argvParserNullString );



  // If arg list has opt flagged by any of the FLAGS
  //     return that opt. Mark flag and opt as used.
  // else  warn and exit.
  std::string getOptOrDie( const std::string& flagSpec ){
    return getOptOrDie( splitFlagSpec(flagSpec) );
  }
  std::string getOptOrDie( const std::vector<std::string>& flags );

  // get enum at position posArgIndex
  // die if argument absent or does not match any of the possible values.
  std::string getEnumOrDie( const int&          posArgIndex,
			    const std::string&  possibleValuesSpec );

  // getEnumOpt. Quits with error message if value is not found in possibleValues
  std::string getEnumOpt( const std::string& flagSpec,
		      const std::string& possibleValuesSpec,
		      const std::string defaultValue = _argvParserNullString ){
    return getEnumOpt( splitFlagSpec(flagSpec),
		   splitOnVerticalBar(possibleValuesSpec),
		   defaultValue );
  }
  std::string getEnumOpt( const std::string& flagSpec,
		      const std::vector<std::string>& possibleValues,
		      const std::string defaultValue = _argvParserNullString ){
    return getEnumOpt( splitFlagSpec(flagSpec),
		   possibleValues,
		   defaultValue );
  }
  std::string getEnumOpt( const std::vector<std::string>& flags,
		      const std::string& possibleValuesSpec,
		      const std::string defaultValue = _argvParserNullString ){
    return getEnumOpt( flags,
		   splitOnVerticalBar(possibleValuesSpec),
		   defaultValue );
  }
  std::string getEnumOpt( const std::vector<std::string>& flags,
			  const std::vector<std::string>& possibleValues,
			  const std::string defaultValue = _argvParserNullString );
  

  // return true if argIndex is in range. argIndex == 0 allowed.
  // if in range, corresponding arg is marked as used.
  // args marked as UsedAsFlagOrOpt are skipped.
  bool hasArg( const int& posArgIndex );


  // Return argIndex_th arg, skipping args mark as UsedAsFlagOrOpt
  // Return Empty string if argIndex out of range.
  std::string getArg( const int& posArgIndex );


  // Return argIndex_th arg, skipping args mark as UsedAsFlagOrOpt
  // Die with error message if argIndex out of range.
  std::string getArgOrDie( const int& posArgIndex );


  /* ********** Methods to get flags or options ********** */

  // If posArgIndex in range, set lhs and mark arg as used.
  //     if error occurs while setting lhs,
  //         for example trying to set an int from "3.14",
  //         die with error message.
  // return posArgIndex(th) argument or empty string if no such argument found.
  template <typename T> std::string set( T& lhs, const int& posArgIndex );
  
  // Set LHS and mark optFlag and opt as used.
  // return matching opt if found, otherwise return empty string.
  template <typename T>
  std::string set( T& lhs, const std::vector<std::string>& flags );

  template <typename T>
  std::string set( T& lhs, const std::string& flagSpec ){
    return set( lhs, splitFlagSpec(flagSpec) );
  }

  // specialization of set for bool's
  std::string set( bool& lhs, const std::vector<std::string>& flags );

  std::string set( bool& lhs, const std::string& flagSpec ){
    return set( lhs, splitFlagSpec(flagSpec) );
  }



  template <typename T>
  std::string set( T& lhs,
		   const std::string& flag,
		   const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return set( lhs, flags );
  }

  // Set lhs and mark used arg as used.
  // Die with error if corresponding argIndex not found
  // otherwise set lhs and return posArgIndex(th) argument
  template <typename T>
  std::string setOrDie( T& lhs, const int& posArgIndex );

  // Set LHS and mark optFlag and opt as used.
  // Die with error if corresponding opt not found
  // return matching opt if found.
  template <typename T>
  std::string setOrDie( T& lhs, const std::vector<std::string>& flags );

  template <typename T>
  std::string setOrDie( T& lhs,
			const std::string& flagSpec ){
    return setOrDie( lhs, splitFlagSpec(flagSpec) );
  }

  template <typename T>
  std::string setOrDie( T& lhs,
			const std::string& flag,
			const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return setOrDie( lhs, flags );
  }


  std::string setCautiously( std::ofstream& ofs, const int& posArgIndex );
  std::string setCautiously( std::ofstream& ofs, const std::string& flagSpec ){
    return setCautiously( ofs, splitFlagSpec(flagSpec) );
  }
  std::string setCautiously( std::ofstream& ofs,
			     const std::string& flag, const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return setCautiously( ofs, flags );
  }
  std::string setCautiously( std::ofstream& ofs, const std::vector<std::string>& flags );

  std::string setCautiouslyOrDie( std::ofstream& ofs, const int& posArgIndex );
  std::string setCautiouslyOrDie( std::ofstream& ofs, const std::string& flagSpec ){
    return setCautiouslyOrDie( ofs, splitFlagSpec(flagSpec) );
  }
  std::string setCautiouslyOrDie( std::ofstream& ofs,
			     const std::string& flag, const std::string& flagAlias ){
    _ARGVPARSER_MAKE_2_FLAGS_VECTOR;
    return setCautiouslyOrDie( ofs, flags );
  }
  std::string setCautiouslyOrDie( std::ofstream& ofs, const std::vector<std::string>& flags );

  /* ** getIstream methods **
   * open file and return reference to it
   * unless filename "-" (useStdinIndicator) is given
   * return referece to std::cin in that case.
   *
   * "OrDie" versions require argument to be present,
   * other versions default to std::cin if arg missing
   */
  std::istream& getIstream( const int& posArgIndex ){
    return _getIstreamAux( posArgIndex, false );
  }
  std::istream* getIstreamPtr( const int& posArgIndex ){
    return _getIstreamPtrAux( posArgIndex, false );
  }
  std::istream& getIstream( const std::string& flagSpec ){
    return _getIstreamAux( splitFlagSpec(flagSpec), false );
  }
  std::istream& getIstream( const std::vector<std::string>& flags ){
    return _getIstreamAux( flags, false );
  }

  /* getIstreamOrDie, like getIstream except die if
   * arg not present
   */
  std::istream& getIstreamOrDie( const int& posArgIndex ){
    return _getIstreamAux( posArgIndex, true );
  }
  std::istream& getIstreamOrDie( const std::string& flagSpec ){
    return _getIstreamAux( splitFlagSpec(flagSpec), true );
  }
  std::istream& getIstreamOrDie( const std::vector<std::string>& flags ){
    return _getIstreamAux( flags, true );
  }
  std::istream* getIstreamPtrOrDie( const int& posArgIndex ){
    return _getIstreamPtrAux( posArgIndex, true );
  }


  // set LHS from next unused arg, or return NULL if none left.
  template <typename T> const char* const setFromNextUnused( T& lhs ){
    std::ostringstream errorMessageSStream;

    const std::string  nextUnused  =  shift();

    if(  !nextUnused.size()  )   return  NULL;

    setFromString( lhs, nextUnused, errorMessageSStream );

    return nextUnused.c_str();
  }


  /* ********** file opening utilties ********** */
  void open( std::ifstream& ifs, const std::string& pathname );

  std::istream* openIstream( const std::string& pathname );

  void openCautiously( std::ofstream& ofs, const std::string& pathname ) const;

  // if ifsPtr points to std::cin noop, otherwise close the file it points to.
  void closeIstream( std::istream* ifsPtr );



  /* ********** shift functions, roughly modeled on the Perl shift function ********** */
  std::string shift(); // returns empty string if no unsed args left.

  // shiftMandatory, like shift but die if no unused args remaining.
  std::string shiftOrDie();

  // like shift, but does not skip unused args. Deprecated.
  std::string shiftUsedOk();

  // like shiftMandatory, but does not skip unused args. Deprecated.
  std::string shiftMandatoryUsedOk();

  
  /* ********** usage and message related functions ********** */
  std::string usage() const{ return _usage; }

  // dieIfUnusedArgs calls printDoc with first argument, if that fails, print usage and exit.
  void dieIfUnusedArgs() const;


  /* ********** "doc" (documentation) methods declarations ********** */
  // Attempt to print doc for first option.
  void printDoc( const std::string& locale = "" ) const{
    if( numArgs() )  printOptionDoc( _argv[1], locale );
  }

  // locale defaults to $LANG, or if not set "C".
  void printOptionDoc( const std::string& optionName,
		       const std::string& locale = "" ) const;

  // locale defaults to "C"
  // in optionValue, $0 is replace with pathname
  void setDoc( const std::string& optionNamesVBarList,
	       const std::string& optionValue,
	       const std::string& locale = "C" );


  // locale defaults to $LANG, or if not set "C".
  // getDoc returns empty string if optionName not found.
  std::string getDoc( const std::string& optionName,
		      std::string locale = "" ) const;


  // print message (unless empty) and usage message, then exit.
  // return type made bool, to allow constructs like  (a > 0) || die( "a must be positive" )
  bool die( const std::string message = "" ) const;


  // *************** Deprecated Methods ***************
  // deprecated name for hasFlag
  bool has( const std::string& flag,
	    const std::string& flagAlias = _argvParserNullString ){
    errorOut << "ArgvParser Warning: DEPRECATED: \"has\" method renamed \"hasFlag\"\n";
    return hasFlag( flag, flagAlias );
  }

  // deprecated name for hasOpt
  bool hasNextArg( const std::string& flag ){
    errorOut << "ArgvParser Warning: DEPRECATED: \"has\" method renamed \"hasOpt\"\n";
    return hasOpt( flag );
  }

  bool hasUnused( const std::string& flag ){
    die( "ArgvParser Error: hasUnused is no longer part of the API, \"hasFlag\" will probably do what you need\"\n" );
    return false; // silence compiler warning.
  }


  // deprecated old name for hasFlagDelete
  bool eat( const std::string& opt, const std::string& alias = _argvParserNullString ){
    die( "ArgvParser Error: method \"eat\" has been renamed \"hasFlagDelete\"" );
    return false; // silence compiler warning.
  }

  // for debugging, print state of _argv, argUsed, argUsedAsFlagOrOpt to std::cout
  void dumpArgvInfo() const;

  /* ********** class functions ********** */
  static bool fileExists( std::string pathname ){  return !access(pathname.c_str(), F_OK); }
  static bool isNullString( const std::string& s ){
    return( s == _argvParserNullString );
  }

  // methods to check if string is numerical
  static bool looksLikeInt(         const std::string& s ){  return looksLikeInt(         s.c_str() );  }
  static bool looksLikeUnsignedInt( const std::string& s ){  return looksLikeUnsignedInt( s.c_str() );  }
  static bool looksLikeDouble(      const std::string& s ){  return looksLikeDouble(      s.c_str() );  }

  static bool looksLikeInt(         const char* const& s );
  static bool looksLikeUnsignedInt( const char* const& s );
  static bool looksLikeDouble(      const char* const& s );

  // return true iff ELEMENT is element of pipeSeparatedList
  static bool isMember( const std::string& pipeSeparatedList,
			const std::string& element );

private:
  std::istream& _getIstreamAux( const int& posArgIndex, const bool& dieIfArgNotFound );
  std::istream* _getIstreamPtrAux( const int& posArgIndex, const bool& dieIfArgNotFound );

  std::istream& _getIstreamAux( const std::vector<std::string>& flags,
				const bool& dieIfArgNotFound );

  std::istream& _getIstreamAux( const std::string& flagSpec, const bool& dieIfArgNotFound ){
    return _getIstreamAux( splitFlagSpec(flagSpec), dieIfArgNotFound );
  }


  // print positional argument not found error then usage message, then exit.
  void _diePosArgNotFoundError( const int& posArgIndex );

  // print flag not found error then usage message, then exit.
  void _dieFlagNotFoundError( const std::vector<std::string>& flags ) const;

  void _dieOptValueNotInPossibleValuesList( const std::string& optFlag,
					    const std::string& optValue,
					    const std::vector<std::string>& possibleValues );

  // is a string a long style switch.
  bool isLongSwitch( const std::string& s ) const;



  // true iff all args have been shifted or used.
  bool allArgsUsed() const{
    for( int i = 1; i < _argc; ++i )  if( !argUsed[i] ) return false;
    return true;
  }

  // list of non-shifted args used as flag or flagged option, _argv[0] (program name) skipped.
  std::vector<std::string> usedAsFlagOrOptArgs() const;

  // list of non-shifted args used as flag or flagged option, _argv[0] (program name) skipped
  std::string usedAsFlagOrOptArgsString() const{
    return(   joinStringList( ", ", usedAsFlagOrOptArgs() )   );
  }

  // list of shifted args
  std::vector<std::string> shiftedArgs() const;

  // list of shifted args
  std::string shiftedArgsString() const{
    return(   joinStringList( ", ", shiftedArgs() )   );
  }

  // utility function similar to Perl's "join" function,
  static std::string joinStringList( const std::string& delimiter,
				     const std::vector<std::string>& stringList );

  // utility function similar to Perl split( '|', s );
  // if assertValidFlags, assert that each resulting string looks like a command line flag.
  static stringVecT splitOnVerticalBar(  const std::string& s,
				         const bool         assertValidFlags = false  );

  // assert that FLAG starts with -[a-zA-Z] or --[a-zA-Z]
  static void assertLooksLikeFlag( const std::string& flag );

  // reality check format of flagSpec, then return splitOnVerticalBar(flagSpec)
  static stringVecT splitFlagSpec( const std::string& flagSpec );

  // return index into argv of the argIndex_th argument, skipping
  // args marked as UsedAsFlagOrOpt.
  // If such an arg exists, mark it as used and  set inRange to true
  // otherwise set inRange to false.
  int _argvIndexOfPositionalArg( const int& argIndex, bool& inRange );

  // return number of args which are not marked as UsedAsFlagOrOpt, including argv[0].
  int _argvUnusedAsFlagOrOptCount() const{
    int c = 0;
    for( size_t argvIndex = 0; argvIndex < argUsedAsFlagOrOpt.size(); ++argvIndex ){
      if( !argUsedAsFlagOrOpt[argvIndex] ) ++c;
    }
    return c;
  }

  /* ***** Auxillary functions for setDoc ***** */
  // call _setDocOneOptionName for each option name
  void _setDoc( const std::vector<std::string>& optionNames,
		const std::string& optionValue,
		const std::string& locale );

  // Do the real work of setDoc
  void _setDocOneOptionName( const std::string& optionName,
			     /***/ std::string  optionValue,
			     const std::string& locale );
  

  /* ************ setFromString methods declarations ************
   * convert rhsString to type of LHS and assign to LHS
   * If conversion fails
   *     set errorMessageSStream and return false
   *
   * Currently implemented types are:
   *     - double
   *     - std::ifstream
   *     - std::istream*&
   *     - std::ofstream
   *     - std::string
   *     - unsigned int
   *     - int
   */
  bool setFromString( double& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( std::ifstream& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( std::istream*& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( std::ofstream& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( std::string& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( unsigned int& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( long unsigned int& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  bool setFromString( int& lhs,
		      const std::string& rhsString,
		      std::ostringstream& errorMessageSStream );

  // ********** end _setFromString methods declarations **********


  // If any arg matches any of the FLAGS
  //     set INDEX to the index of the first such arg
  //     return true
  // Else
  //     return false  (leaving argIndex unchanged)
  bool _setIndexToFirstMatchingFlag( int& argIndex,
				     const std::vector<std::string>& flags );

  // Like _setIndexToFirstMatchingFlag, except match last arg is not allowed.
  bool _setIndexToFirstMatchingOptFlag( int& argIndex,
					const std::vector<std::string>& flags );


  // return true iff S matches one of STRINGS
  bool _cstrMatchesOneOf( const char*& s,
			  const std::vector<std::string>& strings );

  // die with error if argIndex is out of range.
  // marks as used if present.
  void _assertHasArg( const int& argIndex );

  // die with error if opt not found.
  // marks as used if present.
  void _assertHasOpt( const std::vector<std::string>& flags );

  void markUsedAsFlagOrOpt( const int& argIndex ){
    argUsed[ argIndex ]            = true;
    argUsedAsFlagOrOpt[ argIndex ] = true; 
  }


  void substituteVariables( std::string& s ) const; // replace $0 with pathname.

  // remove argument from middle of argument list.
  void _deleteArg( const int& argIndex );


  // **** Class Variables to hold object data *****
  int _argc;           // decreases when args are shifted.
  const int _argcOrig; // value of argc passed to constructor.
  const char** _argv;
  const char ** const _argvOrig;
  std::string _pathname;
  std::string _usage;
  std::vector<   std::pair< std::string, std::vector<stringPairT> >   > docLocaleOptionNameValuePair;
  std::vector<bool> argUsed;
  std::vector<bool> argUsedAsFlagOrOpt;
  std::ostream& errorOut;
};


  /* ======================================================================
   *          BEGIN FUNCTION DEFINITIONS
   * **********************************************************************/

inline std::string ArgvParser::commandLine() const{

  // return join( " ", _argv[0] )
  std::string retVal( _argv[0] );
  for( int i = 1; i < argc(); ++i )  retVal += std::string(" ") + std::string(_argv[i]);
  return retVal;
}


inline void ArgvParser::logCommandLine( const std::string& filename ) const{

  std::ofstream commandLineLog( filename.c_str(), std::ios::app );
  for( int i = 0; i < argc(); ++i ){
    commandLineLog << _argv[i] << std::endl;
  }
}


inline std::vector<std::string> ArgvParser::argsAsStrings() const{

  std::vector<std::string> retVal;
  for( int i = 1; i < argc(); ++i ){
    retVal.push_back( _argv[i] );
  }

  return retVal;
}


inline std::string ArgvParser::operator[]( const int& index ){

  // check index for validity
  if( index < 0 )        die( "Programming Error, expected non-negative index" );
  if( index >= argc() )  _diePosArgNotFoundError( index );

  // index okay, so return arg
  argUsed[index] = true;
  return _argv[index];
}


inline std::string ArgvParser::getArg( const int& posArgIndex ){
  bool inRange;

  int argvIndex = _argvIndexOfPositionalArg( posArgIndex, inRange );
  if( inRange ){
    return(  _argv[ argvIndex ] );
  }
  // else posArgIndex was out of range.
  return _argvParserNullString;
}


inline std::string ArgvParser::getArgOrDie( const int& posArgIndex ){
  std::string retval = getArg( posArgIndex );

  if( retval.size() )  return retval;

  std::string errorMessage( "Command line parsing error: " );
  if( posArgIndex > argc() ){
    errorOut << errorMessage
	     << " attempted to read argument #" << posArgIndex
	     << ", but only " << argc()
	     << " arguments remain unprocessed\n";
    die();
  }
  // else read failed because of args marked as UsedAsFlagOrOpt
  errorOut << errorMessage
	   << " attempted to read positional argument #" << posArgIndex
	   << ", but after removing args processed as flags or flagged options, #" 
	   << _argvUnusedAsFlagOrOptCount() - 1
	   << " is the last available positional argument."
	   << std::endl;
  die();
  return _argvParserNullString; // dummy return to silence compiler.
}



inline bool ArgvParser::hasOpt( const std::vector<std::string>& flags ){
  int argIndex;
  if(   _setIndexToFirstMatchingOptFlag( argIndex, flags )   ){
    markUsedAsFlagOrOpt( argIndex );
    argUsedAsFlagOrOpt[argIndex+1] = true;
    return true;
  }
  return false;
}



inline std::string ArgvParser::getFlag( const std::vector<std::string>& flags ){
  int argIndex;
  if(   _setIndexToFirstMatchingFlag( argIndex, flags )   ){
    markUsedAsFlagOrOpt( argIndex );
    return _argv[argIndex];
  }
  return _argvParserNullString;
}



inline std::string ArgvParser::getOpt( const std::vector<std::string>& flags,
				       const std::string defaultValue ){
  int argIndex;
  if(   _setIndexToFirstMatchingOptFlag( argIndex, flags )   ){
    markUsedAsFlagOrOpt( argIndex );
    markUsedAsFlagOrOpt( argIndex+1 );
    return _argv[argIndex+1];
  }
  return defaultValue;
}


inline std::string ArgvParser::getEnumOrDie(  const int&          posArgIndex,
					      const std::string&  possibleValuesSpec ){

  std::string arg;
  if(  hasArg( posArgIndex )  ){
    arg = getArg( posArgIndex );
    const std::vector<std::string> possibleValues = splitOnVerticalBar( possibleValuesSpec );
    if(  std::find( possibleValues.begin(), possibleValues.end(), arg )
	== possibleValues.end()  ){
      errorOut  <<  "Error, when looking for enum \""  <<  possibleValuesSpec  <<  "\", "
		<<  "as "  <<  posArgIndex  <<  "th argument, "
		<<  "found string \""  <<  arg  <<  std::endl;
      die();
    }
    argUsed[ posArgIndex ]  =  true;
    return arg;
  }
  
  errorOut  <<  "Error, not enough arguments found, when looking for enum \"" 
	    <<  possibleValuesSpec  <<  "\", "
	    <<  "as "  <<  posArgIndex  <<  "th argument, "  <<  std::endl;
  die();
  return _argvParserNullString; // please compiler.
  
}


inline std::string ArgvParser::getEnumOpt( const std::vector<std::string>& flags,
					   const std::vector<std::string>& possibleValues,
					   const std::string defaultValue ){
  int argIndex;
  if(   _setIndexToFirstMatchingOptFlag( argIndex, flags )   ){
    const char* optValue = _argv[argIndex+1];
    if( std::find( possibleValues.begin(), possibleValues.end(), optValue )
	== possibleValues.end() ){
      _dieOptValueNotInPossibleValuesList( _argv[argIndex], optValue, possibleValues );
    }
    markUsedAsFlagOrOpt( argIndex );
    markUsedAsFlagOrOpt( argIndex+1 );
		   return optValue;
  }
  return defaultValue;
}



inline std::string ArgvParser::getOptOrDie( const std::vector<std::string>& flags ){
  int argIndex;
  if(   _setIndexToFirstMatchingOptFlag( argIndex, flags )   ){
    markUsedAsFlagOrOpt( argIndex );
    markUsedAsFlagOrOpt( argIndex+1 );
    return _argv[argIndex+1];
  }

  // else if flag not found
  _dieFlagNotFoundError( flags );
  return _argvParserNullString; // silence compiler.
}



inline std::string ArgvParser::shift(){

  if( argc() < 2 ) return _argvParserNullString;

  std::string retVal( _argv[1] );

  // shift arg values and used state
  for( size_t i = 1; i < size()-1; ++i ){
    _argv[i]              = _argv[i+1];
    argUsed[i]            = argUsed[i+1];
    argUsedAsFlagOrOpt[i] = argUsedAsFlagOrOpt[i+1];
  }
  --_argc;

  return retVal;
}
  
    
inline std::string ArgvParser::shiftOrDie(){
  if( argc() < 2 ) die( "number of args given too small" );

  return( shift() );
}



//********** Definition of set functions **********
template <typename T>
inline std::string ArgvParser::set( T& lhs, const int& posArgIndex ){
  std::ostringstream errorMessageSStream;

  if(  hasArg( posArgIndex )  ){
    std::string arg = getArg( posArgIndex );
    if(   setFromString( lhs, arg, errorMessageSStream  )   ){
      return arg;
    }
    errorOut << "Command Line Parsing Error when attempting to read arg #"
	     << posArgIndex << "\n\n"
	     << errorMessageSStream.str() << "\n";
    die();
  }

  return _argvParserNullString;
}


template <typename T>
inline std::string ArgvParser::set( T& lhs, const stringVecT& flags ){

  GDB_ASSERTF( flags.size(), "ArgvParser given empty flags vector" ); 
  
  std::ostringstream errorMessageSStream;
  if(  hasOpt(flags)  ){
    std::string opt = getOpt(flags);
    if(   setFromString( lhs, opt, errorMessageSStream )   ){
      return opt;
    }
    errorOut << "Command Line Parsing Error when attempting to read option: "
	     << getOpt(flags)
	     << " flagged by flag: "
	     << getFlag(flags) << std::endl << std::endl
	     << errorMessageSStream.str() << std::endl;
    die();
  }
  return _argvParserNullString;
}


inline std::string ArgvParser::set( bool& lhs, const std::vector<std::string>& flags ){

  if( !flags.size() )  die( "Programming error: ArgvParser given empty flags vector" );
  
  const std::string flagString = getFlag(flags);
  lhs = flagString.size();

  return flagString;
}


template <typename T>
inline std::string ArgvParser::setOrDie( T& lhs, const int& posArgIndex ){
  std::ostringstream errorMessageSStream;


  if(  hasArg( posArgIndex )  ){

    const std::string  arg  =  getArg( posArgIndex );

    if(   setFromString( lhs, arg, errorMessageSStream )   ){
      return arg;
    }
    errorOut << "Command Line Parsing Error when attempting to read arg #"
	     << posArgIndex << "\n\n"
	     << errorMessageSStream.str() << "\n";
    die();
  }

  // else posArgIndex not present.
  errorOut << "Command Line Parsing Error when attempting to read "
	   << posArgIndex
	   << "th argument: "
	   << "Perhaps not enough arguments were given?"
	   << std::endl;
  die();
  return _argvParserNullString; // please compiler.
}


template <typename T>
inline std::string ArgvParser::setOrDie( T& lhs, const std::vector<std::string>& flags ){

  if( !flags.size() )  die( "Programming error: ArgvParser given empty flags vector" );

  std::ostringstream errorMessageSStream;
  if(  hasOpt(flags)  ){
    std::string opt = getOpt(flags);
    if(   setFromString( lhs, opt, errorMessageSStream  )   ){
      return opt;
    }
    errorOut << "Command Line Parsing Error when attempting to read option: "
	     << getOpt(flags)
	     << " flagged by flag: "
	     << getFlag(flags) << "\n\n"
	     << errorMessageSStream.str() << "\n";
    die();
  }

  // else if argument matching flag not found, print error message and die
  _dieFlagNotFoundError( flags );

  return _argvParserNullString; // please compiler.
}



// *************** setFromString methods ***************
inline bool ArgvParser::setFromString( /***/ double&      lhs,
				       const std::string& rhsString,
				       /***/ std::ostringstream& errorMessageSStream ){

  if(   looksLikeDouble( rhsString.c_str() )   ){
    lhs = atof( rhsString.c_str() );
    return true;
  }
  // else error
  errorMessageSStream << "Command line parsing error: expected a double, but received: \""
		      << rhsString
		      << "\"";
  return false;
}


inline bool ArgvParser::setFromString( /***/ std::string& lhs,
				       const std::string& rhsString,
				       /***/ std::ostringstream& errorMessageSStream ){
  lhs = rhsString;
  return true; // string to string conversion always succeeds.
}


inline bool ArgvParser::setFromString( /***/ std::ifstream& ifs,
				       const std::string&   pathname,
				       /***/ std::ostringstream& errorMessageSStream ){

  if(   !ArgvParser::fileExists( pathname )   ){
    errorMessageSStream << "Error; tried to open file: \""
		<< pathname
		<< "\", but it does not exist"
		<< std::endl;
    return false;
  }
  ifs.open( pathname.c_str() );
  if( !ifs.is_open() ){
    errorMessageSStream << "Error when attempting to open file: \""
		<< pathname
		<< "\", appears to exist but could not be opened successfully"
		<< std::endl;
    return false;
  }
  return true;
}


inline bool ArgvParser::setFromString( /***/ std::istream*&      istreamPtrRef,
				       const std::string&        pathname,
				       /***/ std::ostringstream& errorMessageSStream ){

  if( pathname == useStdinIndicator() ){
    istreamPtrRef = &std::cin;
    return true;
  }

  if(   !ArgvParser::fileExists( pathname )   ){
    errorMessageSStream << "Error; tried to open file: \""
		<< pathname
		<< "\", but it does not exist"
		<< std::endl;
    return false;
  }

  istreamPtrRef = new std::ifstream;

  bool retVal = 
    setFromString( dynamic_cast<std::ifstream&>(*istreamPtrRef), pathname, errorMessageSStream );

  return retVal;
}



inline bool ArgvParser::setFromString( /***/ std::ofstream& ofs,
				       const std::string&   pathname,
				       /***/ std::ostringstream& errorMessageSStream ){

  ofs.open( pathname.c_str() );

  if( !ofs.is_open() ){
    errorMessageSStream << "Error when attempting to open file: \""
		<< pathname
		<< "\", appears to exist but could not be opened successfully"
		<< std::endl;
    return false;
  }

  return true;
}


inline bool ArgvParser::setFromString( /***/ unsigned int& lhs,
				       const std::string&  rhsString,
				       /***/ std::ostringstream& errorMessageSStream ){

  if(  !looksLikeUnsignedInt( rhsString )  ){
    errorMessageSStream << "Command line parsing error: expected an unsigned int, but received: \""
			<< rhsString
			<< "\"";
    return false;
  }

  sscanf( rhsString.c_str(), "%u", &lhs );

  return true;
}


inline bool ArgvParser::setFromString( /***/ long unsigned int& lhs,
				       const std::string&       rhsString,
				       /***/ std::ostringstream& errorMessageSStream ){
  if(  !rhsString.size()  ){
    die( "ArgvParser programming error, received null string argument where that should be impossible" );
  }

  if(  !looksLikeUnsignedInt( rhsString )  ){
    errorMessageSStream << "Command line parsing error: expected an unsigned int, but received: \""
			<< rhsString
			<< "\"";
    return false;
  }

  sscanf( rhsString.c_str(), "%lu", &lhs );

  return true;
}


inline bool ArgvParser::setFromString( int& lhs,
				       const std::string& rhsString,
				       std::ostringstream& errorMessageSStream ){
  if(  !rhsString.size()  ){
    die( "ArgvParser programming error, received null string argument where that should be impossible" );
  }

  if(  !looksLikeInt( rhsString )  ){
    errorMessageSStream << "Command line parsing error: expected an int, but received: \""
			<< rhsString
			<< "\"";
    return false;
  }

  sscanf( rhsString.c_str(), "%d", &lhs );

  return true;
}


inline bool ArgvParser::_setIndexToFirstMatchingFlag( /***/ int& argIndex,
						      const std::vector<std::string>& flags ){
  for( int i = 1; i < argc(); ++i ){
    if(   _cstrMatchesOneOf( _argv[i], flags )   ){
      argIndex = i;
      return true;
    }
  }
  return false;
}


inline bool ArgvParser::_setIndexToFirstMatchingOptFlag( int& argIndex,
							 const std::vector<std::string>& flags ){
  for( int i = 1; i < argc()-1; ++i ){
    if(   _cstrMatchesOneOf( _argv[i], flags )   ){
      argIndex = i;
      return true;
    }
  }
  return false;
}


inline bool ArgvParser::_cstrMatchesOneOf( const char*& s,
					   const std::vector<std::string>& strings ){
  for( size_t i = 0; i < strings.size(); ++i ){
    if(   !strcmp( s, strings[i].c_str() )   )  return true;
  }
  return false;
}


inline bool ArgvParser::hasFlag( const std::vector<std::string>& flags ){
  int argIndex;
  if(   _setIndexToFirstMatchingFlag( argIndex, flags )   ){
    markUsedAsFlagOrOpt( argIndex );
    return true;
  }
  return false;
}


inline bool ArgvParser::hasFlagDelete( const std::vector<std::string>& flags ){

  int argIndex;

  if(   _setIndexToFirstMatchingFlag( argIndex, flags )   ){
    _deleteArg( argIndex );
    return true;
  }
  return false;
}


inline void ArgvParser::_deleteArg( const int& argIndex ){
  if( argIndex > argc() ) die( "internal error: _deleteArg tried to remove and argIndex that was too big\n" );
  argUsed.erase           ( argUsed.begin()            + argIndex );
  argUsedAsFlagOrOpt.erase( argUsedAsFlagOrOpt.begin() + argIndex );
  for( int i = argIndex; i < argc() - 1; ++i ){
    _argv[i] = _argv[i+1];
  }
  --_argc;
}



inline bool ArgvParser::hasArg( const int& argIndex ){
  bool inRange;
  _argvIndexOfPositionalArg( argIndex, inRange );
  return inRange;
}


inline void ArgvParser::_diePosArgNotFoundError( const int& posArgIndex ){
  errorOut << "\nError: expected at least " << posArgIndex << " arguments\n";
  die();
}


inline void ArgvParser::_dieFlagNotFoundError( const std::vector<std::string>& flags ) const{

  if( flags.size() > 1 ){
    errorOut << "Command Line Parsing Error when attempting to find argument matching one of these flags:\n"
	     << "\"" << flags[0] << "\"";
    for( size_t i = 1; i < flags.size(); ++i )  errorOut << ", " << "\"" << flags[i] << "\"";
    errorOut << std::endl;
  }else{
    errorOut << "Command Line Parsing Error when attempting to find argument matching flag: \""
	     << flags[0]
	     << "\""
	     << std::endl;
  }

  die();
}


inline void
ArgvParser::_dieOptValueNotInPossibleValuesList( const std::string& optFlag,
						 const std::string& optValue,
						 const std::vector<std::string>& possibleValues ){
  if( !possibleValues.size() ){
    die( "Command line Parsing Programming error, ArgvParser getOpt method with limited set of possible values\n\
called with empty possible value list" );
  }
  // else possible value list is non-empty, but optValue is not found in it
  errorOut 
    <<  "Error. When parsing optFlag: \""  <<  optFlag  <<  "\": "
    <<  "opt value \""  <<  optValue  <<  "\"\n"
    <<  "is not one of the possible values:\n";
  for( size_t i = 0; i < possibleValues.size(); ++i ){
    errorOut  <<  "    \""  <<  possibleValues[i]  <<  "\"\n";
  }
  die();
}
  

inline bool ArgvParser::looksLikeInt( const char* const& s ){
  const char* r = s;
  if( !r )   return false;
  if( !*r )  return false;

  // first character can be a digit or '+' or '-'
  if( !isdigit(*r) && (*r != '+') && (*r != '-') )  return false;

  // following characters must be digits
  for( ++r; *r; ++r )  if( !isdigit(*r) )  return false;

  return true;
}


inline bool ArgvParser::looksLikeUnsignedInt( const char* const& s ){
  const char* r = s;
  if( !r )   return false;
  if( !*r )  return false;

  if( !isdigit( *r ) )  return false;

  for( ++r; *r; ++r )  if( !isdigit(*r) )  return false;

  return true;
}


inline bool ArgvParser::looksLikeDouble( const char* const& s ){
  const char* r = s;
  bool eSeen = false;
  bool eJustSeen = false;
  if( !*r ) return false; // empty string.
  if( *r == '-' ) ++r; // skip leading minus sign.
  for( ; *r; ++r ){ // loop until '.' or 'e' seen.
    if( isdigit(*r) ) continue;
    if( *r == '.' ) break;
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
    if( (*r == 'e' || *r == 'E') && !eSeen ){ // for example "-8.88178419700125e-16"
      eSeen = true;
      eJustSeen = true;
      continue;
    }
    return false;
  }
  return true;
}


inline void ArgvParser::_assertHasArg( const int& argIndex ){

  if( argIndex < 0 ){
    die( "Programming error: negative arg index passed to ArgvParser\n" );
  }
  if( argIndex >= _argvUnusedAsFlagOrOptCount() ){
    if( argIndex >= argc() ){
      // problem cannot be explained by shifting or flag-opt processing

      errorOut << "Command line parsing error: expected at least "
	       << argIndex
	       << " arguments, but only "
	       << _argvUnusedAsFlagOrOptCount()-1
	       << " arguments (excluding program name) were given\n";
      die();
    }

    if( argc() == argcOrig() ){
      //  No args have been shifted or deleted so problem seems to be with flag-opt processing

      errorOut << "Command line parsing error: expected at least "
	       << argIndex
	       << " arguments, but only "
	       << _argvUnusedAsFlagOrOptCount()-1
	       << " arguments (excluding program name) remain,"
	       << " after using "
	       << argc() - _argvUnusedAsFlagOrOptCount()
	       << " args as flags or flagged options.\n\n"
	       << "Args used as flags or flagged options are: "
	       << usedAsFlagOrOptArgsString() << "\n\n";
      die();
    }
    
      
    // Else if some args have been shifted or deleted.
    errorOut << "Command line parsing error: expected at least "
	     << argIndex
	     << " arguments, but only "
	     << _argvUnusedAsFlagOrOptCount()-1
	     << " arguments (excluding program name) remain,"
	     << " after using "
	     << argc() - _argvUnusedAsFlagOrOptCount()
	     << " arguments as flags or flagged options,"
	     << " and shifting or deleting"
	     << argcOrig() - argc()
	     << "arguments\n"
	     << "Args used as flags or flagged options are: "
	     << usedAsFlagOrOptArgsString() << "\n"
	     << "Shifted args are: "
	     << shiftedArgsString() << "\n\n";
    die();
  } // end if( argIndex >= _argvUnusedAsFlagOrOptCount() )
} // end _assertHasArg( const int& argIndex )



inline std::string ArgvParser::setCautiously( std::ofstream& ofs, const int& posArgIndex ){
  if(   hasArg( posArgIndex )  ){
    std::string pathname = getArg( posArgIndex );
    openCautiously( ofs, pathname );
    return pathname;
  }
  // else posArgIndex not present.
  return _argvParserNullString;
}



inline std::string ArgvParser::setCautiously( std::ofstream& ofs, const std::vector<std::string>& flags ){

  if( !flags.size() )  die( "Programming error: ArgvParser given empty flags vector" );

  std::string errorMessage;
  if(  hasOpt(flags)  ){
    std::string pathname = getOpt(flags);
    openCautiously( ofs, pathname );
    return pathname;
  }
  // else posArgIndex not present.
  return _argvParserNullString;
}


inline std::string ArgvParser::setCautiouslyOrDie( std::ofstream& ofs,
						   const int& posArgIndex ){
  if(   hasArg( posArgIndex )  ){
    std::string pathname = getArg( posArgIndex );
    openCautiously( ofs, pathname );
    return pathname;
  }
  // else posArgIndex not present.
  errorOut << "Command Line Parsing Error when attempting to read "
	   << posArgIndex
	   << "th argument: "
	   << "Perhaps not enough arguments were given?"
	   << std::endl;
  die();

  return _argvParserNullString; // please compiler.
}



inline std::string ArgvParser::setCautiouslyOrDie( std::ofstream& ofs,
						   const std::vector<std::string>& flags ){

  if( !flags.size() )  die( "Programming error: ArgvParser given empty flags vector" );

  std::string errorMessage;
  if(  hasOpt(flags)  ){
    std::string pathname = getOpt(flags);
    openCautiously( ofs, pathname );
    return pathname;
  }

  // else if argument matching flag not found, print error message and die
  _dieFlagNotFoundError( flags );

  return _argvParserNullString; // please compiler.
}



inline std::istream& ArgvParser::_getIstreamAux( const int& argIndex,
						 const bool& dieIfArgNotFound ){

  if(  hasArg( argIndex )  ){
    if( _argv[argIndex] == useStdinIndicator() ){
      return std::cin;
    }
    // else open file and return
    std::ifstream* ifsPtr = new std::ifstream; // never freed in current implementation.
    set( *ifsPtr, argIndex );
    return *ifsPtr;
  }

  if( dieIfArgNotFound ){
    _diePosArgNotFoundError( argIndex );
  }

  return std::cin;
}


inline std::istream* ArgvParser::_getIstreamPtrAux( const int&  argIndex,
						    const bool& dieIfArgNotFound ){
  
  std::istream& iStream(  _getIstreamAux( argIndex, dieIfArgNotFound )  );
  
  return &iStream;
}



/* if  opt flagged by FLAGS is not present or equals useStdinIndicator()
 *     return std::cin
 * otherwise
 *     attempt to open file stream and return reference to it. The
 *     created file pointer is never freed
 */
inline std::istream& ArgvParser::_getIstreamAux( const std::vector<std::string>& flags,
						 const bool&  dieIfArgNotFound ){
  if(   hasOpt( flags )   ){
    if( getOpt( flags ) == useStdinIndicator() ){
      return std::cin;
    }
    // else open file and return
    std::ifstream* ifsPtr = new std::ifstream; // never freed in current implementation.
    set( *ifsPtr, flags );
    return *ifsPtr;
  }

  if( dieIfArgNotFound ){
    _dieFlagNotFoundError( flags );
  }

  return std::cin;
}



inline void ArgvParser::open( std::ifstream& ifs, const std::string& pathname ){
  ifs.open( pathname.c_str() );
  GDB_ASSERTF( ifs.is_open(), "Error opening file: %s", pathname.c_str() );
}


// return pointer to open file, or std::cin if pathname = "-".
inline std::istream* ArgvParser::openIstream( const std::string& pathname ){
  if( pathname == "-" ){
    return &std::cin;
  }
  else{
    std::ifstream* ifsPtr = new std::ifstream( pathname.c_str() );
    GDB_ASSERTF( ifsPtr->is_open(),
		 "Error opening file: %s", pathname.c_str() );
    return ifsPtr;
  }
}


inline void ArgvParser::openCautiously( std::ofstream& ofs, const std::string& pathname ) const{
  if( !access(pathname.c_str(), F_OK) ){
    errorOut << "Error: file \"" << pathname << "\" already exists. remove it manually or choose another pathname\n";
    die();
  }
  ofs.open( pathname.c_str() );
  if( !ofs.is_open() ){
    errorOut << __FILE__ << ":" << __LINE__ << " Error opening file: \"" << pathname << "\"\n";
    die();
  }
}


inline void ArgvParser::closeIstream( std::istream* ifsPtr ){
  if(  ifsPtr == &std::cin  )   return;
  else                          dynamic_cast<std::ifstream*>(ifsPtr) -> close();
}
  
    


// *************** Exit functions *********************************
inline bool ArgvParser::die( const std::string message ) const{
  static const std::string premessage( "Command Line Parsing Error: " );
    if( message.size() ){
      errorOut << premessage << message << std::endl;
    }
    errorOut << usage() << std::endl;
#ifdef GDB_DEBUG
    std::cerr << "die() call trapped for gdb. hit ctl-c to break.\n";
    for(;;);  // Prevent termination. Useful to get backtrace in gdb.
#endif // GDB_DEBUG
    std::exit( -1 );

    return true; // dummy statement for compiler.
}


inline void ArgvParser::dieIfUnusedArgs() const{
    std::vector<std::string> unused = unusedArgs();
    if( unused.empty() ) return;
    printDoc();
    errorOut << "\nError: following args unparsed:";
    for( size_t i = 0; i < unused.size(); ++i ){
      errorOut << " " << unused[i];
    }
    errorOut << std::endl;
    die();
}


/* ********** "doc" (documentation) methods definitions ********** */
  /* **************************************************
     implementation notes about doc functions: printOptionDoc
     getDoc, setDoc.

     Information about doc option name, value pairs for
     various locations is held in docLocaleOptionNameValuePair.
     
     docLocaleOptionNameValuePair is a vector with one element
     per locale.

     Each element itself is vector of (optionName, optionValue) pairs.

     Roughly it looks like:

     "C",
        ( ("--help", "help message..."), ("--option", "option message...") ),
     "de_DE",
        ( ("--help", "Hilfe Anzeige..."), ("--option", "Wahlanzeige..." ) )
     ...

  */

  // used by printDoc()
  inline void ArgvParser::printOptionDoc( const std::string& optionName,
					  const std::string& locale ) const{
    std::string optionValue = getDoc( optionName, locale );
    if( !optionValue.size() )  return;
    errorOut << optionValue;
    if( optionValue[optionValue.size()-1] != '\n' )  errorOut << "\n";
    exit( 1 );
  }

  
  inline std::string ArgvParser::getDoc( const std::string& optionName,
					 std::string locale ) const{
    if( !docLocaleOptionNameValuePair.size() ) return "";

    // if locale not given, get from environment or default to "C"
    if( !locale.size() ){
      const char* const  envLANG = getenv( "LANG" );
      locale = std::string(  (envLANG ? envLANG : "C" )  );
    }

    // use linear search to look for registered locale matching LOCALE
    size_t localeIdx = 0;
    for( ; localeIdx < docLocaleOptionNameValuePair.size(); ++localeIdx ){
      if( locale == docLocaleOptionNameValuePair[localeIdx].first )  break;
    }

    // if locale not found, use "C" instead.
    if( localeIdx == docLocaleOptionNameValuePair.size() ){
      if( locale == "C" ) return "";  // avoid infinite loop.
      return getDoc( optionName, "C" );
    }
    // locale found so look for option name
    const std::vector<stringPairT>& stringPairVectorP = docLocaleOptionNameValuePair[localeIdx].second;
    for( size_t i = 0; i < stringPairVectorP.size(); ++i ){
      if( stringPairVectorP[i].first == optionName ){
	return stringPairVectorP[i].second;
      }
    }
    return "";
  }


  inline void ArgvParser::setDoc( const std::string& flagSpec,
				  const std::string& optionValue,
				  const std::string& locale ){
    _setDoc(  splitFlagSpec( flagSpec ),
	      optionValue,
	      locale  );
  }


  // call setDocOneOptionName for each option name
  inline void ArgvParser::_setDoc( const stringVecT&  optionNames,
				   const std::string& optionValue,
				   const std::string& locale ){
    for( size_t i = 0; i < optionNames.size(); ++i ){
      _setDocOneOptionName( optionNames[i], optionValue, locale );
    }
  }


  inline void ArgvParser::_setDocOneOptionName( const std::string& optionName,
						/***/ std::string  optionValue,
						const std::string& locale ){
    substituteVariables( optionValue );
    stringPairT optionNameValue = std::make_pair( optionName, optionValue );

    // find locale index
    size_t localeIdx = 0;
    for( ; localeIdx < docLocaleOptionNameValuePair.size(); ++localeIdx ){
      if( locale == docLocaleOptionNameValuePair[localeIdx].first )  break;
    }

    // if first time this locale seen.
    if( localeIdx == docLocaleOptionNameValuePair.size() ){ 
      std::vector<stringPairT> optionNameValueVector( 1, optionNameValue );
      docLocaleOptionNameValuePair.push_back( std::make_pair(locale, optionNameValueVector) );
      return;
    }
    // else locale already seen
    std::vector<stringPairT>& stringPairVectorP = docLocaleOptionNameValuePair[localeIdx].second;
    // check if option name already present, if so replace value.
    for( size_t i = 0; i < stringPairVectorP.size(); ++i ){
      if( stringPairVectorP[i].first == optionName ){
	stringPairVectorP[i].second = optionValue;
	return;
      }
    }
    // option name not found so push it onto vector.
    stringPairVectorP.push_back( optionNameValue );
    return;
  }



// *************** Utility functions *********************************
inline void ArgvParser::dumpArgvInfo() const{
  std::cout << "dumping args ( arg, argUsed, argUsedAsFlagOrOpt );\n";
  for( size_t i = 0; i < size(); ++i ){
    std::cout << "( " << _argv[i] << ", " << argUsed[i] << ", " << argUsedAsFlagOrOpt[i] << "); ";
    if( i && ((i%4) == 0) )  std::cout << std::endl;
  }

  std::cout << "\n\n";
}


inline std::vector<std::string> ArgvParser::unusedArgs() const{
  std::vector<std::string> retVal;
  for( int i = 1; i < _argc; ++i ){
    if( !argUsed[i] )  retVal.push_back( _argv[i] );
  }
  return retVal;
}


inline std::vector<std::string> ArgvParser::usedAsFlagOrOptArgs() const{
  std::vector<std::string> retVal;
  for( int i = 1; i < _argc; ++i ){
    if( argUsedAsFlagOrOpt[i] )  retVal.push_back( _argv[i] );
  }
  return retVal;
}


  // Assumes no args have been deleted from the middle of argv
inline std::vector<std::string> ArgvParser::shiftedArgs() const{
  std::vector<std::string> retVal;
  size_t numShiftedArgs = argcOrig() - argc();
  if( !numShiftedArgs )  return retVal;
  
  for( size_t i = 0; i < numShiftedArgs; ++i ){
    retVal.push_back(   std::string( _argvOrig[i] )   );
  }
  return retVal;
}


inline bool ArgvParser::isMember( const std::string& pipeSeparatedList,
				  const std::string& element ){
  std::vector<std::string> expandedList = splitOnVerticalBar( pipeSeparatedList );
  return(   std::find( expandedList.begin(), expandedList.end(), element )
	    != expandedList.end()   );
}

inline std::string ArgvParser::joinStringList( const std::string& delimiter,
					       const std::vector<std::string>& stringList ){
  std::string retVal;
  
  if( !stringList.size() )  return retVal;
  retVal += stringList[0];
  for( size_t i = 1; i < stringList.size(); ++i ){
    retVal +=  delimiter + stringList[i];
  }
  return retVal;
}


inline stringVecT ArgvParser::splitFlagSpec( const std::string& flagSpec ){
  GDB_ASSERTF(  flagSpec.size(),        "Null string passed as flagOrOptSpec"  );
  return splitOnVerticalBar( flagSpec, true );
}


inline void ArgvParser::assertLooksLikeFlag( const std::string& flag ){
  GDB_ASSERTF(  flag.size() > 1,  "Expected at least two characters in flag: \"%s\"",
		flag.c_str()  );
  GDB_ASSERTF(  flag[0] == '-',   "Expected first character to be \"-\" in flag: \"%s\"",
		flag.c_str()  );

  if( flag[1] == '-' ){
    GDB_ASSERTF(  flag.size() > 2,   "Invalid flag spec: \"%s\"", flag.c_str()  );
    GDB_ASSERTF(  isalpha(flag[2]),  "Invalid flag spec: \"%s\"", flag.c_str()  );
  }else{
    GDB_ASSERTF(  isalpha(flag[1]),  "Invalid flag spec: \"%s\"", flag.c_str()  );
  }
}


inline stringVecT ArgvParser::splitOnVerticalBar(  const std::string& s,
						   const bool         assertValidFlags  ){
  static const char delim = '|';

  stringVecT fields;
  if( !s.size() )  return fields;

  std::string::size_type delimSearchStartPos = 0;
  std::string::size_type delimPos;

  // push_back all but the final field
  for( delimPos = s.find( delim, delimSearchStartPos );
       delimPos != std::string::npos;
       delimPos = s.find( delim, delimSearchStartPos ) ){
    fields.push_back(   s.substr( delimSearchStartPos, delimPos - delimSearchStartPos )   );
    delimSearchStartPos = delimPos + 1;
  }

  // push_back last field
  fields.push_back(   s.substr( delimSearchStartPos )   );

  if( assertValidFlags ){
    std::for_each( fields.begin(), fields.end(), assertLooksLikeFlag );
  }

  return fields;
}



inline int ArgvParser::_argvIndexOfPositionalArg( const int& posArgIndex, bool& inRange ){
  int count = 0; // count of args not marked as UsedAsFlagOrOpt
  int argvIndex = 0;

  
  // set argvIndex to the posArgIndex_th arg, skipped
  // args marked as UsedAsFlagOrOpt
  while( argvIndex < argc() ){

    if( !argUsedAsFlagOrOpt[argvIndex] )  ++count;

    // At this point count should hold the number of
    // non-skipped args in _argv[0...argvIndex] inclusive.
    if( posArgIndex == count-1 ){ // posArgIndex counts from 0, but count counts from 1!
      argUsed[ argvIndex ] = true;
      inRange = true;
      return argvIndex;
    }

    ++argvIndex;
  }
    
  // fell out of loop because posArgIndex is not in range.
  inRange = false;
  return -1;
}



// replace "$0" with _argv[0] in usage.
inline void ArgvParser::substituteVariables( std::string& s ) const{
  std::string::size_type matchPos, searchStartPos = 0;
  const std::string programNameSpecialVariable( "$0" );
  while ((matchPos = s.find( programNameSpecialVariable, searchStartPos)) != std::string::npos){
    s.replace( matchPos, programNameSpecialVariable.size(), _argv[0] );
    searchStartPos = matchPos + programNameSpecialVariable.size();
  }
}    

  /* **********************************************************************
   *          END FUNCTION DEFINITIONS
   ***********************************************************************/


} // end namespace cbrc

#endif // defined _ARGVPARSER_HH_
