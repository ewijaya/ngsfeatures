/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.22
 *  Last Modified: $Date: 2009/09/20 11:46:12 $
 *  Description: See header file.
 */
#include "TagSet.hh"

namespace cbrc{


/* --------------- PUBLIC ACCESSORS --------------- */


bool  TagSet::equal(   const size_t&  i,  const unpackedSeqT&  seqToCompare   ) const{

  if( size(i) != seqToCompare.size() )  return false;   // EXIT

  const size_t  sizeInBytes
    =  sigma4bitPackingUtils::numBytesNeededToStore( seqToCompare.size() );

  // allocate and initialization bit packed representation of seqToCompare
  byte* const  packedPtr   = new byte[sizeInBytes];
  sigma4bitPackingUtils::pack( packedPtr, seqToCompare );

  const bool  retVal  =  (  memcmp( a[i], packedPtr, sizeInBytes )  ==  0  );

  delete[] packedPtr;

  return  retVal;
}


// returns:  index of smallest sequence not lexically less than seqToMatch
//      or:  size() if seqToMatch is lexically greater than any sequence tag.
size_t  TagSet::upperBound(  const unpackedSeqT&  seqToMatch  ) const{

  byte* const  packedPtr
    = new byte[sigma4bitPackingUtils::numBytesNeededToStore( seqToMatch.size() )];

  sigma4bitPackingUtils::pack( packedPtr, seqToMatch );

  size_t  lower  =  0;
  size_t  upper  =  size();
  
  while(  lower  <  upper  ){
    const size_t  mid   =   lower  +  (upper - lower) / 2;
    if(  isLexicallyLess( mid, packedPtr, seqToMatch.size())  )   lower  =  mid + 1;
    else                                                          upper  =  mid;
  }

  delete[] packedPtr;

  return  upper;
}




/* --------------- I/O METHODS --------------- */

void TagSet::readFromTextStream( std::istream& iStream ){

  std::string line;

  DO_OR_DIEF(  perlish::slurpLine( line, iStream ),
	       "Input error. Empty tag list stream?"  );

  const std::vector<std::string> fields  =  perlish::split( "[ \t]+", line );

  ASSERTF(  fields.size() == 2,
	    "Expected 2 fields in first line: \"%s\"", line.c_str()  );

  perlish::fromString(  _size,      fields[0]  );

  perlish::fromString(  _totalSize, fields[1]  );

  ASSERTF(  totalSize() >= size(),
	    "Total size (%zu) should be at least as big as size (%zu)", totalSize(), size()  );


  /* --------------- Allocate Memory --------------- */
  a  =  new byte*[ size()+1 ];

  ASSERTF(  a,
	    "Failed to allocate array a of size (%zu)", size()+1  );

  mem  =  new byte[ totalSize() ];

  ASSERTF(  mem,
	    "Failed to allocate array mem of size (%zu)", totalSize()  );


  /* ---------- Declare vars for read line loop ---------- */
  ResidueIndexMap::vectorT  residueIndices;

  size_t  tagCount           =  0;
  size_t  totalResidueCount  =  0;

  /* --------------- Read line loop --------------- */
  while(  perlish::slurpLine( line, iStream )  ){
    residueIndexMap().assignResidueIndices( residueIndices, line );

    byte* const  curMemStart   =   mem + totalResidueCount;

    a[ tagCount ]  =  curMemStart;

    sigma4bitPackingUtils::pack( curMemStart, residueIndices );

    ++tagCount;
    totalResidueCount  +=  residueIndices.size();
  }


  /* ---------- Checksum Sequence and Residue Counts ---------- */
  ASSERTF(  tagCount == size(),
	    "Expected %zu tags, but only read in %zu", size(), tagCount  );

  ASSERTF(  totalResidueCount == totalSize(),
	    "Expected %zu total redisidues, but only read in %zu",
	    totalSize(), totalResidueCount  );


  // added final value to a[] to allow computation of size of final sequence
  a[ size() ]   =   mem + totalResidueCount;

  assertIsSorted();

} // end method readFromTextStream





void TagSet::assertIsSorted() const{

  for(  size_t i = 0;  i < size()-1;  ++i  ){

    GDB_ASSERTF(
		isLexicallyLess(  i, i+1 ),
		"tag: \"%s\" should come after tag:\"%s\"",
		toString( i ).c_str(),  toString( i+1 ).c_str()
		);

  }
}



bool  TagSet::isLexicallyLess(   const byte* const&  packedSeqPtr1,
				 const size_t&       seqLen1,   //  len in characters
				 const byte* const&  packedSeqPtr2,
				 const size_t&       seqLen2    //  len in characters
				 ) const{
  
  const size_t  minSizeInBytes
    =   std::min(  sigma4bitPackingUtils::numBytesNeededToStore( seqLen1 ),
		   sigma4bitPackingUtils::numBytesNeededToStore( seqLen2 )  );

  const int     valCmp   =   memcmp(  packedSeqPtr1,  packedSeqPtr2,  minSizeInBytes  );

  if(  valCmp  <  0  )   return  true;
  if(  valCmp  >  0  )   return  false;

  // sequences i, j are equal up to minSizeInBytes byte
  return(  seqLen1 < seqLen2  );
}

} // end namespace cbrc

