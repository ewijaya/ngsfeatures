/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.22
 *  Last Modified: $Date: 2009/09/12 02:49:58 $
 *  Description: See header file.
 */
#include "SortedTagSet.hh"

namespace cbrc{


SortedTagSet::tagSeqT  SortedTagSet::toTagSeq( const std::string& s ) const{

    try{
      return(
	     Sigma4FLArray(  residueIndexMap().toResidueIndicesVector( s )  )
	     );
    }
    catch(  std::invalid_argument err ){
      GDB_DIEF( "Error %s when converting string \"%s\"", err.what(), s.c_str() );
    }
    return * (tagSeqT*) NULL;  //  Dummy statement to please compiler.
}



/* ********** I/O METHODS ********** */

void SortedTagSet::readFromTextStream( std::istream& iStream ){

  std::string line;

  DO_OR_DIEF(  perlish::slurpLine( line, iStream ),
	       "Input error. Empty tag list stream?"  );

  size_t numTags;
  
  perlish::fromString( numTags, line );


  /* ***** Initialize Vector and Trim Size ***** */
  ResidueIndexMap::vectorT  residueIndices;

  v.resize( numTags );


  for(  size_t tagCount = 0;  tagCount < numTags;  ++tagCount  ){

    DO_OR_DIEF(  perlish::slurpLine( line, iStream ),
		 "End of file after only %zu out of %zu total tags read",
		 tagCount, numTags  );

    residueIndexMap().assignResidueIndices( residueIndices, line );

    v[tagCount]  =  residueIndices;
    
  }


  DO_OR_DIEF(  !perlish::slurpLine( line, iStream ),
	       "Extra unexpected line: \"%s\" found after tags", line.c_str()  );

  assertIsSorted();
} // end method readFromTextStream



void SortedTagSet::assertIsSorted() const{
  for(  size_t i = 0;  i < v.size()-1;  ++i  ){
    GDB_ASSERTF(  v[i] < v[i+1],
		  "List not sorted or not unique. element %zu is not less than the following element", i  );
  }
}


} // end namespace cbrc

