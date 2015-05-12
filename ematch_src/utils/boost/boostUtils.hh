/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2009/11/01 03:33:49 $
 *
 *  Description: some functions to wrap around boost to
 *               make code more compact and accessible to
 *               boost newbies
 *
 *
 */
#ifndef _BOOSTUTILS_HH
#define _BOOSTUTILS_HH
#include <iostream>
#include <boost/regex.hpp>


namespace cbrc{


  /* --------------- Typedefs --------------- */

  typedef  boost::match_results<std::string::const_iterator>  match_resultT;


  /* --------------- Function Declarations --------------- */

  // true iff PATTERN matches a prefix of TEXT
  bool matchesPrefixOf(  const std::string::const_iterator&   textBeg,
			 const std::string::const_iterator&   textEnd,
			 const boost::regex&                  pattern  );

  bool matchesPrefixOf(  const std::string&  text,
			 const boost::regex& pattern  );


  // if POINT points to start of match to REGEX, advance it to one past
  // the match. Otherwise leave it untouched.
  // returns true iff match found starting at POINT.
  bool advancePastMatchAtPoint( /* */ std::string::const_iterator& point,
				const std::string::const_iterator& textEnd,
				const boost::regex&                regex );


  
  /* --------------- Function Definitions --------------- */

  /* return true iff PATTERN matches a prefix of S */
  inline bool matchesPrefixOf(  const std::string&   text,
				const boost::regex&  pattern  ){

    return matchesPrefixOf( text.begin(), text.end(), pattern );
  }


  /* return true iff PATTERN matches a prefix of S */
  inline bool matchesPrefixOf(  const std::string::const_iterator&   textBeg,
				const std::string::const_iterator&   textEnd,
				const boost::regex&                  pattern  ){
    boost::smatch match;
    
    if(  boost::regex_search( textBeg, textEnd, match, pattern  )  ){
      return( !match.position() );
    }

    return false;
  }



  // if POINT points to start of match to REGEX, advance it to one past
  // the match. Otherwise leave it untouched.
  // returns true iff match found starting at POINT.
  inline bool advancePastMatchAtPoint( /* */ std::string::const_iterator& point,
				       const std::string::const_iterator& textEnd,
				       const boost::regex&                regex ){

    boost::match_results<std::string::const_iterator> whatMatched;

    bool matchWasFound =
      boost::regex_search( 
			  point, textEnd,
			  whatMatched,
			  regex,
			  boost::match_default | boost::match_continuous
			   );

    if( matchWasFound ){
      point += whatMatched.length(0);
    }

    return matchWasFound;
  }


} // end namespace cbrc
#endif // _BOOSTUTILS_HH
