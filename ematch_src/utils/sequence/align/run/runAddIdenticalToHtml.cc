/*  Author: Paul Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2004.12.8
 *  Last Modified: $Date: 2009/06/21 02:12:50 $
 *  
 *  Description: Constructs html for aligned best hits to queries coming from pseudo-html input
 *               consisting of lines of text in which each line starts with a sequence id.
 *
 *               Those ids are compared to the sequences in the querySequencesStream. 
 *
 *               Achtung! The order of id's in querySequencesStream and queryHtmlStream is assumed
 *               to be consistent.
 *
 *               For each id in both query streams the sequences in dataSequencesStream are searched
 *               identical sequences. If an identical sequence is found a link to it is added.
 *               with an anchor like: <A name=id>id</A>.
 *
 *               The queryHtmlStream is modified to contain a link to that anchor based on 
 *               outputPathnameForLinking.
 *
 *               Assumes that the sequence ID
 *
 *  Purpose: This file was originally created as a version of runAddAlignmentToHtml that only
 *           detects identical sequences without any sequence similarity search. The purpose
 *           was as a backup to runAddAlignmentToHtml when there was a problem in the sequence
 *           similarity search program.
 */
#include <iostream>
#include <cstdlib>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/html/htmlStrings.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "../AminoScoreIdentity.hh"
#include "../SimilaritySearchableSequences.hh"


static struct{
  static const bool printHeader = false;
} opts;


namespace cbrc{
  void runAddIdenticalToHtml( std::istream& dataSequencesStream,
			      std::istream& queryHtmlStream,
			      std::istream& querySequencesStream,
			      const std::string& outputPathnameForLinking,
			      std::ostream& outputStream ){
    outputStream << 
      htmlStrings::header( "alignment of query strings to close matches in WoLFPSORT dataset" );
    if( opts.printHeader ){
      std::cout << htmlStrings::header( "WoLFPSORT predictions" );
    }


    /* ***** Read Database Sequences ***** */
    FastaSeqSlurper seqSlurper;

    std::vector<LabeledSequence>  dataSequences;

    seqSlurper.slurpLabeledSequences( dataSequences, dataSequencesStream );

    SimilaritySearchableSequences sss( aminoScoreIdentity, dataSequences );


    /* ***** Read Query Sequences ***** */
    std::vector<LabeledSequence> querySequences;

    seqSlurper.slurpLabeledSequences(  querySequences,
				       querySequencesStream,
				       aminoResidueIndexMap  );


    const std::vector<std::string>  queryHtml
      =  perlish::slurpLines( queryHtmlStream );

    if( queryHtml.size() != querySequences.size() ){
	// This should not happen if the wolfpsort pipe is working correctly.
	std::cerr << __FILE__ << ":" << __LINE__ <<  " Error: Number of query lines in summary html file: "
		  << queryHtml.size() << " not equal to number of query sequences: "
		  << querySequences.size() << std::endl;
    }

#ifdef CBRC_DEBUG  
    /* dump query for debugging */
    std::cerr << "query sequences:\n";
    for( size_t i = 0; i < querySequences.size(); ++i ){
      std::cerr << querySequences[i] << std::endl;
    }

    /* dump dataset for debugging */
    std::cerr << "dataset sequences:\n";
    for( size_t i = 0; i < sss.size(); ++i ){
      std::cerr << sss(i) << std::endl;
    }
#endif // defined CBRC_DEBUG

    // adjust loosenessFactor so that more careful (but time consuming) matching is done for
    // small queries.
    alignScoreT loosenessFactor = 512;
    loosenessFactor -= querySequences.size() * 16;
    if( loosenessFactor < 32 ) loosenessFactor = 32;

    for( size_t i = 0; i < queryHtml.size(); ++i ){
      std::vector<std::string> fields = perlish::split( "[ \t]+", queryHtml[i] );
      std::string id = fields[0];
      enum { DISSIMILAR, IDENTICAL } matchDegree = DISSIMILAR;
      SimilaritySearchableSequences::indexScorePairT bestMatchResult;
      std::vector<size_t> identicalMatches;
      identicalMatches.clear();
      identicalMatches = sss.identicalMatches( querySequences[i] );
      if( identicalMatches.size() )  matchDegree = IDENTICAL;
      if( matchDegree == IDENTICAL ){
	std::cout << queryHtml[i]
		  << " <I>identical</I> to seq";
	if( identicalMatches.size() > 2 ) std::cout << "s";
	std::cout << ": ";
	for( size_t matchIndex = 0; matchIndex < identicalMatches.size(); ++matchIndex ){
	  if( matchIndex ) std::cout << ", ";
	  std::cout << sss( identicalMatches[matchIndex] ).name() << ":" 
		    << sss( identicalMatches[matchIndex] ).className();
	}
	std::cout << "<BR>" << std::endl;
      }else{
	std::cout << queryHtml[i] << "<BR>" << std::endl;
      }
    }
    outputStream << htmlStrings::end();
    std::cout << htmlStrings::end();
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "DatasetSequencesFile queryHtmlFile querySequencesFile "
			 "ouputPathnameForLinking outputFile" );
  int curArg = 0;

  std::ifstream datasetSequencesFile;
  args.setOrDie( datasetSequencesFile, ++curArg );

  std::ifstream queryHtmlFile;
  args.setOrDie( queryHtmlFile, ++curArg );

  std::ifstream querySequencesFile;
  args.setOrDie( querySequencesFile, ++curArg );

  std::string outputPathnameForLinking( args[++curArg] );

  std::ofstream outputFile;
  args.setOrDie( outputFile, ++curArg );

  args.dieIfUnusedArgs();
  cbrc::runAddIdenticalToHtml( datasetSequencesFile, 
			       queryHtmlFile, 
			       querySequencesFile, 
			       outputPathnameForLinking, 
			       outputFile );
  return 1;
}
