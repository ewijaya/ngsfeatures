/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.11
 *  Last Modified: $Date: 2006/09/12 05:00:54 $
 *
 *  Description: Contain contents of transfac text file line.
 *
 */
#ifndef _TRANSFACTEXTLINE_HH
#define _TRANSFACTEXTLINE_HH
#include <iostream>

namespace cbrc{

class TransfacTextLine{
public:
  TransfacTextLine( const std::string& line ){ set(line); }
  TransfacTextLine() : _label( "" ){}  // null constructor indicates empty line.
  TransfacTextLine( const TransfacTextLine& transfacLine ) 
    : _label(transfacLine._label),
      _content(transfacLine._content )
  {}
  void set( const std::string& line ); // parse line and set label and content accordingly.
  bool empty() const{   return( !_label.size() );   }
  std::string label() const{ return _label; }
  std::string content() const{ return _content; }
private:
  std::string _label; // e.g. VV, etc.
  std::string _content; // excludes label and white space separation label and line.
};

std::ostream& operator<<( std::ostream& os, const TransfacTextLine& ttl );

}; // end namespace cbrc
#endif // _TRANSFACTEXTLINE_HH
