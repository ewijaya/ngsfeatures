/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.12
 *  Last Modified: $Date: 2009/09/20 08:08:55 $
 *
 *  Description: Represent counts of tags
 *
 *  Purpose: Created for RECOUNT project.
 *
 */
#ifndef RECOUNTTAGCOUNTVECTOR_HH_
#define RECOUNTTAGCOUNTVECTOR_HH_
#include <iostream>
#include "utils/FLArray/FLEArray.hh"
#include "SortedTagSet.hh"
#include "recountTypes.hh"

namespace cbrc{

class RecountTagCounts{
public:

  /* ********** TYPEDEFS ********** */
  typedef  FLENumArray<tagCountT>  arrayT;
  typedef  arrayT::const_iterator  const_iterator;

  /* ********** CONSTRUCTORS ********** */
  RecountTagCounts(  const size_t&  maxTagId  )
    : _counts( maxTagId )
  {
    zero();
  }


  RecountTagCounts(  const SortedTagSet&   tagId_to_seq,
		     /***/  std::istream&  tagCountsTextStream  );


  /* ********** INITIALIZATION RELATED METHODS ********** */

  void zero(){  _counts.zero();  }


  /* ********** ACCESSORS ********** */

  const arrayT&  counts() const {  return _counts;  }

  const size_t&  size() const {  return counts().size();  }

  const tagCountT&  operator()( const size_t& i ) const {  return counts()(i);  }

  /***/ tagCountT&  operator[]( const size_t& i ) const {  return _counts[i];   }

  const_iterator  constBegin() const {  return counts().constBegin();  }
  const_iterator  constEnd  () const {  return counts().constEnd  ();  }

  
private:
  // object data
  arrayT  _counts;
};


inline std::ostream& operator<<( std::ostream& oStream, const RecountTagCounts& tagCounts ){

  BOOST_FOREACH( const tagCountT& tagCount, tagCounts.counts() ){
    oStream << tagCount << std::endl;
  }
  return oStream;

}



} // end namespace cbrc
#endif // RECOUNTTAGCOUNTVECTOR_HH_
