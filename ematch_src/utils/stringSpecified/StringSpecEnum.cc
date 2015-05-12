/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.11.1
 *  Last Modified: $Date: 2009/11/02 06:12:04 $
 *  Description: See header file.
 */
#include <algorithm>
#include <boost/foreach.hpp>
#include "StringSpecEnum.hh"

namespace cbrc{

  StringSpecEnum::StringSpecEnum(  const std::string&  specString,
				   const std::string   delimiter
				 )
    : _specString(specString), _delimiter(delimiter), _value( "" )
{
  splitPossibleValues();
  
  // confirm no empty string in sSpec.
  BOOST_FOREACH(   const std::string&  valueString,   possibleValues()   ){
    assert( valueString.size() );
  }

  // confirm each string in specString is unique
  stringVecT possibleValuesCopy;
  std::copy( possibleValues().begin(), possibleValues().end(), possibleValuesCopy.begin() );
  
  stringVecT::const_iterator lastUniqPos
    =  std::unique( possibleValuesCopy.begin(), possibleValuesCopy.end() );

  assert( lastUniqPos == possibleValuesCopy.end() );
}


size_t StringSpecEnum::index( const std::string& s ) const{

  stringVecT::const_iterator pos
    =  std::find( possibleValues().begin(), possibleValues().end(), s );

  return(  pos - possibleValues().begin()  );
}






void StringSpecEnum::splitPossibleValues(){

  assert( specString().size() );

  size_t prvDelimEnd = 0;

  // pick up all but final field
  for( size_t i = 0; i <= specString().size() - delimiter().size(); /* progress in loop */ ){
    if(   delimiter()  ==  specString().substr( i, delimiter().size() )   ){
      const size_t      fieldLength  =  i - prvDelimEnd;
      const std::string field  = specString().substr( prvDelimEnd, fieldLength );
      _possibleValues.push_back(  field  );
      prvDelimEnd = i;
      i += delimiter().size();
    }else{
      ++i;
    }
  }

  // pick up final field
  const size_t      fieldLength  =  specString().size() - prvDelimEnd;
  const std::string field  = specString().substr( prvDelimEnd, fieldLength );
  _possibleValues.push_back( field );

}





} // end namespace cbrc

