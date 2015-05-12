/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.7
 *  Last Modified: $Date: 2009/05/22 12:13:48 $
 *  Description: See header file.
 */
#include "utils/gdb/gdbUtils.hh"
#include "RecountNeighborList.hh"

namespace cbrc{


/* ********** CONSTRUCTORS ********** */
void RecountNeighborList::set(  const tagIdVecT&  neighborIds,
			   const probVecT&   probs  )
{
    
  GDB_ASSERTF(  neighborIds.size() == probs.size(),
		"neighborIds.size() = %zu, ≠ probs.size() = %zu",
		neighborIds.size(), probs.size()  );

  _neighborVec.resize( neighborIds.size() );

  for(  size_t i = 0;  i < neighborIds.size();  ++i  ){
    _neighborVec[i].set( neighborIds[i], probs[i] );
  }
} // end constructor RecountNeighborList( tagId, neighborIds, probs )



bool RecountNeighborList::read( std::istream& iStream ){

  if(   (   iStream.eof()  == std::istream::traits_type::eof())
	|| (iStream.peek() == std::istream::traits_type::eof())   ){
    return false;
  }

  // read id
  iStream.read(  (char*) &_id, sizeof(_id)  );

  GDB_ASSERTF( !iStream.fail(), "Binary input failed in middle of record" );

  // read number of neighbors
  size_t listSize;
  iStream.read(  (char*) &listSize, sizeof(listSize)  );

  GDB_ASSERTF(  listSize <= neighborVec().max_size(),
		"listSize=%zu, too big for memory", listSize  );

  _neighborVec.resize( listSize );

  // read in neighborlist
  iStream.read(  (char*) &_neighborVec[0], listSize * sizeof( _neighborVec[0] )  );

  return true;
}


void RecountNeighborList::write( std::ostream& oStream ) const{

  // write id
  oStream.write(  (char*) &_id, sizeof(_id)  );

  // write number of neighbors
  const size_t  numNeighbors  =  neighborVec().size();
  oStream.write(  (char*) &numNeighbors, sizeof(numNeighbors)  );

  // write neighborlist
  oStream.write(  (char*) &_neighborVec[0],  numNeighbors * sizeof( _neighborVec[0] )  );
}


void RecountNeighborList::normalizeProbabilities(){

  probT sum = 0;
  
  BOOST_FOREACH(  const RecountTagIdProbPair& idProbPair,  neighborVec()  ){
    sum  +=  idProbPair.prob();
  }

  const probT sumReciprocal  =  1.0 / sum;

  BOOST_FOREACH(  RecountTagIdProbPair& idProbPair,  _neighborVec  ){
    idProbPair.setProb(  idProbPair.prob() * sumReciprocal  );
  }

} // end method normalizeProbabilities


} // end namespace cbrc	


