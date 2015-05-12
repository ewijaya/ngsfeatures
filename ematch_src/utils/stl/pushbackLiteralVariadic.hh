/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2008.1.5
 *  Last Modified: $Date: 2009/06/02 07:52:18 $
 *
 *  Description: Function to push a list of literals onto a vector
 *
 *  Achtung!  This code is not very useful as is. I keep it because
 *            it illustrates a (sophisticated?) use of macros with
 *            variadic functions.
 *            
 *
 *  Usage: std::vector<unsigned> vec;
 *         vec.pushbackLiteral( 2, 3, 9, 10 );
 * 
 *  Implementation Issues:
 *
 *      What kind of killed this idea is that this function needs a sentinel that
 *      works for all of the literal types. The only value I could find that is
 *      common to the types I want to support is "0.0". This means that any list
 *      containing an actual 0, will be truncated...
 *
 *      I first tried to use templates instead of the macro _PUSHBACK_LITERAL_FUNCTION_GENERATOR
 *      but I was not able to get g++ to compile the appropriate instantions and instead
 *      always got "reference not found" linker errors.
 *                       
 *      Using preprocessor is elegant because it avoids the need for explicit
 *      sentinels -- but it puts "pushbackLiteral" in the global namespace.
 *
 *  Related Classes
 *      "toVector.hh" provides a similar function, but just for strings, in which case
 *      using NULL as a sentinel is okay.
 *
 */
#ifndef _PUSHBACKLITERALVARIADIC_HH
#define _PUSHBACKLITERALVARIADIC_HH
#include <iostream>
#include <string>
#include <vector>
#include <cstdarg>
#define pushbackLiteral( ... ) cbrc::_pushbackLiteral( __VA_ARGS__, 0.0 )  // append sentinel!


namespace cbrc{

#define _PUSHBACK_LITERAL_FUNCTION_GENERATOR( elemToT, elemFromT )              \
/* The end of the variable length argument list must end with a NULL sentinel*/ \
inline void _pushbackLiteral(  /***/ std::vector<elemToT>& vec,		        \
			       const elemFromT&  elem0, ...  ){                 \
  vec.push_back( elem0 );                                                       \
                                                                                \
  /* ******* now process optional arguments ******* */                          \
  va_list optionalArgs;                                                         \
  va_start( optionalArgs, elem0 );                                              \
                                                                                \
  elemFromT elem;                                                               \
                                                                                \
  for( ;; ){								        \
    elem = va_arg( optionalArgs, elemFromT );                                   \
    if(  elem == 0  )  break;  /* LOOP EXIT */			\
    vec.push_back( elem );                                                      \
  }                                                                             \
  va_end( optionalArgs );						        \
}


_PUSHBACK_LITERAL_FUNCTION_GENERATOR( double,   double   )

_PUSHBACK_LITERAL_FUNCTION_GENERATOR( int,      int      )

_PUSHBACK_LITERAL_FUNCTION_GENERATOR( unsigned, unsigned )


} // end namespace cbrc.
#endif // _PUSHBACKLITERALVARIADIC_HH
