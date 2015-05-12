/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.7
 *  Last Modified: $Date: 2008/06/09 13:24:10 $
 *
 *  Description: Class to hold sequence translated to residue indices
 *               by ResidueIndexMap
 *
 *  Purpose: Originally created for the SuffixArray class made for
 *           the LAST project.
 *
 *  See Also: LabeledSequence.hh, almost the same but sequence contains
 *            std::vector<std::string> labels instead of just a name.
 *
 */
#ifndef RESIDUEINDEXEDSEQUENCE_HH
#define RESIDUEINDEXEDSEQUENCE_HH
#include <iostream>
#include "ResidueIndexMap/ResidueIndexMap.hh"

namespace cbrc{


class ResidueIndexedSequence{
public:
  /* ***** typedef's ***** */
  typedef ResidueIndexMap::arrayT::const_iterator const_iterator;

  /* ***** Constructors ***** */
  ResidueIndexedSequence( const ResidueIndexMap& rim,
			  const std::string& asciiSeq,
			  const std::string name = std::string("") )
    : rim(rim), _name(name){
    rim.assignResidueIndices( _sequence, asciiSeq );
  }


  ResidueIndexedSequence( std::istream& is ){
    read( is );
  }

  // null constructor
  ResidueIndexedSequence(){}
  
  /* ***** Accessors ***** */

  const ResidueIndexMap& residueIndexMap()  const{ return rim; }
  ResidueIndexMap& residueIndexMapNonconst() { return rim; }
  const ResidueIndexMap::arrayT& sequence() const{ return _sequence; }
  const std::string& name() const{ return _name; }



  /* ***** short cut accessors for member data ***** */

  std::string toResidues() const{
    return(   residueIndexMap().toResidues( _sequence )   );
  }



  /* ***** methods for using like standard array or vector ***** */

  size_t size() const{ return _sequence.size(); }
  ResidueIndexMap::indexT* begin() const { return _sequence.begin(); }
  ResidueIndexMap::indexT* end()   const { return _sequence.end();   }

  const ResidueIndexMap::indexT& operator()( const size_t& i ) const{
    return _sequence[i];
  }
  ResidueIndexMap::indexT& operator[]( const size_t& i ) const{
    return _sequence[i];
  }
  

  /* ***** binary IO ***** */
  static const std::string& binaryStreamSignature(){
    static const std::string s( "RISQ000\n" );
    return s;
  }

  // These binary IO functions die upon error
  void read(  std::istream& is );
  void write( std::ostream& os ) const;

private:
  /* *************** object data *************** */
  /* ******* persistent object data ******* */
  ResidueIndexMap rim;

  std::string _name;

  ResidueIndexMap::arrayT _sequence;
};

} // end namespace cbrc
#endif // RESIDUEINDEXEDSEQUENCE_HH
