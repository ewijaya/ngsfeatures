/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.11.1
 *  Last Modified: $Date: 2009/11/02 06:11:57 $
 *
 *  Description: Enum like set of strings, contents specifiable by a single string
 *
 *  Purpose: Designed to be convenient for small string sets.
 *
 */
#ifndef STRINGSPECENUM_HH_
#define STRINGSPECENUM_HH_
#include <iostream>
#include <cassert>
#include "utils/stl/stlTypes.hh"

namespace cbrc{

class StringSpecEnum{
public:

  /* --------------- TYPEDEFS --------------- */



  /* --------------- CONSTRUCTORS --------------- */

  StringSpecEnum(  const std::string&  specString,
		   const std::string   delimiter  =  defaultDelimiter()
		   );


  /* --------------- ACCESSORS --------------- */

  const std::string&  specString()      const{  return _specString;      }

  const std::string&  delimiter()       const{  return _delimiter;       }

  const std::string&  value()           const{  return _value;           }

  const stringVecT&   possibleValues()  const{  return _possibleValues;  }

  size_t  size()  const{  return possibleValues().size();  }


  /* --------------- OTHER METHODS --------------- */
  
  // return index of VALUE,
  // or size() if VALUE is invalid.
  size_t index( const std::string& s ) const;


  bool operator==( const std::string& s ){
    assert(  index(s) < size()  );
    return( s == value() );
  }


  bool isInitialized() const{
    return( value().size() );
  }

  /* --------------- CLASS CONSTANTS --------------- */
  static const std::string&  defaultDelimiter(){
    static const std::string  _defaultDelimiter( "-" );
    return  _defaultDelimiter;
  }

private:
  void splitPossibleValues();

  /* --------------- OBJECT DATA --------------- */
  const std::string&  _specString;
  const std::string&  _delimiter;
  /***/ std::string   _value;
  /***/ stringVecT    _possibleValues;
};



inline std::ostream& operator<<( std::ostream& os, const StringSpecEnum& stringSpecEnum ){
  os  <<  stringSpecEnum.value();
  return os;
}



} // end namespace cbrc
#endif // STRINGSPECENUM_HH_
