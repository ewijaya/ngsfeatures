/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.8
 *  Last Modified: $Date: 2009/05/08 02:09:29 $
 *
 *  Purpose: try code involving PushbackLiteralVariadic
 */
#include <iostream>
#include <boost/foreach.hpp>
#include "../pushbackLiteralVariadic.hh"



namespace cbrc{

  void tryPushbackLiteralVariadic(){
    std::vector<unsigned> idVec;
    std::vector<double>   valVec;

    pushbackLiteral( idVec,   2,   3,    4,    5,   7 );

    pushbackLiteral( valVec, 0.1, 0.6, 0.15, 0.05, 0.1 );

    std::cout << "ids are: ";
    BOOST_FOREACH(  const unsigned&  id, idVec ){
      std::cout << id << ", ";
    }
    std::cout << std::endl;


    std::cout << "probs are: ";
    BOOST_FOREACH(  const double&  val,  valVec ){
      std::cout << val << ", ";
    }
    std::cout << std::endl;
  } // end tryPushbackLiteralVariadic()


} // end namescape cbrc



int main( int argc, const char* argv[] ){

  cbrc::tryPushbackLiteralVariadic();

  return 1;

}

