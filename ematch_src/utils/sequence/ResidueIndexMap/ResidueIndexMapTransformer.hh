/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.11
 *  Last Modified: $Date: 2008/08/11 11:21:55 $
 *
 *  Description: Simple Transformer of ResidueIndexMapped sequences.
 *               Maps residues from one alphabet to another (possible same) alphabet.
 *               The mapping is not necessarily one to one.
 *
 *  Purpose: Created to implement in silico transcription of DNA to RNA
 *
 */
#ifndef RESIDUEINDEXMAPTRANSFORMER_HH_
#define RESIDUEINDEXMAPTRANSFORMER_HH_
#include <iostream>
#include "ResidueIndexMap.hh"

namespace cbrc{

class ResidueIndexMapTransformer{
public:
  /* ********** TYPEDEFS ********** */


  /* ********** CONSTRUCTORS ********** */
  ResidueIndexMapTransformer( const ResidueIndexMap& origResidueIndexMap,
			      const ResidueIndexMap& tranResidueIndexMap,
			      const std::string& origAscii,
			      const std::string& tranAscii );

  void transformInPlace( ResidueIndexMap::indexT& indexToTransform );

  void transformInPlace( ResidueIndexMap::arrayT& indicesToTransform );
  
private:
  // object data
  const ResidueIndexMap& origResidueIndexMap;
  const ResidueIndexMap& tranResidueIndexMap;
  const std::string origAscii;
  const std::string tranAscii;
  FLENumArray<ResidueIndexMap::indexT> origToTran;
};

} // end namespace cbrc
#endif // RESIDUEINDEXMAPTRANSFORMER_HH_
