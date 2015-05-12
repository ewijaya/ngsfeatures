/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.6.16
 *  Last Modified: $Date: 2009/04/25 07:00:49 $
 *  
 *  Description: Do simple jacknife test of nearest neighbor classifier accuracy using
 *               blast scores as distances. Blast score data is handled by Blast8ResultReader
 *               and class label information is handled by LabeledInstances.
 *
 *               Also contains code bin the data points by eValue in a way which gives a
 *               reasonable smooth curve of accuracy vs. eValue for plotting.
 *
 */
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include "../Blast8ResultReader.hh"
#include "../BlastEValuesByClass.hh"
#include "utils/FLArray/FLEArrayByIndexSortingPredicate.hh"
#include "utils/math/moreNumericLimits.hh"
#include "classifiers/LabeledInstances.hh"
#include "utils/geometry/smoothing.hh"

namespace cbrc{
  void labeledBlastToOddsRatio( std::istream& trainStream, std::istream& blastResultsStream ){
    Blast8ResultReader blastResults( blastResultsStream, true );
    LabeledInstances lis( LabeledInstances::text, trainStream );
    BlastEValuesByClass blastClassifier( blastResults, lis );

    std::vector<smoothing::NPOINT> correctByEValue;
    double log2r = 1.0 / log( 2.0 );

    FLEArray<double> eValues( lis.size() );
    for( size_t i = 0; i < lis.size(); ++i ){
      if( !blastResults.has( lis(i).name() ) ) continue;
      std::vector< std::pair<std::string, double> > hitList = blastClassifier.bestEValuePerClass( lis(i).name() );
      eValues[i] = hitList[0].second;
    }
    FLEArray<size_t> eValueSortedIndices = FLEArrayIdentityPermutation( eValues.size() );
    std::stable_sort( eValueSortedIndices.begin(), eValueSortedIndices.end(), FLEArrayByIndexSortingPredicate<double>(eValues) );
    for( size_t index = 0; index < lis.size(); ++index ){
      size_t i = eValueSortedIndices[index];
      if( !blastResults.has( lis(i).name() ) ) continue;
      std::vector< std::pair<std::string, double> > hitList = blastClassifier.bestEValuePerClass( lis(i).name() );
      std::string className = lis.classIdxToName( lis(i).classIdx() );
      double correct = ( hitList[0].first == className ) ? 1.0 : 0.0;
      //      correctByEValue.push_back( WeightedPoint2D<double, double>( log2r * log(hitList[0].second), correct ) );
      correctByEValue.push_back( smoothing::NPOINT( log2r * log(hitList[0].second), correct ) );
    }
    bool minValid = false;
    double minVal = 0; // silence compiler.

    // find smallest value not equal to -inf.
    std::cout << __FILE__ << ":" << __LINE__ << "correctByEValue.size(): " << correctByEValue.size() << std::endl;
    double log0 = log2r * log(0.0);
    for( size_t j = 0; j < correctByEValue.size(); ++j ){
      std::cout << __FILE__ << ":" << __LINE__ << " j: " << j << " ";
      std::cout << "correctByEValue[j].x(): " << correctByEValue[j].x() << std::endl;
      //      if( correctByEValue[j].x() <= -std::numeric_limits<double>::infinity() ) continue;
      //      if(  moreNumericLimits::isNegativeInfinity( correctByEValue[j].x() )  )  continue;
      if( correctByEValue[j].x() == log0 ) continue;
      if( minValid )  if( correctByEValue[j].x() < minVal ) minVal = correctByEValue[j].x();
      else{
	std::cout << __FILE__ << ":" << __LINE__ << "j: " << j << " ";
	std::cout << "correctByEValue[j].x(): " << correctByEValue[j].x() << std::endl;
	minValid = true;
	minVal = correctByEValue[j].x();
      }
    }
    std::cout << __FILE__ << ":" << __LINE__ << "minVal: " << minVal << std::endl;
    assert( minValid );
    // replace -inf with smallest finite value
    for( size_t j = 0; j < correctByEValue.size(); ++j ){
      //      if( correctByEValue[j].x() == -std::numeric_limits<double>::infinity() ) correctByEValue[j].setX( minVal );      
      //      if(  moreNumericLimits::isNegativeInfinity( correctByEValue[j].x() )  ){
      if( correctByEValue[j].x() == log0 ){
	std::cout << __FILE__ << ":" << __LINE__ << " setting " << j << " to: " << minVal;
	std::cout << " current val: " << correctByEValue[j] << std::endl;
	correctByEValue[j].setX( minVal );
      }
    }

    correctByEValue = smoothing::mergeIdenticalAdjacentX( correctByEValue );
    std::vector<smoothing::POINT> smoothedByEValue = smoothing::monotonicSmoothY( correctByEValue, false );
    std::cout << "correct by eValue:\n";
    for( size_t i = 0; i < correctByEValue.size(); ++i ){
      std::cout << correctByEValue[i].weight() << ": " << correctByEValue[i].x() << " " << correctByEValue[i].y();
      std::cout << "  |  " << smoothedByEValue[i].x() << " " << smoothedByEValue[i].y() << std::endl;
    }
    std::cout << std::endl;
  }
}; // end namescape cbrc

int main( int argc, char** argv ){
  std::string usage( "Usage: " );
  usage += argv[0];
  usage += " WoLFPSORTfile blastResultFile";
  if( argc < 3 ){
    std::cout << usage << "\n  not enough arguments\n";
    exit( -1 );
  }
  int curArg = 1;
  std::ifstream trainStream( argv[curArg] ); ++curArg;
  if( !trainStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }
  std::ifstream blastResultsStream( argv[curArg] ); ++curArg;
  if( !trainStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }

  cbrc::labeledBlastToOddsRatio( trainStream, blastResultsStream );
  return 1;
}

