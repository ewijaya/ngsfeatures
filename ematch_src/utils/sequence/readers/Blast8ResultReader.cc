/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004-2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.5.18
 *  Last Modified: $Date: 2009/08/25 21:12:28 $
 *  
 *  Description: See header files.
 */
#include <cmath>
#include "utils/sequence/readers/Blast8ResultReader.hh"

namespace cbrc{


bool Blast8Result::setFromStream( std::istream& is ){
  std::string line = perlish::slurpLine( is, Blast8ResultReader::isBlastCommentLine );
  if( !line.size() )  return false;
  fields = perlish::split( line, " \t\n" );
  return true;
}
  

Blast8ResultReader::Blast8ResultReader( std::istream& is, const bool ignoreSelfHit ){
  std::vector<std::string> lines = perlish::slurpLines( is );
  // do first line.
  if( !lines.size() ) return;
  size_t i;
  for( i = 0; (i < lines.size()) && isBlastCommentLine( lines[i] ); ++i );  // skip blast style comments
  if( i >= lines.size() ) return;
  std::vector<std::string> fields = perlish::split( lines[i], " \t\n" );
  std::string curQuery = fields[0];
  std::string lastQuery = curQuery;
  std::vector<Blast8Result> hits;
  hits.push_back( fields );
  // do rest of lines, query results are not pushed until a new query name is seen.
  for( ; i < lines.size(); ++i ){ 
    if(  isBlastCommentLine( lines[i] )  )  continue; // skip blast style comments

    std::vector<std::string> fields = perlish::split( lines[i], " \t\n" );
    if( ignoreSelfHit && (fields[0] == fields[1]) ) continue;
    curQuery = fields[0];
    if( curQuery != lastQuery ){
      queryLabels[ lastQuery ] = results.size();
      results.push_back( hits );
      hits.clear();
    }
    hits.push_back( fields );
    lastQuery = curQuery;
  }
  // push the hits for the last query
  queryLabels[ lastQuery ] = results.size();
  results.push_back( hits );
}

void Blast8ResultReader::sortByEValue(){
  for( size_t i = 0; i < results.size(); ++i ){
    std::stable_sort( results[i].begin(), results[i].end() );
  }
}

bool Blast8ResultReader::sortedByEValue() const{
  for( size_t i = 0; i < results.size(); ++i ){
    if(  std::adjacent_find( results[i].begin(), results[i].end(), Blast8Result::firstGreater )
	 != results[i].end()  ){
      return false;
    }
  }
  return true;
}

double Blast8ResultReader::maxEValue() const{
  double maxValue = 0.0; // smallest possible eValue.
  for( size_t i = 0; i < results.size(); ++i ){
    for( size_t j = 0; j < results[i].size(); ++j ){
      if( results[i][j].eValue() > maxValue ) maxValue = results[i][j].eValue();
    }
  }
  return maxValue;
}

void Blast8ResultReader::printNormalizedSimilarity( std::ostream& os ) const{
  double m = maxEValue();
  for( size_t i = 0; i < results.size(); ++i ){
    for( size_t j = 0; j < results[i].size(); ++j ){
      os << results[i][j].queryName() << " " << results[i][j].hitName() << " ";
      os << ( m - results[i][j].eValue() ) / m << std::endl;
    }
  }
}

void Blast8ResultReader::printNegLgEValues( std::ostream& os ) const{
  double logReciprocal = 1 / log( 2.0 );
  for( size_t i = 0; i < results.size(); ++i ){
    for( size_t j = 0; j < results[i].size(); ++j ){
      os << results[i][j].queryName() << " " << results[i][j].hitName() << " ";
      os << -logReciprocal * log( results[i][j].eValue() ) << std::endl;
    }
  }
}


bool Blast8ResultReader::isBlastCommentLine( const std::string& line ){
  return(  line.find( "[blastall]" ) != std::string::npos  );
}


std::ostream& operator<<( std::ostream& os, const Blast8ResultReader& brr ){
  for( size_t i = 0; i < brr.results.size(); ++i ){
    for( size_t j = 0; j < brr.results[i].size(); ++j ){
      os << brr.results[i][j] << std::endl;
    }
  }
  return os;
}
    

}; // end namespace cbrc
