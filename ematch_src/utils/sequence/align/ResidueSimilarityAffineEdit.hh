/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.9.16
 *  Last Modified: $Date: 2008/06/26 10:25:52 $
 *
 *  Description: Residue Similarity specialization for the identity function
 *
 *               Conceptually inherits from "AminoScore" but I would like to
 *               rename that class "ResidueSimilarity".
 *
 */
#ifndef RESIDUESIMILARITYAFFINEEDIT_HH
#define RESIDUESIMILARITYAFFINEEDIT_HH
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include "Alignment.hh"


namespace cbrc{

class ResidueSimilarityAffineEdit{
public:
  /* ***** Constructors ***** */
  ResidueSimilarityAffineEdit( const ResidueIndexMap& rim )
    : _residueIndexMap(rim)
  {}

  /* ********** ACCESSORS ********** */
  const ResidueIndexMap& residueIndexMap() const{
    return _residueIndexMap;
  }

  // maxScore ≡ max_i,j { score( i, j ) }
  const alignScoreT maxScore()      const{  return  1;  }

  // minScore ≡ min_i,j { score( i, j ), gapInitiation(), gapExtension() }
  const alignScoreT minScore()      const{  return -2;  }
  const alignScoreT gapInitiation() const{  return -2;  }
  const alignScoreT gapExtension()  const{  return  0;  }

  alignScoreT maxScore(  const ResidueIndexMap::indexT& r1  ) const{
    return maxScore();
  }

  alignScoreT score( const ResidueIndexMap::indexT& r1,
		     const ResidueIndexMap::indexT& r2 ) const{
    return(  r1 == r2  );
  }
private:
  // object data
  const ResidueIndexMap& _residueIndexMap;
};


const ResidueSimilarityAffineEdit   residueSimilarityAffineEdit_DNA( DNAExtendedResidueIndexMap );
const ResidueSimilarityAffineEdit   residueSimilarityAffineEdit_amino( aminoResidueIndexMap );

}; // end namespace cbrc
#endif // defined RESIDUESIMILARITYAFFINEEDIT_HH

