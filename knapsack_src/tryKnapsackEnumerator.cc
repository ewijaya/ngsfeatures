/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul B. Horton, All rights reserved.
 *  Creation Date: 2009.2.4
 *  Last Modified: $Date: 2009/07/06 08:25:19 $
 *
 *  Purpose: try code involving KnapsackEnumerator
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../KnapsackObjectVector.hh"
#include "../KnapsackEnumerator.hh"


typedef  cbrc::KnapsackEnumerator::weightT  weightT;

weightT        arg_capacity;
std::istream*  arg_objectStreamPtr;


namespace cbrc{
  void tryKnapsackEnumerator(){

    KnapsackEnumerator sack( arg_capacity );

    //std::cout << "capacity is: " << sack.capacity() << std::endl;

    KnapsackObjectVector objects( *arg_objectStreamPtr );

    //std::cout  <<  "objects are:  "   <<  objects  <<  std::endl;


    sack.computeCombinations( objects );

    //std::cout  <<  "Sorted objects are:\n";

    //std::cout << "Array sack.objectUniverse():" << std::endl;
    //for( size_t i = 0; i < sack.objectUniverse().size(); ++i ){
    //  if( i ) std::cout << " ";
    //  std::cout << sack.objectUniverse()(i);
    //}
    //std::cout << std::endl;


    //std::cout  <<  "Possible combinations are:\n";

    for( size_t i = 0; i < sack.combinations().size(); ++i ){
      const KnapsackEnumerator::idVecT&  curCombination  =  sack.combinations()[i];
      for( size_t j = 0; j < curCombination.size(); ++j ){
	if( j )  std::cout  <<  ",";
	std::cout  <<  sack.objectUniverse()( curCombination[j] ).id();
      }
      std::cout  <<  std::endl;
    }
    
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "capacity knapsackObjectFile" );


  argvP.setOrDie( arg_capacity, 1 );
  
  arg_objectStreamPtr  =  argvP.getIstreamPtr( 2 );
  
  argvP.dieIfUnusedArgs();
  cbrc::tryKnapsackEnumerator();
  return 1;
}

