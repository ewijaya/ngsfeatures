/*
 *  Author: "Paul B. Horton"
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2004.12.8
 *  Last Modified: $Date: 2009/07/13 10:02:26 $
 *
 *  Purpose: run SimilaritySearchableSequences.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/align/AminoScorePam120.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "../SimilaritySearchableSequences.hh"
#include "../SeqGlobalAffineAligner.hh"

// macros for command line parsing
#ifndef QQ
#define QQ(x) #x
#define Q(x) QQ(x)
#endif // not defined QQ
#define USAGE_MESSAGE            [OPTIONS] sequencesFile queryFile
#define LOOSENESS_FLAGS           -l|--looseness
#define OUTPUT_FORMAT_FLAGS       -o|--ouput
#define OUTPUT_FORMAT_DEFAULT     text
#define MARK_DIFFERENT_FLAGS      -d|--different

namespace cbrc{
  void runSimilaritySearchableSequences( std::istream&      sequencesStream,
					 std::istream&      queryStream,
					 const alignScoreT& loosenessFactor,
					 std::string&       outputFormat,
					 bool&              markDifferent
					 ){

    const AminoScore& aminoScore = aminoScorePam120;

    
    /* ***** Read in Sequences ***** */
    const ResidueIndexMap&  residueIndexMap  =  aminoResidueIndexMap;

    FastaSeqSlurper seqSlurper;
    
    std::vector<LabeledSequence>  databaseSequences;

    seqSlurper.slurpLabeledSequences( databaseSequences, sequencesStream, residueIndexMap );

    const SimilaritySearchableSequences sss( aminoScore, databaseSequences );

    std::vector<LabeledSequence> querySequences;

    seqSlurper.slurpLabeledSequences( querySequences, queryStream, residueIndexMap );


    for(  size_t i = 0;  i < querySequences.size();  ++i  ){

      const SimilaritySearchableSequences::indexScorePairT
	result = sss.bestMatch( querySequences[i], loosenessFactor );

      if( result.first != sss.size() ){
	SeqGlobalAffineAligner gas( aminoScore, querySequences[i].residueIndices() );
	Alignment bestAlignment = gas.alignment( sss(result.first).residueIndices() );

	std::cout  <<  "best hit: "  <<  sss.seqName( result )
		   <<  " score: "    <<  result.second  <<  std::endl;

	bestAlignment.outputMarkSame( outputFormat, std::cout, markDifferent );
      }
    
    }

  } // end runSimilaritySearchableSequences


}; // end namescape cbrc


int main( int argc, const char* argv[] ){

  // check validity of OUTPUT_FORMAT_DEFAULT
  assert(  cbrc::ArgvParser::isMember( Q(ALIGNMENT_OUTPUT_FORMATS),
				       Q(OUTPUT_FORMAT_DEFAULT) )
	   );

  cbrc::ArgvParser args( argc, argv, Q(USAGE_MESSAGE) );

  args.setDoc( "--help", "\
Usage: $0 "Q(USAGE_MESSAGE)"\n\
\n\
  sequencesFile\n\
\n\
      multifasta file holding dataset sequences\n\
\n\
  queryFile\n\
\n\
      multifasta file holding query sequence(s)\n\
\n\
  OPTIONS\n\
\n\
  ["Q(LOOSENESS_FLAGS)" loosenessFactor]\n\
\n\
      positive number. Bigger values allows looser matching. Defaults to 32.\n\
\n\
  ["Q(OUTPUT_FORMAT_FLAGS)" outputFormat]\n\
      supported formats are: "Q(ALIGNMENT_OUTPUT_FORMATS)"\n\
      default value is: "Q(OUTPUT_FORMAT_DEFAULT)"\n\
\n\
  ["Q(MARK_DIFFERENT_FLAGS)"]\n\
      mark columns in which residues differ. Default is to mark matching residues.\n\
  " );
  
  args.printDoc();

  cbrc::alignScoreT loosenessFactor = 32;
  args.set( loosenessFactor, Q(LOOSENESS_FLAGS) );

  int curArg = 0;
  std::ifstream sequencesFile;
  args.setOrDie( sequencesFile, ++curArg );

  std::ifstream queryFile;
  args.setOrDie( queryFile, ++curArg );

  std::string outputFormat = args.getEnumOpt( Q(OUTPUT_FORMAT_FLAGS),
					      Q(ALIGNMENT_OUTPUT_FORMATS),
					      Q(OUTPUT_FORMAT_DEFAULT) );

  bool markDifferent(   args.hasOpt( Q(MARK_DIFFERENT_FLAGS) )   );

  args.dieIfUnusedArgs();
  cbrc::runSimilaritySearchableSequences( sequencesFile, queryFile, loosenessFactor,
					  outputFormat, markDifferent);
  return 1;
}
