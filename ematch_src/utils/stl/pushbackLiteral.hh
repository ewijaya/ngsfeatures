/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2008.1.5
 *  Last Modified: $Date: 2009/06/02 07:52:36 $
 *
 *  Description: Function to push a list of literals onto a vector
 *
 *  Usage: std::vector<unsigned> vec;
 *         pushbackLiteral( vec, "3 9 10 29" );
 * 
 *  Implementation Issues:
 *      This file uses the cpp preprocessor to implement something like template functions
 *      I first tried using real templates, but had trouble getting
 *      g++ to compile the appropriate instantions and instead always got
 *      "reference not found" linker errors.
 *                       
 *  Related classes:
 *      pushbackLiteralVariadic, attempts to implement similar functionality using
 *      variadic functions.
 *
 */
#ifndef _PUSHBACKLITERAL_HH
#define _PUSHBACKLITERAL_HH
#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include "utils/perlish/perlish.hh"


namespace cbrc{

// do reality check on valuesString
static void assertStringLooksOkay( const std::string& valuesString ){
  GDB_ASSERTF(  valuesString.find( ',' ) == std::string::npos,
		"Commas not allowed in valuesString, valuesString was: \"%s\"",
		valuesString.c_str()  );
}


#define _PUSHBACK_LITERAL_FUNCTION_GENERATOR( elemT )                       \
inline void pushbackLiteral(  /***/ std::vector<elemT>& vec,                \
			      const std::string& valuesString  ){           \
                                                                            \
  assertStringLooksOkay( valuesString );                                    \
                                                                            \
  const stringVecT valueStrings  =                                          \
      perlish::split( boost::regex(" +"), valuesString );		    \
                                                                            \
  vec.reserve( vec.size() + valueStrings.size() );                          \
                                                                            \
  for(  stringVecT::const_iterator itr = valueStrings.begin();              \
        itr != valueStrings.end();                                          \
        ++itr  ){                                                           \
    vec.push_back( boost::lexical_cast<elemT>(*itr) );			    \
  }                                                                         \
}


  

/* ***** Define pushbackLiteral for common types ***** */
_PUSHBACK_LITERAL_FUNCTION_GENERATOR( double   )
_PUSHBACK_LITERAL_FUNCTION_GENERATOR( int )
_PUSHBACK_LITERAL_FUNCTION_GENERATOR( unsigned )



} // end namespace cbrc.
#endif // _PUSHBACKLITERAL_HH
