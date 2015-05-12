/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.26
 *  Last Modified: $Date: 2009/07/06 08:27:06 $
 *
 *  Description: 
 *
 *  Purpose: (high level purpose or motivation)
 *
 */
#ifndef KNAPSACKOBJECT_HH_
#define KNAPSACKOBJECT_HH_
#include <iostream>

namespace cbrc{

class KnapsackObject{
public:
  /* ********** TYPEDEFS ********** */
  typedef  short unsigned         idT;
  typedef  double             weightT;

  /* ********** CONSTRUCTORS ********** */
  KnapsackObject( const idT& id, const weightT& weight )
    : _id(id), _weight(weight)
  {}

  /* ********** ACCESSORS ********** */
  
  const     idT&     id() const{  return _id;      }

  const weightT& weight() const{  return _weight;  }


  /* ********** COMPARISONS ********** */
  
  bool operator<( const KnapsackObject& otherObject ) const{
    return(  (   weight() < otherObject.weight()  )  ?  true
	     : ( weight() > otherObject.weight()  )  ?  false
	     : /* else */         ( id() < otherObject.id() )
	     );
  }

private:
  // object data
  idT          _id;
  weightT  _weight;
};



inline std::ostream& operator<<( std::ostream& oStream, const KnapsackObject& object ){

  oStream  <<  "("  <<  object.id()  <<  ","  <<  object.weight()  <<  ")";
  return oStream;
}


} // end namespace cbrc
#endif // KNAPSACKOBJECT_HH_
