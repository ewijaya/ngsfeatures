/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.6.16
 *  Last Modified: $Date: 2009/04/25 06:27:25 $
 *
 *  Description: See header file.
 *
 */

#include "BlastEValuesByClass.hh"
#include "utils/stl/pairPredicate.hh"



namespace cbrc{



std::vector< std::pair<std::string, double> > 
BlastEValuesByClass::bestEValuePerClass( const std::string& id, const bool quiet ) const{

  StringKeyHashtable<double> classToEValue;
  const std::vector<Blast8Result> hits = blastResults( id );


  for( size_t i = 0; i < hits.size(); ++i ){

    const std::string hitName = hits[i].hitName();

    if(  !labeledInstances.instanceNames().has( hitName )  ){
      if( !quiet )  std::cerr << "Warning: class not known for id: \"" << hitName << "\"\n";
      continue;
    }

    const size_t instanceIdx   =  labeledInstances.instanceNames()( hitName );

    const std::string className  =  labeledInstances.classLabel( instanceIdx );

    if( /**/  (  !classToEValue.has( className )  )
	||    hits[i].eValue() < classToEValue( className )  ){
      classToEValue[ className ]  =  hits[i].eValue();
    }
  }


  std::vector< std::pair<std::string, double> > classEValueList
    = classToEValue.contentsAsVector();

  std::sort( classEValueList.begin(), classEValueList.end(),
	     pairPredicate::secondLess<std::string, double> );

  return classEValueList;
}


}; // end namespace cbrc
