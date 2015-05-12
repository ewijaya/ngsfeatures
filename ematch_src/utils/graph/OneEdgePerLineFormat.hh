/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.2.2
 *  Last Modified: $Date: 2008/02/27 01:32:13 $
 *
 *  Description: Useful functions relating to the graph file format with one edge per line.
 *
 */

#ifndef _ONEEDGEPERLINEFORMAT_HH_
#define _ONEEDGEPERLINEFORMAT_HH_

#include <iostream>
#include <boost/regex.hpp>
#include "utils/graph/graphTypes.hh"

namespace cbrc{

class OneEdgePerLineFormat{
public:
  static nodeIndexT numNodesInStream( std::istream& is );
  static const boost::regex& inputDelimiter(){
    static boost::regex delim( "\t+" );
    return delim;
  }
private:
};

}; // end namespace
#endif // defined _ONEEDGEPERLINEFORMAT_HH_

