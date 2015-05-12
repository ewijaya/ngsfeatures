/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2009/06/15 08:07:40 $
 *  
 *  Purpose: try out SeqGlobalAffineAligner.
 *
 *  See Also: ../test/testSeqGlobalAffineAligner.cc
 */
#include <iostream>
#include "utils/FLArray/FLEArray.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaRecordReader.hh"
#include "../SeqGlobalAffineAligner.hh"


std::istream*  arg_fastaIstreamPtr;

namespace cbrc{
  void trySeqGlobalAffineAligner(){

    FastaRecordReader fr( *arg_fastaIstreamPtr );

    const AminoScore&  ad  =  aminoScorePam120;


    /* ***** Read Sequences ***** */
    const FastaRecord*  curRecord  =  fr.nextRecordOrDie();

    const ResidueIndexMap::arrayT
      s1(  aminoResidueIndexMap.toResidueIndices( curRecord->seq() )  );

    curRecord  =  fr.nextRecordOrDie();

    const ResidueIndexMap::arrayT
      s2(  aminoResidueIndexMap.toResidueIndices( curRecord->seq() )  );


    SeqGlobalAffineAligner aligner( ad, s1 );

    const FLEArray<Alignment::alignScoreT> suffixBounds
      = SeqGlobalAffineAligner::bestPossibleSuffixScore( ad, s2 );

    const Alignment::alignScoreT  score  =  aligner.score( s2 );


    std::cout  /** OUTPUT **/
      <<  "score calculated without bounding "  <<  score  <<  std::endl

      <<  "scoreBounded calculated with bound -9999: " 
      <<  aligner.scoreBounded( s2, suffixBounds, -9999 )  <<  std::endl

      << "scoreBounded calculated with bound 9999: " 
      <<  aligner.scoreBounded( s2, suffixBounds, 9999 )   <<  std::endl

      <<  "scoreBounded calculated with bound "  <<  score  <<  ": " 
      <<  aligner.scoreBounded( s2, suffixBounds, score )  <<  std::endl

      << "Reality check: if bound <= score, scoreBounded should equal score without bounding\n";

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser argvP( argc, argv, "[fastaFile]" );

  arg_fastaIstreamPtr  =  argvP.getIstreamPtr( 1 );

  argvP.printDoc();

  argvP.dieIfUnusedArgs();

  cbrc::trySeqGlobalAffineAligner();
  return 1;
}
