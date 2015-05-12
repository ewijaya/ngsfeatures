/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.9.4
 *
 *  Last Modified: 2003.9.4
 *  
 *  Purpose: (high level purpose or motivation)
 *
 *  Usage: (explain or give example of use, not necessary the same as the command line usage message)
 *
 *  Description: (reasonably detailed description of what this file does)
 *
 *  Reference: (Reference to relevant literature, only applicable sometimes)
 */

#ifndef _FLDIAGONALMATRIX_HH_
#define _FLDIAGONALMATRIX_HH_

namespace cbrc{

template <typename elemType>
class FLDiagonalMatrix {
public:
  FLDiagonalMatrix(){ m = NULL; byteSize = 0; }
  FLDiagonalMatrix( const size_t sz0 ) : sz0(sz0){
    elemSize = sz0 * (sz0+1) / 2;
    byteSize = elemSize * sizeof(elemType);
    m = new elemType[elemSize];
    p = new elemType*[sz0];
    size_t index = 0;
    for( size_t i = 0; i < sz0; ++i ){
      index += i;
      p[i] = &(m[index]);
    }
  }
  void zero(){
    memset( m, 0, byteSize );
  }
  size_t size0() const{ return sz0; }
  elemType& operator()( size_t index0, size_t index1 ) const{
    if( index0 > index1 ){  size_t a = index0; index0 = index1; index1 = a; } // swap( index0, index1 );
    validateIndices( index0, index1 );
    return (p[index0])[index1];
  }
  void validateIndices( size_t index0, size_t index1 ) const{}
  void friend testFLDiagonalMatrix();
private:
  size_t sz0;  // number of elements in a row or column.
  size_t elemSize;
  size_t byteSize; // size in bytes of the whole matrix.
  elemType *m;
  elemType **p;
};



template <typename elemType>
std::ostream &operator<<( std::ostream &os, const FLDiagonalMatrix<elemType>& fldm ){
  if( fldm.size0() == 0 ) return os;
  for( size_t i = 0; i < fldm.size0(); ++i ){
    os << fldm( i, 0);
    for( size_t j = 1; j <= i; ++j ){
      os << " " << fldm(i, j);
    }
    os << std::endl;
  }
  return os;
}


}; // end namespace
#endif // defined _FLDIAGONALMATRIX_HH_

