/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.10
 *  Last Modified: $Date: 2009/06/03 08:35:30 $
 *  Description: See header file.
 */
#include "HammingDistanceComputer.hh"

namespace cbrc{

size_t HammingDistanceComputer
::computeBounded( const LabeledSequence& seq0,
		  const LabeledSequence& seq1,
		  const double&          threshold
		  ){

  const size_t&  len0  =  seq0.length();
  const size_t&  len1  =  seq1.length();


  const size_t shorterLen        =  std::min( len0, len1 );
  size_t numMismatchesSeenSoFar  =  std::max( len0, len1 ) - shorterLen;

  // if lengths are more different than THRESHOLD, give up immediately.
  if( numMismatchesSeenSoFar >= threshold ){
    return numMismatchesSeenSoFar;
  }

  const ResidueIndexMap::arrayT& querySeq = seq0.residueIndices();
  const ResidueIndexMap::arrayT&   curSeq = seq1.residueIndices();

  for( size_t i = 0; i < shorterLen; ++i ){

    if(  querySeq[i]  !=  curSeq[i]  ){
      if(  ++numMismatchesSeenSoFar  >=  threshold  ){
	return numMismatchesSeenSoFar;
      }
    }
  }

  return numMismatchesSeenSoFar;
}


size_t HammingDistanceComputer
::computeBoundedGapAtEnd( const std::string&  seq0,
			  const std::string&  seq1,
			  const double&       threshold
			  ){

  const size_t&  len0  =  seq0.size();
  const size_t&  len1  =  seq1.size();


  const size_t shorterLen        =  std::min( len0, len1 );
  size_t numMismatchesSeenSoFar  =  std::max( len0, len1 ) - shorterLen;


  // if lengths are more different than THRESHOLD, give up immediately.
  if( numMismatchesSeenSoFar >= threshold ){
    return numMismatchesSeenSoFar;
  }

  for( size_t i = 0; i < shorterLen; ++i ){

    if(  seq0[i]  !=  seq1[i]  ){
      if(  ++numMismatchesSeenSoFar  >=  threshold  ){
	return numMismatchesSeenSoFar;
      }
    }
  }

  return numMismatchesSeenSoFar;
}



size_t HammingDistanceComputer
::computeBoundedGapAtBeg( const LabeledSequence& seq0,
			  const LabeledSequence& seq1,
			  const double&          threshold
		  ){

  const size_t&  len0  =  seq0.length();
  const size_t&  len1  =  seq1.length();


  const size_t shorterLen        =  std::min( len0, len1 );
  const size_t longerLen         =  std::max( len0, len1 );
  const size_t lenDiff           =  longerLen - shorterLen;

  size_t numMismatchesSeenSoFar  =  lenDiff;

  // if lengths are more different than THRESHOLD, give up immediately.
  if( numMismatchesSeenSoFar >= threshold ){
    return numMismatchesSeenSoFar;
  }

  const ResidueIndexMap::arrayT& querySeq = seq0.residueIndices();
  const ResidueIndexMap::arrayT&   curSeq = seq1.residueIndices();

  ResidueIndexMap::arrayT::const_iterator queryItr;
  ResidueIndexMap::arrayT::const_iterator curSeqItr;

  if( len0 > len1 ){
    queryItr  = querySeq.constBegin();
    curSeqItr = curSeq.constBegin() + lenDiff;
  }else{
    queryItr  = querySeq.constBegin() + lenDiff;
    curSeqItr = curSeq.constBegin();
  }    

  for(  /* no init */;  queryItr != querySeq.constEnd();  ++queryItr, ++curSeqItr  ){
    if( *queryItr != *curSeqItr ){
      if(  ++numMismatchesSeenSoFar  >=  threshold  ){
	return numMismatchesSeenSoFar;
      }
    }
  }

  GDB_ASSERTF(  curSeqItr  ==  curSeq.constEnd()  );

  return numMismatchesSeenSoFar;
}


size_t HammingDistanceComputer
::computeBoundedGapAtBeg( const std::string&  seq0,
			  const std::string&  seq1,
			  const double&       threshold
			  ){

  const size_t&  len0  =  seq0.size();
  const size_t&  len1  =  seq1.size();


  const size_t shorterLen        =  std::min( len0, len1 );
  const size_t longerLen         =  std::max( len0, len1 );
  const size_t lenDiff           =  longerLen - shorterLen;

  size_t numMismatchesSeenSoFar  =  lenDiff;

  // if lengths are more different than THRESHOLD, give up immediately.
  if( numMismatchesSeenSoFar >= threshold ){
    return numMismatchesSeenSoFar;
  }

  const std::string*  shorterPtr;
  const std::string*  longerPtr;

  if( len0 > len1 ){
    shorterPtr  =  &seq0;
    longerPtr   =  &seq1;
  }else{
    shorterPtr  =  &seq1;
    longerPtr   =  &seq0;
  }    

  std::string::const_iterator  shorterItr  =  shorterPtr->begin();
  std::string::const_iterator   longerItr  =   longerPtr->begin() + lenDiff;

  std::string::const_iterator  shorterEnd  =  shorterPtr->end();

  for(  /* no init */;  shorterItr != shorterEnd;  ++shorterItr, ++longerItr ){
    if( *shorterItr != *longerItr ){
      if(  ++numMismatchesSeenSoFar  >=  threshold  ){
	return numMismatchesSeenSoFar;
      }
    }
  }

  GDB_ASSERTF(  longerItr  ==  longerPtr->end(),  "end of sequences did not match"  );

  return numMismatchesSeenSoFar;
}


} // end namespace cbrc

