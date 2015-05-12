/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.4
 *  Last Modified: $Date: 2008/06/30 07:23:19 $
 *  
 *  Purpose: try ResidueIndexMap.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapPredefined.hh"

namespace cbrc{

void tryResidueIndexMap(){
  ResidueIndexMap rim( CBRC_DNA_ALPHABET_EXTENDED );

  std::cout << "validResidue( n ) = " << rim.validResidue('n') << std::endl;
  std::cout << "validResidue( x ) = " << rim.validResidue('x') << std::endl;
  std::cout << "validResidue( a ) = " << rim.validResidue('a') << std::endl;
  std::cout << "validResidue( T ) = " << rim.validResidue('T') << std::endl;
  std::cout << "max residue index is " << static_cast<int>(rim.maxIndex()) << std::endl;
  std::string testString = "TgCA";
  for( unsigned int i = 0; i < testString.size(); i++ ){
    std::cout << "residueIndex for " << testString.at(i) << " is ";
    std::cout << (int) rim.toResidueIndex( testString.at(i) ) << std::endl;
  }
  for( ResidueIndexMap::indexT i = 0; i < 4; i++ ){
    std::cout << "residue with index " << (int) i << " is " << rim.toResidue(i) << std::endl;
  }

  std::string t;
  t.resize(5);
  for( int i = 0; i < 5; ++i ) t[i] = (i%4);
  std::cout << "using toResidues: " << rim.toResidues( t ) << std::endl;

  std::cout << "try toResidueIndex exception\n";
  try{
    rim.toResidueIndex( 'z' );
  }
  catch( std::invalid_argument& err ){
    std::cout << "err: " << err.what() << std::endl;
  }

  std::cout << "try toResidueIndex exception\n";
  try{
    rim.toResidue( 4 );
  }
  catch( std::invalid_argument& err ){
    std::cout << "err: " << err.what() << std::endl;
  }

  std::string seq( "gaacttgac" );
  ResidueIndexMap::arrayT seqEncoded = rim.toResidueIndices( seq );

  std::cout << "encoded sequence: ";
  for( unsigned int i = 0; i < seqEncoded.size(); ++i ){
    std::cout << rim.toResidue( seqEncoded[i] );
  }
  std::cout << std::endl;

}

} // end namespace cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args" );
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMap();
  return 1;
}
