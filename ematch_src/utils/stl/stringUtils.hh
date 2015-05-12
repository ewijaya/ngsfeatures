/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.16
 *  Last Modified: $Date: 2009/05/06 09:35:00 $
 *
 *  Description: Some utility functions for std::string
 *
 *  Caveat: This file unfinished and untested.
 */
#ifndef STRINGUTILS_HH_
#define STRINGUTILS_HH_
#include <iostream>
#include <string>
#include <vector>


namespace cbrc{



namespace stringUtils{

  /* ********** CONSTANTS ********** */
  static const std::string  null( "" );


  /* ********** FUNCTIONS ********** */

  // return true iff Q is prefix of T
  // named after java string method.
  inline bool startsWith( const std::string& t, const std::string& q ){

    if(  t.size() < q.size()  )   return false;
    
    std::string::const_iterator tItr = t.begin();

    for(  std::string::const_iterator qItr = q.begin();
	  qItr != q.end();
	  ++qItr, ++tItr ){
      if( *tItr != *qItr )  return false;
    }

    return true;
  }


  // return true iff Q is suffix of T
  // named after java string method.
  inline bool endsWith( const std::string& t, const std::string& q ){

    if(  t.size() < q.size()  )   return false;
    
    std::string::const_reverse_iterator  tItr  =  t.rbegin();

    for(  std::string::const_reverse_iterator qItr  =  q.rbegin();
	  qItr != q.rend();
	  ++qItr, ++tItr ){
      /* debug */ std::cout << __FILE__ << ":"     << __LINE__
			    << " comparing char: " << *qItr << std::endl;
      
      if( *qItr != *tItr )  return false;
    }

    return true;
  }


  /* replace all occurrences of oldSubstring in S with newSubstring
   * replacement is down sequentially from the start of S.
   * which determines what happens to overlapping occurences of oldSubstring.
   *
   * The number of replacements done is returned.
   */
  inline std::string::size_type
  stringReplaceAll( std::string& s,
		    const std::string& oldSubstring,
		    const std::string& newSubstring ){
    std::string::size_type curMatchStartPos = 0;
    size_t numReplacementsDone = 0;
  
    while(   (curMatchStartPos = s.find( oldSubstring ))  !=  std::string::npos   ){
      s.replace( curMatchStartPos, oldSubstring.size(), newSubstring );
      ++numReplacementsDone;
      curMatchStartPos += newSubstring.size();
    }
  
    return numReplacementsDone;
  } // end stringReplaceAll

};

} // end namespace cbrc
#endif // STRINGUTILS_HH_
