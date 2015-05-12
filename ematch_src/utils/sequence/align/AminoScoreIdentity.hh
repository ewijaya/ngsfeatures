/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.9.16
 *  Last Modified: $Date: 2009/04/20 11:48:27 $
 *
 *  Description: Amino Score specialization for the identity function
 *               defined aminoScoreIdentity
 *
 */
#ifndef _AMINOSCOREIDENTITY_HH_
#define _AMINOSCOREIDENTITY_HH_
#include "AminoScore.hh"

namespace cbrc{

class AminoScoreIdentity : public AminoScore{
public:
  AminoScoreIdentity( const ResidueIndexMap& residueIndexMap = aminoResidueIndexMap,
		      const alignScoreT matchScore    =  1,
		      const alignScoreT mismatchScore =  0,
		      const alignScoreT gapInitiation = -2,
		      const alignScoreT gapExtension  = -1 )
    : _residueIndexMap(residueIndexMap),
      matchScore    ( matchScore    ),
      mismatchScore ( mismatchScore ),
      _gapInitiation( gapInitiation ),
      _gapExtension ( gapExtension  )
  {
    _minScore = matchScore;
    if( _minScore > mismatchScore )  _minScore = mismatchScore;
    if( _minScore > gapInitiation )  _minScore = gapInitiation;
    if( _minScore > gapExtension  )  _minScore = gapExtension;
  }

  alignScoreT score( const ResidueIndexMap::indexT& r1,
		     const ResidueIndexMap::indexT& r2 ) const{
    if( r1 == r2 ){
      return matchScore;
    }else{
      return mismatchScore;
    }
  }

  alignScoreT maxScore( const ResidueIndexMap::indexT& r1 ) const{ return matchScore; }

  /* ********** ACCESSORS ********** */
  const ResidueIndexMap& residueIndexMap() const{  return _residueIndexMap;  }

  alignScoreT maxScore()     const { return matchScore;       }

  alignScoreT minScore()     const { return _minScore;        }

  alignScoreT gapInitiation() const{  return _gapInitiation;  }

  alignScoreT gapExtension()  const{  return _gapExtension;   }

private:
  // opject data
  const ResidueIndexMap& _residueIndexMap;
  alignScoreT matchScore;
  alignScoreT mismatchScore;
  alignScoreT _gapInitiation;
  alignScoreT _gapExtension;
  alignScoreT _minScore;
};


const AminoScoreIdentity aminoScoreIdentity;


}; // end namespace cbrc
#endif // defined _AMINOSCOREIDENTITY_HH_

