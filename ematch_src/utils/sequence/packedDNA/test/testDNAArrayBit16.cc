/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.22
 *  Last Modified: $Date: 2008/06/27 09:45:33 $
 *  
 *  Purpose: test DNAArrayBit16.
 */
#include <iostream>
#include "../DNAArrayBit16.hh"


void testDNAArrayBit16( cbrc::DNAArrayBit16_bases8 bases ){
  cbrc::DNAArrayBit16<8> dnaArray( bases );
  const cbrc::ResidueIndexMap& rim = cbrc::DNAProperResidueIndexMap;
  std::cout << "size is: " << dnaArray.length() << std::endl;
  for( size_t i = dnaArray.length()-1; i < dnaArray.length(); --i ){
    std::cout << rim.toResidue( dnaArray(i) );
  }
  std::cout << std::endl;
  std::cout << "dnaArray: " << dnaArray << "\n";

  std::string s( "tgca" );
  std::cout << "adding prefix: " << s << std::endl;
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
    std::cout << "try an error...\n" << dnaArray(8);
  }
  catch( std::out_of_range& err ){
    std::cout << "out of range err says: " << err.what() << std::endl;
  }
  catch(...){
    std::cout << "something unexpected\n";
  }
  std::cout << std::flush;
  std::cout << "try length error\n";
  try{
    cbrc::DNAArrayBit16<9> tooBig( bases );
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
  cbrc::DNAArrayBit16<3> shortArray( rim.toResidueIndices( "gac" ) );
  std::cout << "shortArray: " << shortArray << std::endl;

  std::cout << " shortArray == zebra? " << (shortArray=="zebra") << std::endl;
  std::cout << " shortArray == gat?   " << (shortArray=="gat")   << std::endl;
  std::cout << " shortArray == gact?  " << (shortArray=="gact")  << std::endl;
  std::cout << " shortArray == ga?    " << (shortArray=="ga")    << std::endl;
  std::cout << " shortArray == gAc?   " << (shortArray=="gAc")   << std::endl;

  cbrc::DNAArrayBit16<2> ar1( 0 );
  cbrc::DNAArrayBit16<2> ar2( 1 );
  int testArray[] = { 39, 12 };

  std::cout << "testArray[0] " << testArray[ar1] << std::endl;
  std::cout << "testArray[1] " << testArray[ar2] << std::endl;

  cbrc::DNAArrayBit16<5> ar3;
  ar3.set(  rim.toResidueIndices( std::string("ccatg") )  );
  std::cout << "should say ccatg: " << ar3 << std::endl;
}


int main( int argc, char** argv ){
  if( argc != 2 ){
    std::cout << "Usage: " << argv[0] << " int" << std::endl;
    exit( 0 );
  }
  std::cout << "first arg: " << argv[1] << std::endl;
  unsigned short int bases = atoi( argv[1] );
  std::cout << "bases int: " << bases << std::endl;
  testDNAArrayBit16( static_cast<cbrc::DNAArrayBit16_bases8>(bases) );
  return 1;
}

