/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.9
 *  Last Modified: $Date: 2008/06/25 10:56:41 $
 *  Description: See header file.
 */
#include "LabeledSequence.hh"

namespace cbrc{

/* ********** CONSTRUCTORS ********** */
LabeledSequence::LabeledSequence( const std::string& residues,
				  const std::vector<std::string>& labels )
  : _residues(residues){
  assert( residues.size() > 0 );

  _labels.reserve( labels.size() );
  copy( labels.begin(), labels.end(), back_inserter(_labels) );

  const ResidueIndexMap& residueIndexMap = residueIndexMapGuessedFromResidues( residues );
  _residueIndexMapPtr = &residueIndexMap;

  initResidueIndices();
}


LabeledSequence::LabeledSequence( const std::string& residues,
				  const std::vector<std::string>& labels,
				  const ResidueIndexMap& residueIndexMap
				  )
  : _residues(residues),
    _residueIndexMapPtr(&residueIndexMap)
{
  assert( residues.size() > 0 );

  _labels.reserve( labels.size() );
  copy( labels.begin(), labels.end(), back_inserter(_labels) );

  initResidueIndices();
}
  

void LabeledSequence::initResidueIndices(){

  _residueIndices.setSize( residues().size() );
  for( size_t i = 0; i < residues().size(); ++i ){
    _residueIndices[i] = residueIndexMap().toResidueIndex( residues()[i] );
  }
}



LabeledSequence::LabeledSequence( const LabeledSequence& ls )
  : _residues(ls._residues),
    _residueIndices(ls._residueIndices),
    _residueIndexMapPtr(ls._residueIndexMapPtr)
{
  _labels.reserve( ls.labels().size() );
  copy( ls._labels.begin(), ls._labels.end(), back_inserter(_labels) );
}



std::ostream& operator<<( std::ostream &os, const LabeledSequence& ls ){
  os << ">";
  for( size_t i = 0; i < ls.labels().size(); ++i )  os << " " << ls.labels()[i];
  os << std::endl;
  std::vector<std::string> seqSegments = perlish::splitByLength( ls.residues(), 60 );
  for( size_t i = 0; i < seqSegments.size()-1; ++i )  os << seqSegments[i] << std::endl;
  if( seqSegments.size() )                            os << seqSegments[seqSegments.size()-1];  // do not print trailing "\n".
  return os;
}


} // end namespace cbrc

