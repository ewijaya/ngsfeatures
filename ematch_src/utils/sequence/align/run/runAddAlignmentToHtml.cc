/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: (C) 2004,2006, Paul B. Horton. All Rights Reserved.
 *  Creation Date: 2004.12.8
 *  Last Modified: $Date: 2009/06/21 02:13:34 $
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
 *               the best match. If a good best match is found the alignment is output to outputStream
 *               with an anchor like: <A name=id>id</A>.
 *
 *               The queryHtmlStream is modified to contain a link to that anchor based on 
 *               outputPathnameForLinking. The html new line "<BR>" is also added to lines of
 *               queryHtmlStream.
 */
#include <iostream>
#include <cstdlib>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"
#include "utils/sequence/readers/fasta/FastaSeqSlurper.hh"
#include "utils/html/htmlStrings.hh"
#include "../AminoScorePam120.hh"
#include "../SimilaritySearchableSequences.hh"

static struct{
  static const bool printHeader = false;
} opts;

namespace cbrc{
  void runAddAlignmentToHtml( std::istream& dataSequencesStream,
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

    SimilaritySearchableSequences sss( aminoScorePam120, dataSequences );



    /* ***** Read Query Sequences ***** */
    std::vector<LabeledSequence> querySequences;

    seqSlurper.slurpLabeledSequences(  querySequences,
				       querySequencesStream,
				       aminoResidueIndexMap  );


    // read in html lines. Do not skip lines that start with '#'
    const std::vector<std::string>  queryHtml
      =  perlish::slurpLines( queryHtmlStream, perlish::alwaysFalse );

    // adjust loosenessFactor so that more careful (but time consuming) matching is done for
    // small queries.
    alignScoreT loosenessFactor = 512;
    loosenessFactor -= querySequences.size() * 16;
    if( loosenessFactor < 32 ) loosenessFactor = 32;    

    size_t querySequencesIndex = 0;
    for( size_t i = 0; i < queryHtml.size(); ++i ){
      std::vector<std::string> fields = perlish::split( "[ \t]+", queryHtml[i] );
      std::string seqName = fields[0];
      for( ; (querySequencesIndex < querySequences.size()) &&
	     (querySequences[ querySequencesIndex ].name() != seqName)
	     ; ++querySequencesIndex );
      enum { DISSIMILAR, ALIGNABLE, IDENTICAL } matchDegree = DISSIMILAR;
      SimilaritySearchableSequences::indexScorePairT bestMatchResult;
      std::vector<size_t> identicalMatches;
      identicalMatches.clear();
      if( querySequencesIndex < querySequences.size() ){
	identicalMatches = sss.identicalMatches( querySequences[i] );
	if( identicalMatches.size() )  matchDegree = IDENTICAL;
	else{
	  bestMatchResult = sss.bestMatch( querySequences[i], loosenessFactor );
	  if( bestMatchResult.first < sss.size() ){
	    matchDegree = ALIGNABLE;
	  }
	}
      }
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
      }else if( matchDegree == ALIGNABLE ){
	const LabeledSequence& matchingSequence = sss(bestMatchResult.first);
	const std::string& matchingName = matchingSequence.name();
	AminoScorePam120 ad;
	SeqGlobalAffineAligner gas( ad, 
				    querySequences[ querySequencesIndex ].residueIndices() );
	Alignment bestAlignment = gas.alignment( sss(bestMatchResult.first).residueIndices() );
	outputStream << htmlStrings::anchorName( seqName, seqName )
		     << " <I>vs</I> " << matchingName << " <BR>" << std::endl;
	bestAlignment.outputHtmlMarkSame( outputStream, true /* mark different */ );
	std::cout << queryHtml[i] << " "
		  << htmlStrings::anchorHrefBasic( outputPathnameForLinking, "similar", matchingName )
		  << " to seq " << matchingName
		  << " of class " << matchingSequence.className() << "<BR>" << std::endl;
      }else{
	std::cout << queryHtml[i] << "<BR>" << std::endl;
      }
    }
    outputStream << htmlStrings::end();
    std::cout << htmlStrings::end();
  }
}; // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, 
			 "DatasetSequencesFile queryHtmlFile querySequencesFile \
ouputPathnameForLinking outputFile" );
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
  cbrc::runAddAlignmentToHtml( datasetSequencesFile, 
			       queryHtmlFile, 
			       querySequencesFile, 
			       outputPathnameForLinking, 
			       outputFile );
  return 1;
}
