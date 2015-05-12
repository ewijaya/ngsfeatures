/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/06/20 23:57:52 $
 *
 *  Purpose: Similarity function between two amino acids for alignment purposes.
 *
 *  Note: Would like to replace this file with "ResidueSubstitutionScore.hh" because I
 *        think that is a better name.
 *
 */
#ifndef _AMINOSCORE_HH_
#define _AMINOSCORE_HH_
#include <iostream>
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"


namespace cbrc{

  typedef int alignScoreT;
  const alignScoreT alignScoreTMin = INT_MIN;

class AminoScore{
public:
  //  virtual AminoScore* clone() const = 0;

  virtual alignScoreT gapInitiation() const = 0;
  virtual alignScoreT gapExtension() const = 0;
  virtual alignScoreT score( const ResidueIndexMap::indexT& r1,
			     const ResidueIndexMap::indexT& r2 ) const = 0;
  virtual alignScoreT maxScore() const = 0;
  virtual alignScoreT maxScore( const ResidueIndexMap::indexT& r1 ) const = 0;

  // Minimum of { score for any residue pair, gapInitiation, and gapExtension },
  // algorithms rely on this!
  virtual alignScoreT minScore() const = 0;

  bool minScoreIsValid() const;

  virtual const ResidueIndexMap& residueIndexMap() const = 0;
  virtual ~AminoScore(){}
};

}; // end namespace cbrc

#endif // defined _AMINOSCORE_HH_
