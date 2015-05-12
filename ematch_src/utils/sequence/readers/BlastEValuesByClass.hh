/*
 *  Author: Please set CBRC_AUTHOR environment variable
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.6.16
 *
 *  Last Modified: 2004.6.17
 *  
 *  Purpose: Hold blast hit eValues and class label information in a manner convenient 
 *  to use with nearest neighbor type classifiers.
 *
 */

#ifndef _BLASTEVALUESBYCLASS_HH_
#define _BLASTEVALUESBYCLASS_HH_
#include "classifiers/LabeledInstances.hh"
#include "Blast8ResultReader.hh"
#include "utils/stl/stlUtils.hh"


namespace cbrc{

class BlastEValuesByClass{
public:
  BlastEValuesByClass( const Blast8ResultReader& blastResults, 
		       const LabeledInstances&   labeledInstances )
    : blastResults( blastResults ), labeledInstances( labeledInstances )
  {}

  std::vector< std::pair<std::string, double> >
  bestEValuePerClass( const std::string& id,
		      const bool quiet = true ) const;

private:
  const Blast8ResultReader&  blastResults;
  const LabeledInstances&    labeledInstances;
};

}; // end namespace cbrc
#endif // defined _BLASTEVALUESBYCLASS_HH_


