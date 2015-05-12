/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.7
 *  Last Modified: $Date: 2007/10/13 09:26:49 $
 *  Description: See header file.
 */
#include "ResidueIndexedSequence.hh"
#include "utils/stl/stringBinaryIO.hh"

namespace cbrc{


/* ********** binary IO ********** */
void ResidueIndexedSequence::read( std::istream& is ){


  /* ******* read in signature and confirm if it is correct ******* */
  std::string signatureReadFromStream;

  stringBinaryIO::read( signatureReadFromStream, is, binaryStreamSignature().size() );

  if(   signatureReadFromStream != binaryStreamSignature()   ){
    perlish::die( __FILE__, __LINE__, "Signature read from input stream does not match\n" );
  }


  // read in data
  rim.read(is);

  stringBinaryIO::read( _name, is );
  _sequence.read( is );
}


void ResidueIndexedSequence::write( std::ostream& os ) const{

  /* ******* write each component ******* */
  stringBinaryIO::writeContentsOnly( binaryStreamSignature(), os );

  residueIndexMap().write( os );

  stringBinaryIO::write( _name, os );

  _sequence.write( os );
}


} // end namespace cbrc

