/*
 *  Author: Paul Horton
 *  
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004,2006,2007 Paul Horton, All rights reserved.
 *  Creation Date: 2004.4.20
 *  Last Modified: $Date: 2009/06/21 00:02:25 $
 *  
 *  Description: A sequence both as ASCII and as residue
 *               index array. Each sequence can have a list of string
 *               labels associated with it.
 *
 *  See Also: ResidueIndexedSequence.hh
 */
#include "utils/perlish/perlish.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"

#ifndef LABELEDSEQUENCE_HH_
#define LABELEDSEQUENCE_HH_

namespace cbrc{

const std::string LabeledSequenceUndefinedLabel( "undefined" );

class LabeledSequence{
public:
  /* ***** CONSTRUCTORS ***** */

  // use contents of residues to guess as which predefined ResidueIndexMap to use.
  LabeledSequence( const std::string& residues,
		   const std::vector<std::string>& labels );

  LabeledSequence( const std::string& residues,
		   const std::vector<std::string>& labels,
		   const ResidueIndexMap&          residueIndexMap );

  LabeledSequence( const LabeledSequence& ls );

  const std::string& residues() const{  return _residues;  }
  const ResidueIndexMap::arrayT& residueIndices()    const{ return  _residueIndices; }
  const ResidueIndexMap::arrayT* residueIndicesPtr() const{ return &_residueIndices; }
  const std::string& name()      const{  return label(0);  }
  const std::string& className() const{  return label(1);  }

  const std::string& label( const size_t& i ) const{
    if( i < labels().size() )  return labels()[i];
    return LabeledSequenceUndefinedLabel;
  }

  const std::vector<std::string>& labels() const{  return _labels;  }

  size_t size()   const{ return residueIndices().size(); }
  size_t length() const{ return residueIndices().size(); }

  const ResidueIndexMap& residueIndexMap() const{
    return *_residueIndexMapPtr;
  }

  static bool notLonger( const LabeledSequence& ls0, const LabeledSequence& ls1 ){
    return( ls0.residues().size() <= ls1.residues().size() );
  }
private:
  void initResidueIndices();

  // object data
  std::string                         _residues;
  ResidueIndexMap::arrayT             _residueIndices;
  std::vector<std::string>            _labels;

  // pointer to ResidueIndexMap object, the object itself is external to this class
  const ResidueIndexMap*              _residueIndexMapPtr;
};


std::ostream& operator<<( std::ostream &os, const LabeledSequence& ls );



} // end namespace cbrc
#endif // defined LABELEDSEQUENCE_HH_
