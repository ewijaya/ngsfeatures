/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/06/15 08:05:13 $
 *  
 *  Purpose: test SeqGlobalAffineAligner.
 *
 *  Description: Tests score methods by comparing the results of score calculation with
 *               and without bounding for all combinations of prefixes of the input
 *               sequences, starting with the short prefixes.
 *      
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/FLArray/FLEArray.hh"
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaRecordReader.hh"
#include "../SeqGlobalAffineAligner.hh"



std::istream*  arg_fastaIstreamPtr;


namespace cbrc{

  void testSeqGlobalAffineAligner(){
    const AminoScore&  ad  =  aminoScorePam120;

    FastaRecordReader fr( *arg_fastaIstreamPtr );

    const FastaRecord*  curRecordPtr  =  fr.nextRecordOrDie();
    const ResidueIndexMap::arrayT
      s1(  aminoResidueIndexMap.toResidueIndices( curRecordPtr->seq() )  );

    curRecordPtr  =  fr.nextRecordOrDie();
    const ResidueIndexMap::arrayT
      s2(  aminoResidueIndexMap.toResidueIndices( curRecordPtr->seq() )  );


    for( size_t i1 = 1; i1 < s1.size(); ++i1 ){

      ResidueIndexMap::arrayT prefix1( s1, 0, i1 );
      SeqGlobalAffineAligner gas( ad, prefix1 );

      for( size_t i2 = 1; i2 < s2.size(); ++i2 ){
	ResidueIndexMap::arrayT prefix2( s2, 0, i2 );
	FLEArray<int> suffixBounds = SeqGlobalAffineAligner::bestPossibleSuffixScore( ad, prefix2 );
	int score = gas.score( prefix2 );
	int boundedScore = gas.scoreBounded( prefix2, suffixBounds, score );
	if( score != boundedScore ){
	  std::cout << "# score: " << score << " boundedScore: " << boundedScore << std::endl;
	  std::cout << "> prefix1\n" << aminoResidueIndexMap.toResidues( prefix1 ) << std::endl;
	  std::cout << "> prefix2\n" << aminoResidueIndexMap.toResidues( prefix2 ) << std::endl;
	  exit( 1 );
	}
      }
      std::cerr << "prefix starting with " << i1 << " checked" << std::endl;
    }
  }
} // end namescape cbrc



int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "[fastaFile]" );

  arg_fastaIstreamPtr  =  argvP.getIstreamPtr( 1 );

  argvP.printDoc();

  argvP.dieIfUnusedArgs();

  cbrc::testSeqGlobalAffineAligner();

  return 1;
}
