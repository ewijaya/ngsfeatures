/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.6.16
 *  Last Modified: $Date: 2009/04/25 06:21:47 $
 *  
 *  Description: filter protein pairs in the WoLF_PSORT dataset.
 *               pairs with good (low) e-value scores and
 *               the same localization site are printed out.
 *
 *
 */
#include <iostream>
#include <fstream>
#include <limits>
#include "../Blast8ResultReader.hh"
#include "utils/perlish/perlish.hh"
#include "utils/math/StayFiniteLog.hh"
#include "classifiers/LabeledInstances.hh"

namespace cbrc{
  void filterSameClassHighBlast( std::istream& trainStream,
				 std::istream& blastResultsStream,
				 const double& eValueThreshold ){
    Blast8ResultReader blastResults( blastResultsStream, false );
    LabeledInstances lis( LabeledInstances::text, trainStream );
    std::map<std::string, std::string> nameToClass = lis.getNameToClassMap();
    std::vector<std::string> hitNames;

    for( size_t i = 0; i < lis.size(); ++i ){
      if(  !blastResults.has( lis(i).name() )  )  continue;

      const std::vector<Blast8Result> hitV = blastResults( lis(i).name() );

      hitNames.clear();

      for( size_t j = 0; j < hitV.size(); ++j ){

	if(( nameToClass[ hitV[j].hitName() ] == nameToClass[ lis(i).name() ] )
	   && (              hitV[j].eValue() <=  eValueThreshold           )
	   ){
	  if( hitV[j].hitName() != lis(i).name() )  hitNames.push_back( hitV[j].hitName() );
	}
      }

      if( hitNames.size() > 0 ){
	std::cout << lis(i).name();
	for( size_t i = 0; i < hitNames.size(); ++i )  std::cout << " " << hitNames[i];
	std::cout << std::endl;
      }
    }
  }
}; // end namescape cbrc


int main( int argc, char** argv ){
  std::string usage( "Usage: " );
  usage += argv[0];
  usage += " WoLFPSORTfile blastResultFile [lg] BlastScoreThreshold";
  if( (argc < 4) || (argc > 5) ){
    std::cout << usage << "\nError: wrong number of arguments\n";
    exit( -1 );
  }
  int curArg = 1;
  std::ifstream trainStream( argv[curArg++] );
  if( !trainStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }
  std::ifstream blastResultsStream( argv[curArg++] );
  if( !blastResultsStream ){
    std::cout << "  could not open file " << argv[curArg-1] << std::endl;
    exit( -1 );
  }
  double eValueThreshold;
  if(  strcmp( argv[curArg], "lg" )  ){
    eValueThreshold = cbrc::perlish::toDouble( argv[curArg] );
  }else{
    if( argc != 5 ){
      std::cout << usage << "\nError: wrong number of arguments\n";
      exit( -1 );
    }
    eValueThreshold = pow( 2, cbrc::perlish::toDouble( argv[++curArg] ) );
  }
  std::cerr << __FILE__ << ":" << __LINE__ << " eValueThreshold: " << eValueThreshold << std::endl;
  cbrc::filterSameClassHighBlast( trainStream, blastResultsStream, 
				  eValueThreshold );
  return 1;
}

