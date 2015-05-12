/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.11.26
 *  Last Modified: $Date: 2009/08/08 00:24:10 $
 *  
 *  Description: See header files.
 *
 *  Purpose:
 */

#include "AminoScore.hh"

namespace cbrc{

bool AminoScore::minScoreIsValid() const{
  if( minScore() > gapInitiation() )  return false;
  if( minScore() > gapExtension()  )  return false;
  for( size_t i = 0; i < residueIndexMap().sigma(); ++i ){
    for( size_t j = 0; j < residueIndexMap().sigma(); ++j ){
      if( minScore() > score( i, j ) )  return false;
    }
  }
  return true;
}



std::ostream& operator<<( std::ostream& os, const AminoScore& aScore ){

  static const char* const emptyBufferString = "    ";

  os << emptyBufferString;
  for( ResidueIndexMap::indexT i = 0; i < aScore.residueIndexMap().sigma(); ++i ){
    os << aScore.residueIndexMap().toResidue(i) << "   ";
  }
  os << std::endl;

  for( ResidueIndexMap::indexT i = 0; i < aScore.residueIndexMap().sigma(); ++i ){
    os << aScore.residueIndexMap().toResidue(i);

    for( ResidueIndexMap::indexT j = 0; j < aScore.residueIndexMap().sigma(); ++j ){
      // format and output score(i,j)
      char fieldBuffer[5];
      snprintf( fieldBuffer, 5, "%4d", aScore.score(i,j) );
      os << fieldBuffer;
    }

    os << std::endl;
  }

  return os;
}


}; // end namespace cbrc
