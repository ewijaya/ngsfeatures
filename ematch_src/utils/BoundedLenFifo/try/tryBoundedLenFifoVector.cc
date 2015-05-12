/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.8
 *  Last Modified: $Date: 2009/06/08 13:34:32 $
 *
 *  Purpose: try code involving BoundedLenFifoVector
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../BoundedLenFifoVector.hh"


/* ********** PARAMETERS FROM COMMAND LINE ********** */
static size_t  arg_vectorLen;


namespace cbrc{

  

  void dumpFifoVector( const BoundedLenFifoVector<std::string>& fifoVector ){
    for( size_t i = 0;  i < fifoVector.size();  ++i  ){
      std::cout  <<  fifoVector.oldestElem( i )  << std::endl;
    }
    std::cout  <<  "========================================\n\n";
  }    

  void tryBoundedLenFifoVector(){
    BoundedLenFifoVector<std::string> fifoVector( arg_vectorLen );

    std::string line;

    // fill up fifoVector
    while(   ( fifoVector.size() < fifoVector.capacity() )
	     && std::getline( std::cin, line )
	     ){
      fifoVector.push( line );
    }

    // process first line
    dumpFifoVector( fifoVector );

    // process from 2nd line, up to numLines - fifoVector.capacity()
    while(  std::getline( std::cin, line )  ){
      fifoVector.push( line );
      dumpFifoVector( fifoVector );
    }

    // process remaining lines
    for(  fifoVector.popOldest();
	  fifoVector.size();
	  fifoVector.popOldest()  ){
      dumpFifoVector( fifoVector );
    }

  } // end tryBoundedLenFifoVector

} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "vectorLength" );

  argvP.setDoc( "-help|--help|-man|--man",
		"This program reads std::in and for each line, prints\n"\
		"that line, plus the following vectorLength lines" );

  argvP.setOrDie( arg_vectorLen, 1 );
  
  argvP.dieIfUnusedArgs();
  cbrc::tryBoundedLenFifoVector();
  return 1;
}

