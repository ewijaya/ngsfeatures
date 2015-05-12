/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.12
 *  Last Modified: $Date: 2008/08/13 12:54:09 $
 *
 *  Purpose: try code involving ResidueIndexMapTranslator
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ResidueIndexMapPredefinedTranslators.hh"


namespace cbrc{
  void tryResidueIndexMapTranslator(){


    //                              M  S  L  STOP
    std::string nucleicAcidString( "auguccuugugagu" );

    ResidueIndexMap::arrayT                      
      RNAseq(  RNAProperResidueIndexMap.toResidueIndices( nucleicAcidString )  );

    ResidueIndexMap::arrayT
      proteinSeq(  RNAseq.size() / 3  /* ÷ codon length */  );


    ResidueIndexMap::arrayT::const_iterator aminoEnd;

      
    standardRNACodonTranslator.translateInPlace(  RNAseq.begin(), RNAseq.end(),
						  proteinSeq.begin(), aminoEnd  );


    std::string
      proteinString(  aminoResidueIndexMap.toResidues( proteinSeq.begin(), aminoEnd )  );

    std::cout << "nucleic acid sequence is: " << nucleicAcidString << std::endl;
    std::cout << "translated version is   : " << proteinString     << std::endl;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "put usage message here!" );
  
  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexMapTranslator();
  return 1;
}

