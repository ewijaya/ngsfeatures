/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.24
 *  Last Modified: $Date: 2008/02/24 03:18:27 $
 *  Description: See header file.
 */
#include "SymmetricBitMatrix.hh"

namespace cbrc{

SymmetricBitMatrix::SymmetricBitMatrix( const size_t& _width ) 
  : _width(_width){
  if( width() == 0 ) return;

  size_t sizeInBits = rowStart( width() );
  _sizeInBytes  =  sizeInBits / SYMMETRICBITMATRIX_BITS_PER_BYTE;

  if( sizeInBytes() != sizeInBits * SYMMETRICBITMATRIX_BITS_PER_BYTE ){
    ++_sizeInBytes;
  }

  m = new byte[sizeInBytes()];
}



void SymmetricBitMatrix::setSize( const size_t& _width ){
  this->_width = _width;
  assert( !m );
  size_t sizeInBits = rowStart( width() );
  _sizeInBytes = sizeInBits/SYMMETRICBITMATRIX_BITS_PER_BYTE;
  if( sizeInBytes() != sizeInBits * SYMMETRICBITMATRIX_BITS_PER_BYTE ) ++_sizeInBytes;
  m = new byte[sizeInBytes()];
}



bool SymmetricBitMatrix::validIndex( const size_t& index0, const size_t& index1 ) const{
  if( (index0 >= width()) || (index1 >= width()) ){
    if( index0 >= width() ){
      std::cerr << "** index0: " << index0 << " >= width: " << width() << std::endl << std::flush;
    }
    if( index1 >= width() ){
      std::cerr << "** index1: " << index0 << " >= width: " << width() << std::endl << std::flush;
    }
    return false;
  }
  if( index1 <= index0 ){
    std::cerr << "** index1: " << index1 << " <= index0: " << index0 << std::endl << std::flush;
    return false;
  }
  return true;
}


} // end namespace cbrc

