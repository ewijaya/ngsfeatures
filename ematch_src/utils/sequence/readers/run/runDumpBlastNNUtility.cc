/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.1.19
 *  Last Modified: $Date: 2009/04/25 06:21:09 $
 *  
 *  Purpose: Perform a nearest neighbor classification using blast eValue scores.
 *
 *  Input:  The blast data is read from a file in blast -m 8 format.
 *          The id to class mapping is taken from a LabeledInstances file.
 *          The utility matrix is also read in from a file.
 * 
 *  Output: (blast score, utility) pairs are output.          
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "utils/stl/stlUtils.hh"
#include "classifiers/LabeledInstances.hh"
#include "classifiers/UtilityMatrix.hh"
#include "utils/FLArray/FLArrayByIndexSortingPredicate.hh"
#include "utils/sequence/readers/Blast8ResultReader.hh"
#include "utils/sequence/readers/BlastEValuesByClass.hh"

namespace cbrc{
  void dumpBlastNNUtility( std::istream& trainStream, 
				std::istream& utilityMatrixStream,
				std::istream& blastResultsStream ){
    Blast8ResultReader blastResults( blastResultsStream, true );
    LabeledInstances lis( LabeledInstances::text, trainStream );
    utilityMatrixT utilityMatrix( utilityMatrixStream );
    BlastEValuesByClass blastClassifier( blastResults, lis );
    std::vector< std::pair<size_t, double> > eValues;

    for( size_t i = 0; i < lis.size(); ++i ){
      if( !blastResults.has( lis(i).name() ) ) continue;
      std::vector< std::pair<std::string, double> > hitList = blastClassifier.bestEValuePerClass( lis(i).name() );
      eValues.push_back( std::make_pair<size_t, double>(i, hitList[0].second) );
    }
    std::stable_sort( eValues.begin(), eValues.end(),
		      stlUtils::pairSecondLess<size_t, double> );
    double log2r = 1.0 / log( 2.0 );
    // First do id's with blast hits.
    for( size_t index = 0; index < eValues.size(); ++index ){
      // classify by blast
      size_t i = eValues[index].first;
      std::vector< std::pair<std::string, double> > hitList = blastClassifier.bestEValuePerClass( lis(i).name() );
      
      // take care of ties.
      size_t numSameScore;
      for( numSameScore = 1; 
	   (numSameScore < hitList.size()) && hitList[numSameScore].second == hitList[0].second; 
	   ++numSameScore );
      if( hitList.size() > numSameScore ){ // check correctness.
	assert( hitList[numSameScore-1].second == hitList[0].second );
	assert( hitList[numSameScore].second != hitList[0].second );
      }
      std::string predictedClass = hitList[0].first;
      if( numSameScore == 1 ){
	predictedClass = hitList[0].first;
      }else{
	FLENumArray<size_t> classCounts( lis.numClasses(), (size_t)0 );
	for( size_t j = 0; j < numSameScore; ++j ){
	  ++classCounts[ lis.classNameToIdx( hitList[j].first ) ];
	}
	size_t maxCount = classCounts.max();
	// mask out non maximal counts
	// using the fact that LabeledInstances promises
	// to index class in order of most frequence first.
	// would be better to consider the total utility instead of just using the majority class...
	FLENumArray<size_t>::iterator bestPos = std::find( classCounts.begin(), classCounts.end(), maxCount );
	predictedClass = lis.classIdxToName( bestPos - classCounts.begin() );
      }
      std::string className = lis.classIdxToName( lis(i).classIdx() );

      std::cout  << lis(i).name() << " " << className << " " << predictedClass << " " << utilityMatrix( className, predictedClass ) << " " << log2r * log( hitList[0].second );
      std::cout << std::endl;
    }

    // now do id's without blast hits
    for( size_t i = 0; i < lis.size(); ++i ){
      if(  blastResults.has( lis(i).name() )  )  continue;
      std::string className = lis.classIdxToName( lis(i).classIdx() );

      // print dummy value for blast
      std::cout << lis(i).name() << " " << className << " " << "pass" << " " << "utilityUndefined eValueUndefined\n";
    }

  }
}; // end namescape cbrc

int main( int argc, char** argv ){
  std::string usage( "Usage: " );
  usage += argv[0];
  usage += " labeledInstancesFile utilityMatrixFile blastResultsFile";
  std::string filenameBase;

  if( argc < 3 ){
    std::cout << "Error: not enough arguments\n";
    std::cout << usage << std::endl;
    exit( -1 );
  }
  int curArg = 1;
  std::ifstream trainStream( argv[curArg] ); ++curArg;
  if( !trainStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }
  std::ifstream utilityMatrixStream( argv[curArg] ); ++curArg;
  if( !utilityMatrixStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }
  std::ifstream blastResultsStream( argv[curArg] ); ++curArg;
  if( !blastResultsStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }
  
  if( argc > curArg ){
    std::cout << usage << "  too many command line arguments given\n";
    exit( -1 );
  }

  cbrc::dumpBlastNNUtility( trainStream, utilityMatrixStream, blastResultsStream );
  return 1;
}

