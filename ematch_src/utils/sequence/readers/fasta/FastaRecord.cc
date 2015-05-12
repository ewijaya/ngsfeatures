/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.3
 *  Last Modified: $Date: 2009/05/03 22:50:05 $
 *  Description: See header file.
 */
#include "boost/foreach.hpp"
#include "FastaRecord.hh"


namespace cbrc{



/* ********** GENERAL METHODS ********** */
void FastaRecord::clear(){
  _seq.resize( 0 );
  _headLine.resize( 0 );
  std::vector<std::string>().swap( _commentLinesBeforeHead );
  std::vector<std::string>().swap( _bodyLines              );
  std::vector<std::string>().swap( _labels                 );
}

/* ********** RELATED FUNCTIONS ********** */

std::ostream& operator<<( std::ostream& os, const FastaRecord& fastaRecord ){

  BOOST_FOREACH( const std::string commentLine, fastaRecord.commentLinesBeforeHead() ){
    os << commentLine << std::endl;
  }

  os << fastaRecord.headLine() << std::endl;

  BOOST_FOREACH( const std::string bodyLine, fastaRecord.bodyLines() ){
    os << bodyLine << std::endl;
  }

  return os;
}


} // end namespace cbrc

