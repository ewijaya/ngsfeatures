/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004,2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.4.12
 *  Last Modified: $Date: 2008/06/13 04:35:29 $
 *
 *  Description: Representation of a pair-wise alignment of biosequences.
 *
 */
#ifndef _ALIGNMENT_HH_
#define _ALIGNMENT_HH_
#define ALIGNMENT_OUTPUT_FORMATS text|html
#ifndef QQ
#define QQ(x) #x
#define Q(x) QQ(x)
#endif // not defined QQ
#include <string>
#include <algorithm>
#include <assert.h>
#include "utils/sequence/ResidueIndexMap/ResidueIndexMap.hh"
#include "utils/sequence/align/AminoScore.hh"

namespace cbrc{

class Alignment{
public:
  /* ***** Typedefs ***** */
  typedef int  alignScoreT;

  Alignment( const ResidueIndexMap& rim, const std::string& s0, const std::string& s1 ) 
    : rim(rim), s0(s0), s1(s1){}
  friend std::ostream& operator<<( std::ostream& os, const Alignment& al );
  void outputHtml( std::ostream& os ) const;

  // add mark for matching columns (or non-matching columns if markDifferent is set).
  void outputMarkSame( const std::string& outputFormat,
		       std::ostream& os,
		       const bool markDifferent = false,
		       const char markChar = '*' );

  void outputHtmlMarkSame( std::ostream& os, const bool markDifferent = false,
			   const char markChar = '*' ) const;
  void outputTextMarkSame( std::ostream& os, const bool markDifferent = false,
			   const char markChar = '*' ) const;
  static char indelChar(){ return('-'); }
  size_t size() const{ return s0.size(); }
  double percentIdentity() const;
  alignScoreT score( const AminoScore& aScore ) const;

  // For debugging. Assert strings making up alignment are
  // consistent with ResidueIndexMap and indelChar.
  void assertStringsValid() const; 
private:
  void reverse(){ 
    std::reverse( s0.begin(), s0.end() );
    std::reverse( s1.begin(), s1.end() );
  }
  const ResidueIndexMap& rim;
  std::string s0;
  std::string s1;
};

}; // end namespace cbrc
#endif // defined _ALIGNMENT_HH_
