/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.12
 *  Last Modified: $Date: 2009/09/28 02:44:18 $
 *
 *  Description: Represent counts of tags
 *
 *  Purpose: Created for RECOUNT project.
 *
 */
#ifndef RECOUNTTAGCOUNTVECTOR_HH_
#define RECOUNTTAGCOUNTVECTOR_HH_
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/FLArray/FLEArray.hh"
#include "TagSet.hh"
#include "recountTypes.hh"

namespace cbrc{

class RecountTagCounts{
public:


  /* --------------- TYPEDEFS --------------- */
  typedef  FLENumArray<tagCountT>  arrayT;

  typedef  arrayT::      iterator        iterator;
  typedef  arrayT::const_iterator  const_iterator;


  /* --------------- CONSTRUCTORS --------------- */

  RecountTagCounts()
    : _counts()
  {}

  RecountTagCounts(  const size_t&  maxTagID  )
    : _counts( maxTagID )
  {
    zero();
  }


  RecountTagCounts(  const  TagSet&        tagID_to_seq,
		     /***/  std::istream&  tagCountsTextStream  );


  
  /* --------------- INITIALIZATION RELATED METHODS --------------- */


  void zero(){  _counts.zero();  }


  void  setSize(   const size_t&  maxTagID   )
  {
    _counts.setSize(  maxTagID  );
  }
    

  void  assign(   const RecountTagCounts&  countsToCopy   ){
    _counts.assign(  countsToCopy.constBegin(),  countsToCopy.constEnd()  );
  }


  /* --------------- ACCESSORS --------------- */

  const arrayT&  counts() const {  return _counts;  }

  const size_t&  size() const {  return counts().size();  }

  const tagCountT&  operator()( const size_t& i ) const {  return counts()(i);  }

  /***/ tagCountT&  operator[]( const size_t& i ) const {  return _counts[i];   }

  /***/ iterator       begin() const {  return counts().     begin();  }
  /***/ iterator       end  () const {  return counts().     end  ();  }
  const_iterator  constBegin() const {  return counts().constBegin();  }
  const_iterator  constEnd  () const {  return counts().constEnd  ();  }



  /* --------------- OUTPUT METHODS --------------- */
  void print(   const TagSet&  tagID_to_seq,
	        std::ostream&  oStream  =  std::cout   ) const;


  /* --------------- ARITHMETIC METHODS --------------- */

  tagCountT  sum(){
    return counts().sum();
  }

  void operator*=(   const tagCountT&  multiplier  ){
    _counts *= multiplier;
  }


  // element-wise assignment  difference(i)  <==  minuend(i) - subtrahend(i)
  static
  void assignDifference(   /***/ RecountTagCounts&  difference,
			   const RecountTagCounts&  minuend,
			   const RecountTagCounts&  subtrahend   );

  // return  max_i  | count1[i] - count2[i] |
  static
  tagCountT maxAbsDiff(   const RecountTagCounts&  counts1,
			  const RecountTagCounts&  counts2   );


  // elementwise assign  sum[i]  <--  addend1[i] + addend2[i] + addend3[i]
  static
  void assignSum(  /***/ RecountTagCounts&  sum,
		   const RecountTagCounts&  addend1,
		   const RecountTagCounts&  addend2,
		   const RecountTagCounts&  addend3  );

    
  /* --------------- ASSERT METHODS --------------- */

  // return tag id and count of tag with smallest count,
  // in case of ties, return smallest id
  tagCountT  min() const{
    return  counts().min();
  }


private:
  // object data
  arrayT  _counts;
};


inline std::ostream& operator<<( std::ostream& oStream, const RecountTagCounts& tagCounts ){

  BOOST_FOREACH( const tagCountT& tagCount, tagCounts.counts() ){
    oStream  <<  tagCount  <<  std::endl;
  }
  return oStream;

}



} // end namespace cbrc
#endif // RECOUNTTAGCOUNTVECTOR_HH_
