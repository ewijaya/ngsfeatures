/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.12
 *  Last Modified: $Date: 2009/09/23 09:31:45 $
 *  Description: See header file.
 */
#include "boost/lexical_cast.hpp"
#include "utils/perlish/perlish.hh"
#include "RecountTagCounts.hh"

namespace cbrc{


/*  --------------- CONSTRUCTORS --------------- */

RecountTagCounts::RecountTagCounts(  const TagSet&        tagId_to_seq,
				     /***/ std::istream&  tagCountsTextStream  )
  : _counts( tagId_to_seq.size() ){

  _counts.zero();


  for( ;; ){

    const std::string curLine = perlish::slurpLine( tagCountsTextStream );

    if( !curLine.size() )  break;  // LOOP EXIT


    GDB_ASSERTF(  !boost::regex_search( curLine, boost::regex("\t\t") ),
		  "Input error. Fields should be separted by exactly one tab character." \
		  " line:\n%s\n", curLine.c_str()  );

    const stringVecT  fields  =  perlish::split( "\t", curLine );

    GDB_ASSERTF(  fields.size() == 2,
		  "Expected 2 tab separated fields, but got %s", curLine.c_str()  );

    const size_t  tagID  =  tagId_to_seq.getSerialNumberOrDie( fields[0] );

    _counts[ tagID ]  =  boost::lexical_cast<tagCountT>( fields[1] );
  }

} // end constructor RecountTagCounts



/* --------------- OUTPUT METHODS --------------- */

void RecountTagCounts::print(   const TagSet&  tagID_to_seq,
			        std::ostream&  oStream   ) const{
  for(  size_t i = 0;  i < size();  ++i  ){
    oStream  <<  tagID_to_seq.toString(i)  <<  "\t"  <<  (*this)(i)  <<  std::endl;
  }
}



/* --------------- ARITHMETIC METHODS --------------- */

// Class method.
void RecountTagCounts::assignDifference(   /***/ RecountTagCounts&  difference,
					   const RecountTagCounts&  minuend,
					   const RecountTagCounts&  subtrahend   ){
  /* -- Reality check -- */
  assert(   difference.size()  ==   minuend   .size()   );
  assert(   difference.size()  ==   subtrahend.size()   );

  RecountTagCounts::const_iterator  min   =   minuend   .begin();
  RecountTagCounts::const_iterator  sub   =   subtrahend.begin();

  for(   RecountTagCounts::iterator  diff   =   difference.begin();
	 diff  <  difference.end();
	 ++diff, ++min, ++sub
	 ){
    *diff  =  *min - *sub;
  }
} // end assignDifference.



// Class method.
void RecountTagCounts::assignSum(   /***/ RecountTagCounts&  sum,
				    const RecountTagCounts&  addend1,
				    const RecountTagCounts&  addend2,
				    const RecountTagCounts&  addend3   ){

  /* -- Reality check -- */
  assert(   sum.size()  ==   addend1.size()   );
  assert(   sum.size()  ==   addend2.size()   );
  assert(   sum.size()  ==   addend3.size()   );


  RecountTagCounts::const_iterator  addendPtr1   =   addend1.constBegin();
  RecountTagCounts::const_iterator  addendPtr2   =   addend2.constBegin();
  RecountTagCounts::const_iterator  addendPtr3   =   addend3.constBegin();
  for(  RecountTagCounts::iterator  sumPtr       =   sum.begin();
	sumPtr  <  sum.end();
	++sumPtr, ++addendPtr1, ++addendPtr2, ++addendPtr3
	){
    *sumPtr  =  *addendPtr1 + *addendPtr2 + *addendPtr3;
  }

} // end assignSum( sum, addend1, addend2, addend3 ).



// Class method
tagCountT RecountTagCounts::maxAbsDiff(   const RecountTagCounts&  counts1,
					  const RecountTagCounts&  counts2   ){

  tagCountT retVal  =  0.0;

  // reality check
  assert(   counts1.size()  ==   counts2.size()   );

  /***/ RecountTagCounts::const_iterator  ptr1  =  counts1.constBegin();
  for(  RecountTagCounts::const_iterator  ptr2  =  counts2.constBegin();
	ptr2  !=  counts2.constEnd();
	++ptr1, ++ptr2
	){

    const tagCountT  absDiff   =   fabs(  *ptr1 - *ptr2 );

    retVal  =  ( absDiff > retVal  )  ?  absDiff  :  retVal;

  }

  return retVal;
} // end maxAbsDiff.




} // end namespace cbrc

