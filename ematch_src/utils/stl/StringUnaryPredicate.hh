/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2005.11.10
 *
 *  Last Modified: $Date: 2005/11/10 07:55:08 $
 *  
 *  Description: Unary predicates for std::string.
 *
 */

#ifndef _STRINGUNARYPREDICATE_HH_
#define _STRINGUNARYPREDICATE_HH_
#include <string>

namespace cbrc{

class StringUnaryPredicate{
public:
  virtual bool operator()( const std::string& s ) const = 0;
  virtual ~StringUnaryPredicate(){}
private:
};

class StringUnaryPredicateAlwaysTrue : public StringUnaryPredicate{
public:
  bool operator()( const std::string& s ) const{ return true; }
};

const StringUnaryPredicateAlwaysTrue stringUnaryPredicateAlwaysTrue;

class StringUnaryPredicateAlwaysFalse : public StringUnaryPredicate{
public:
  bool operator()( const std::string& s ) const{ return false; }
};

const StringUnaryPredicateAlwaysFalse stringUnaryPredicateAlwaysFalse;

class StringUnaryPredicateContainsSimpleMatch : public StringUnaryPredicate{
public: 
  StringUnaryPredicateContainsSimpleMatch( const std::string& toMatch ) : toMatch(toMatch){}
  bool operator()( const std::string& s ) const{
    return( s.find( toMatch ) != std::string::npos );
  }
private:
  const std::string toMatch;
};

class StringUnaryPredicateContainsNotSimpleMatch : public StringUnaryPredicate{
public: 
  StringUnaryPredicateContainsNotSimpleMatch( const std::string& toMatch ) : toMatch(toMatch){}
  bool operator()( const std::string& s ) const{
    return( s.find( toMatch ) == std::string::npos );
  }
private:
  const std::string toMatch;
};


}; // end namespace cbrc
#endif // defined _STRINGUNARYPREDICATE_HH_

