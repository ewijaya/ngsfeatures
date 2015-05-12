/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.7
 *  Last Modified: $Date: 2009/05/14 02:20:22 $
 *
 *  Description: Class to represent tagId, probability pair.
 *
 */
#ifndef RECOUNTTAGIDPROBPAIR_HH_
#define RECOUNTTAGIDPROBPAIR_HH_
#include <iostream>
#include "recountTypes.hh"

namespace cbrc{

class RecountTagIdProbPair{
public:
  /* ********** CONSTRUCTORS ********** */
  RecountTagIdProbPair()
  {} // null constructor does nothing.

  /* ********** ACCESSORS ********** */
  const tagIdT&   id() const{  return _tagId;  }

  const probT&  prob() const{  return _prob;   }


  void set(  const tagIdT& tagId, const probT& prob  ){
    _tagId = tagId;  _prob = prob;
  }

  void setTagId( const tagIdT& tagId ){  _tagId = tagId;  }

  void setProb( const probT& prob ){  _prob = prob;  }

private:
  // object data
  tagIdT   _tagId;
  probT    _prob;
};


inline std::ostream& operator<<( std::ostream& oStream, const RecountTagIdProbPair& pair ){
  oStream << "(" << pair.id() << "," << pair.prob() << ")";
  return oStream;
}

} // end namespace cbrc
#endif // RECOUNTTAGIDPROBPAIR_HH_
