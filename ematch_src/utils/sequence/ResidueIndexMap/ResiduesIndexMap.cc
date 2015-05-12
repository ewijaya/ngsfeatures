/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005,2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.12.7
 *  Last Modified: $Date: 2008/07/21 13:18:18 $
 *  
 *  Description: See header files.
 *
 */
#include "ResiduesIndexMap.hh"

namespace cbrc{


ResiduesIndexMap::ResiduesIndexMap( const ResidueIndexMap& rim ) : rim(rim){
  assert( sigma() > 1 );
  _maxStringLen = _computeMaxStringLen();
}



ResiduesIndexMap::indexT
ResiduesIndexMap::operator()( const ResidueIndexMap::indexT* const& r,
			      const size_t&  stringLen )
  const{

  assert( stringLen <= maxStringLen() );

  indexT residuesIndex = 0;
  indexT curSigitMultiplier = 1;

  for( size_t curSigitPos = stringLen - 1;
       curSigitPos < stringLen; // means: curSigitPos > -1
       --curSigitPos ){
    residuesIndex += r[curSigitPos] * curSigitMultiplier;
    curSigitMultiplier *= sigma();
  }

  return residuesIndex;
}



ResiduesIndexMap::indexT ResiduesIndexMap::operator()( const std::string& residues ) const{
  assert( residues.size() <= maxStringLen() );
  ResidueIndexMap::arrayT residueIndexArray = rim.toResidueIndices( residues );
  return this->operator()( &residueIndexArray[0], residues.size() );
}



ResidueIndexMap::vectorT
ResiduesIndexMap::toResidueIndicesVector( const ResiduesIndexMap::indexT& idx, 
					  const size_t&  stringLen ) const{
  assert( stringLen <= maxStringLen() );

  std::vector<ResidueIndexMap::indexT> retVal( stringLen );

  indexT curSigitMultiplier = _sigmaRaisedToPower( stringLen-1 );
  indexT remainder = idx;

  // fill in residueIndex values for return string, one position at a time.
  for( size_t curSigitPos = 0; curSigitPos < stringLen; ++curSigitPos ){
    retVal[curSigitPos] = remainder / curSigitMultiplier;
    remainder = remainder % curSigitMultiplier;
    curSigitMultiplier /= sigma();
  }

  return retVal;
}



std::string ResiduesIndexMap::toResidues( const ResiduesIndexMap::indexT& residuesIndex,
					  const size_t& stringLen ) const{
  assert( stringLen <= maxStringLen() );
  ResidueIndexMap::vectorT residueIndicesVector = toResidueIndicesVector( residuesIndex, stringLen );
  return rim.toResidues( residueIndicesVector );
}



/* ******* Private methods ******* */

// Return longest string length for which the index will not numerically overflow.
size_t ResiduesIndexMap::_computeMaxStringLen() const{
  indexT maxIdxPossibleWithCurStringLen = sigma() - 1;
  size_t curStringLen = 1;
  indexT curSigitMultiplier = sigma();

  // increment curStringLen, until doing so would case numerical overflow, then return.
  for( curStringLen = 1;
       /* Achtung! sole loop exit inside loop. */;
       ++curStringLen
       ){
    
    for( ResidueIndexMap::indexT resIdx = 1; resIdx < sigma(); ++resIdx ){

      // return if appending resIdx to string would result in numerical overflow.
      if(  (    maxIdxPossibleWithCurStringLen    +    resIdx     * curSigitMultiplier ) 
	   <= ( maxIdxPossibleWithCurStringLen    +    (resIdx-1) * curSigitMultiplier )  ){
	return curStringLen;
      }
    }

    // Prepare for next sigit (base sigma digit).
    maxIdxPossibleWithCurStringLen += (sigma()-1) * curSigitMultiplier;
    curSigitMultiplier *= sigma();
  }
}  


}; // end namespace cbrc
