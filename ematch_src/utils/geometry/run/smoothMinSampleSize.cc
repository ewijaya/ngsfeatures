/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.12
 *  Last Modified: $Date: 2007/02/13 04:49:14 $
 *  
 *  Description: Curve Smoother.
 *
 *               Smooths a curve until all y points are based on at least
 *               minSampleSize points.
 *  
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../smoothing.hh"


namespace cbrc{
  void smoothMinPoints( size_t minSampleSize ){
    std::vector<std::string> lines = perlish::slurpLines();
    std::vector<smoothing::NPOINT> u;
    for( size_t i = 0; i < lines.size(); ++i ){
      std::vector<std::string> fields = perlish::split( lines[i], " " );
      double x; // x value of input point
      double y; // y value of input point
      unsigned int weight = 1;  /* weight (num samples) of input point, 
				 * defaults to one if third input field missing. */
      if( fields.size() < 2 ){
	std::cerr << "Warning: skipping line with less than 2 fields: " << lines[i];
	break;
      }
      if( fields.size() > 3 ){
	std::cerr << "Warning: ignoring 3rd and later fields in line: " << lines[i];
      }
      if( fields.size() == 3 ){
	weight = perlish::toUnsignedInt( fields.at(2) );
      }
      x = perlish::toDouble( fields.at(0) );
      y = perlish::toDouble( fields.at(1) );
      u.push_back( smoothing::NPOINT(x, y, weight) );
    }

    smoothing::sortInPlaceAscendingValue( u );
    std::vector<smoothing::NPOINT> v = smoothing::mergeIdenticalAdjacentX( u );

    std::vector<smoothing::POINT> smoothed( v.size() );
    for( size_t i = 0; i < v.size(); ++i )  smoothed[i].setX( v[i].x() );

    smoothing::averageYCoordinatesWithNeighboringPointsUntilMinSampleSizeAttained
      ( v, smoothed, minSampleSize );

    //    std::cout << "after smoothing\n";
    for( size_t i = 0; i < smoothed.size(); ++i ){
      std::cout << smoothed[i] << "\n";
    }
  }


}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage: $0 minPoints\n\
\n\
    datainput is taken from standard in\n\
\n\
    INPUT FORMAT:\n\
\n\
        white space separated x, y coordinates one per line\n\
        optionally with a third field to specify a point replicated n times (defaults to one).\n\
\n\
    EXAMPLE INPUT:\n\
\n\
        0 0.3\n\
        0.1 1.1\n\
        0.3 0.9 2\n\
        ...\n"
			 );
  size_t minSampleSize;
  args.set( minSampleSize, 1 );
  args.dieIfUnusedArgs();
  cbrc::smoothMinPoints(  cbrc::perlish::toUnsignedInt( argv[1] )  );
  return 1;
}
