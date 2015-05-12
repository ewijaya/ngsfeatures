/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.12
 *  Last Modified: $Date: 2009/09/25 02:51:23 $
 *  Description: See header file.
 */
#include "boost/lexical_cast.hpp"
#include "utils/stl/stlConstants.hh"
#include "utils/perlish/perlish.hh"
#include "RecountTagCounts_SortedTagSet.hh"

namespace cbrc{

  RecountTagCounts::RecountTagCounts(  const SortedTagSet&  tagId_to_seq,
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

      const SortedTagSet::tagSeqT&  tagSeq  =  tagId_to_seq.toTagSeq( fields[0] );

      GDB_ASSERTF(  tagId_to_seq.has( tagSeq ),
		    "Could not find id for tag \"%s\" from line: \"%s\"",
		    tagSeq.c_str(), curLine.c_str()  );

      _counts[ tagId_to_seq(tagSeq) ]  =  boost::lexical_cast<tagCountT>( fields[1] );
    }

  } // end constructor RecountTagCounts

} // end namespace cbrc

