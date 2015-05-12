/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.27
 *  Last Modified: $Date: 2009/06/27 08:08:36 $
 *  Description: See header file.
 */
#include <boost/lexical_cast.hpp>
#include "KnapsackObjectVector.hh"

namespace cbrc{


void KnapsackObjectVector::readText_idsFrom0( std::istream& iStream ){
  _v.clear();


  const std::string  line  =  perlish::slurpLine( iStream );
  //std::cout << "LINE: " <<  line << std::endl;
  const std::vector<std::string>  fields  =  perlish::split( "\t", line );

  for(  idT curId = 0;  curId  < fields.size();  ++curId  ){
    const KnapsackObject  curObject(  curId,  boost::lexical_cast<weightT>(fields[curId])  );
    //std::cout << "CUROBJ " <<  curObject << std::endl;
    _v.push_back( curObject );
  }
}

void KnapsackObjectVector::makeFromWeightVector( const std::vector<double> &logProbVec ){
  _v.clear();

  for(  idT curId = 0;  curId  < logProbVec.size();  ++curId  ){
    const KnapsackObject  curObject(  curId,  boost::lexical_cast<weightT>(logProbVec[curId])  );
    //std::cout << "CUROBJ " <<  curObject << std::endl;
    _v.push_back( curObject );
  }
}

} // end namespace cbrc

