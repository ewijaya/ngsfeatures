/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.2.4
 *  Last Modified: $Date: 2009/07/06 08:24:58 $
 *
 *  Description: Given a set of items, each assigned a non-negative
 *               weight, and a knapsack of fixed capacity.
 *               Compute all of the subsets of the items which can
 *               fit in the knapsack.
 *
 *  Purpose: Created for use in the sequence tag count corrector
 *           project RECOUNT
 *
 */
#ifndef KNAPSACKENUMERATOR_HH_
#define KNAPSACKENUMERATOR_HH_
#include <iostream>
#include "utils/FLArray/FLEArray.hh"
#include "KnapsackObject.hh"
#include "KnapsackObjectVector.hh"

namespace cbrc{

class KnapsackEnumerator{
public:

  /* ********** TYPEDEFS ********** */
  typedef  KnapsackObject::weightT  weightT;
  typedef  KnapsackObject::    idT      idT;

  typedef  std::vector<idT>  idVecT;

  typedef  std::vector<idVecT>  objectCombinationsVecT;




  /* ********** CONSTRUCTORS ********** */
  KnapsackEnumerator(  const weightT&  capacity  )
    : _capacity   ( capacity    )
  {
    
  }


  /* ********** ACCESSORS ********** */

  // object parameters
  const double&     capacity()   {  return _capacity;      }

  // computed quantity
  const std::vector<idVecT>&  combinations()  {  return _combinations;  }

  const KnapsackObjectVector&  objectUniverse() const{
    return _objectUniverse;
  }

  /* ********** METHODS ********** */
  void computeCombinations( const KnapsackObjectVector&  objects  );

  friend void tryKnapsackEnumerator(); 

private:


  /* ********** OTHER PRIVATE METHODS ********** */
  void computeCombinationsAux(  const size_t&  dfsLevel  );


  /* ** object data ** */

  // object parameters
  weightT  _capacity;

  // quantity to compute
  std::vector<idVecT>  _combinations;

  // computation stack
  idVecT                idStack;   //  idStack[i] is id of ith object in current combination
  std::vector<weightT>  sumStack;  // sumStack[i] is sum of objects sumStack[0..i]

  // list of objects in universe, sorted by lightest first.
  KnapsackObjectVector  _objectUniverse;  

};

} // end namespace cbrc
#endif // KNAPSACKENUMERATOR_HH_
