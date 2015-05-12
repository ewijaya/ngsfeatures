/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/05/15 04:08:02 $
 *
 *  Description: Some typedefs useful for recount project.
 *
 */
#ifndef RECOUNT_TYPES_HH_
#define RECOUNT_TYPES_HH_
#include <iostream>
#include <vector>


namespace cbrc{


typedef  short unsigned int  neighborhoodSizeT;
typedef  double              probT;
typedef  double              tagCountT;
typedef  unsigned int        tagIdT;

typedef  std::vector<probT>   probVecT;
typedef  std::vector<tagIdT>  tagIdVecT;


} // end namespace cbrc
#endif // RECOUNT_TYPES_HH_
