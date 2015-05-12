/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.27
 *  Last Modified: $Date: 2009/11/02 05:21:30 $
 *
 *  Description: Vector of Knapsack objects
 *
 *  Purpose: Originally created for RECOUNT
 *
 */
#ifndef KNAPSACKOBJECTVECTOR_HH_
#define KNAPSACKOBJECTVECTOR_HH_
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/perlish/perlish.hh"
#include "KnapsackObject.hh"

namespace cbrc{

class KnapsackObjectVector{
public:

  /* --------------- TYPEDEFS --------------- */

  typedef  KnapsackObject::    idT      idT;
  typedef  KnapsackObject::weightT  weightT;


  /* --------------- CONSTRUCTORS --------------- */

  KnapsackObjectVector(){}

  KnapsackObjectVector( const KnapsackObjectVector& other )
    : _v( other._v )
  {}

  KnapsackObjectVector( const std::vector<KnapsackObject>& v )
    : _v( v )
  {}

  // Created by Edward
  KnapsackObjectVector( const std::vector<double>& logProbVec )
  { 
      makeFromWeightVector(logProbVec);
  }


  KnapsackObjectVector( std::istream& iStream ){
    readText_idsFrom0( iStream );
  }



  /* --------------- ACCESSORS --------------- */

  std::vector<KnapsackObject>::iterator begin(){  return _v.begin();  }
  std::vector<KnapsackObject>::iterator   end(){  return _v.  end();  }

  const std::vector<KnapsackObject>& v() const{  return _v;  }

  const KnapsackObject&  operator()(  const size_t&  i  )  const{
    assert(  i < size()  );
    return v()[i];
  }

  const size_t  size() const{  return v().size();  }


  /* ********** I/O METHODS ********** */
  void readText_idsFrom0( std::istream& iStream );
  void makeFromWeightVector(const std::vector<double>& logProbVec);

private:
  // object data
  std::vector<KnapsackObject>  _v;
};



inline
std::ostream& operator<<( std::ostream& oStream, const KnapsackObjectVector& objectVector ){

  for(  size_t i = 0;  i < objectVector.size();  ++i  ){
    if( i )   oStream  <<  " ";
    oStream  <<  objectVector(i);
  }

  return oStream;
}



} // end namespace cbrc


  

#endif // KNAPSACKOBJECTVECTOR_HH_
