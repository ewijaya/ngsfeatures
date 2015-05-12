/*
 *  Author: "Paul B. Horton"
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.1.7
 *
 *  Last Modified: 2005.1.7
 *  
 *  Description: Represent an html anchor.
 *
 *  Purpose: Originally created for the WoLF PSORT web site.
 *
 */

#ifndef _ANCHOR_HH_
#define _ANCHOR_HH_

namespace cbrc{

namespace html{

class Anchor{
public:
  Anchor( const std::string contents ) : contents(contents){}
  void setTitle( const std::string title ){ this->title = title; }
  void setName( const std::string name ){ this->name = name; }
  void setHref( const std::string href ){ this->href = href; }
  friend std::ostream& operator<<( std::ostream& os, const Anchor& a );
private:
  std::string contents;
  std::string title;
  std::string href;
  std::string name;
};

inline std::ostream& operator<<( std::ostream& os, const Anchor& a ){
  os << "<A";
  if( a.name.size() )   os << " name=\"" << a.name << "\"";
  if( a.title.size() )  os << " title=\"" << a.title << "\"";
  if( a.href.size() )  os << " href=\"" << a.href << "\"";
  os << ">" << a.contents << "</A>";
  return os;
}

};

}; // end namespace cbrc

#endif // defined _ANCHOR_HH_

