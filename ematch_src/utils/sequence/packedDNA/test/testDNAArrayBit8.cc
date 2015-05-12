/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.4
 *  Last Modified: $Date: 2008/06/27 10:46:04 $
 *  
 *  Purpose: test DNAArrayBit8.
 */
#include <iostream>
#include "../DNAArrayBit8.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"

void testDNAArrayBit8( unsigned char bases ){
  cbrc::DNAArrayBit8<4> dnaArray( bases );
  const cbrc::ResidueIndexMap& rim = cbrc::DNAProperResidueIndexMap;
  std::cout << "size is: " << dnaArray.length() << std::endl;
  for( size_t i = dnaArray.length()-1; i < dnaArray.length(); --i ){
    std::cout << rim.toResidue( dnaArray(i) );
  }
  std::cout << std::endl;
  std::cout << "dnaArray: " << dnaArray << "\n";

  std::string s( "tgca" );
  for( size_t i = 0; i < s.length(); ++i ){
    dnaArray.set( i, rim.toResidueIndex( s[i] ) );
  }
  for( size_t i = 0; i < s.length(); ++i ){
    //    std::cout << i << "=" << static_cast<int>(dnaArray(i)) << ",";
    std::cout << rim.toResidue( dnaArray(i) );
  }
  std::cout << std::endl;

  std::cout << "substituted a into first position: " << dnaArray.substitute(0,0) << std::endl;

  std::cout << "test getIndices: " << rim.toResidues( dnaArray.getIndices() ) << std::endl;

  try{
    std::cout << "try an error...\n" << dnaArray(4);
  }
  catch( std::out_of_range& err ){
    std::cout << "out of range err says: " << err.what() << std::endl;
  }
  catch(...){
    std::cout << "something unexpected\n";
  }
  std::cout << "try length error\n";
  try{
    cbrc::DNAArrayBit8<5> tooBig( bases );
  }
  catch( std::length_error& err ){
    std::cout << "length_error err says: " << err.what() << std::endl;
  }
  std::cout << "try invalid_argument\n";
  try{
    dnaArray.set( 2, 4 );
  }
  catch( std::invalid_argument& err ){
    std::cout << "out of range err says: " << err.what() << std::endl;
  }
  catch(...){
    std::cout << "something unexpected\n";
  }

  std::cout << "constructing \"gac\"\n";
  cbrc::DNAArrayBit8<3> shortArray( rim.toResidueIndices( "gac" ) );
  std::cout << "shortArray: " << shortArray << std::endl;

  std::cout << " shortArray == zebra? " << (shortArray=="zebra") << std::endl;
  std::cout << " shortArray == gat?   " << (shortArray=="gat")   << std::endl;
  std::cout << " shortArray == gact?  " << (shortArray=="gact")  << std::endl;
  std::cout << " shortArray == ga?    " << (shortArray=="ga")    << std::endl;
  std::cout << " shortArray == gAc?   " << (shortArray=="gAc")   << std::endl;

  cbrc::DNAArrayBit8<2> ar1( 0 );
  cbrc::DNAArrayBit8<2> ar2( 1 );
  int testArray[] = { 39, 12 };

  std::cout << "testArray[0] " << testArray[ar1] << std::endl;
  std::cout << "testArray[1] " << testArray[ar2] << std::endl;


}


int main( int argc, char** argv ){
  if( argc != 2 ){
    std::cout << "Usage: " << argv[0] << " int" << std::endl;
    exit( 0 );
  }
  std::cout << "first arg: " << argv[1] << std::endl;
  unsigned int bases = atoi( argv[1] );
  std::cout << "bases int: " << bases << std::endl;
  testDNAArrayBit8( (cbrc::byte) bases );
  return 1;
}
