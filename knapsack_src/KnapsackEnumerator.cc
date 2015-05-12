/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul B. Horton, All rights reserved.
 *  Creation Date: 2009.2.4
 *  Last Modified: $Date: 2009/07/06 08:25:07 $
 *  Description: See header file.
 */
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "KnapsackEnumerator.hh"

namespace cbrc{

void KnapsackEnumerator::
computeCombinations(  const KnapsackObjectVector&  objects  ){

  /* ***** Initialize global variables used for search ***** */
  _combinations.clear();

  _objectUniverse = objects;

  std::stable_sort(  _objectUniverse.begin(),
		     _objectUniverse.end()  );

  idStack .resize(  objectUniverse().size()  );
  sumStack.resize(  objectUniverse().size()  );

  for(  idT id = 0;  id < objectUniverse().size();  ++id  ){
    const weightT&  curWeight  =  objectUniverse()(id).weight();
    if(  curWeight  <=  capacity()  ){
      idStack [0]  =  id;
      sumStack[0]  =  curWeight;
      computeCombinationsAux( 0 );
    }
    else{
      break;
    }
  }

  // just a stub for now.
} // end computeCombinations.


/*
 *  idStack[0..dfsLevel] holds current combination
 * sumStack[dfsLevel]    holds sum of idStack[0..dfsLevel]
 */
void KnapsackEnumerator::
computeCombinationsAux(  const size_t&  dfsLevel  ){

  // Save current combination
  _combinations.push_back( idStack );
  _combinations[ _combinations.size() - 1 ].resize( dfsLevel+1 );


  /* ***** Continue Depth First Search at next level ***** */

  const idT    nextLevel  =  dfsLevel + 1;
  const idT&      prevId  =  idStack[dfsLevel];
  const weightT&  curSum  =  sumStack[dfsLevel];


  for(  size_t id = prevId + 1;  id < objectUniverse().size();  ++id  ){

    const weightT  newSum  =  curSum + objectUniverse()( id ).weight();

    if(  newSum  <=  capacity()  ){
      idStack [ nextLevel ]  =  id;
      sumStack[ nextLevel ]  =  newSum;
      computeCombinationsAux( nextLevel );
    }else{
      break;
    }
  }
} // end method computeCombinationsAux.

} // end namespace cbrc

