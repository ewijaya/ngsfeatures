/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.7
 *  Last Modified: $Date: 2009/05/22 12:13:40 $
 *
 *  Description: Represent id and neighbors (with probabilities) of a single tag.
 *
 *  Purpose: Created for the RECOUNT project.
 *
 */
#ifndef RECOUNTNEIGHBORLIST_HH_
#define RECOUNTNEIGHBORLIST_HH_
#include <iostream>
#include <boost/foreach.hpp>
#include "recountTypes.hh"
#include "RecountTagIdProbPair.hh"


namespace cbrc{

class RecountNeighborList{
public:
  /* ********** TYPEDEFS ********** */
  typedef  std::vector<RecountTagIdProbPair>  neighborVecT;

  typedef  neighborVecT::const_iterator  const_iterator;
  typedef  neighborVecT::iterator              iterator;

  /* ********** CONSTRUCTORS ********** */
  RecountNeighborList(){}

  RecountNeighborList(  const tagIdT&     id,
			const tagIdVecT&  neighborIds,
			const probVecT&   neighborProbs  )
    : _id   ( id    )
  {
    set( neighborIds, neighborProbs );
  }


  /* ********** ASSIGNMENT-LIKE METHODS ********** */
  void set(  const tagIdT&     id,
	     const tagIdVecT&  neighborIds,
	     const probVecT&   neighborProbs  ){
    _id    = id;
    set( neighborIds, neighborProbs );
  }


  void set(  const tagIdVecT&  neighborIds,
	     const probVecT&   neighborProbs  );


  /* ********** ACCESSORS ********** */
  const tagIdT&  id()  const  {  return _id;     }


  const_iterator begin() const{  return _neighborVec.begin();  }
  const_iterator end  () const{  return _neighborVec.end  ();  }


  /* ********** BINARY I/O ********** */
  bool read( std::istream& iStream );

  void write( std::ostream& oStream ) const;
  
  /* ********** OTHER METHODS ********** */
  void normalizeProbabilities();

private:
  const neighborVecT&  neighborVec() const {  return _neighborVec;  }

  // object data
  tagIdT  _id;

  neighborVecT  _neighborVec;
};


inline std::ostream&  operator<<(  /***/ std::ostream&         oStream,
				   const RecountNeighborList&  tagNode  ){

  oStream  <<  "id:"  <<  tagNode.id()  <<  " ";

  BOOST_FOREACH( const RecountTagIdProbPair& tagIdPair, tagNode ){
    oStream << tagIdPair << " ";
  }
  return oStream;
}


} // end namespace cbrc

#endif // RECOUNTNEIGHBORLIST_HH_
