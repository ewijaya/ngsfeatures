/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.30
 *  Last Modified: $Date: 2008/02/24 02:24:04 $
 *
 *  Purpose: try code involving BitMatrix
 */
#include <iostream>
#include "../BitMatrix.hh"


namespace cbrc{

void tryBitMatrix(){
  int dim1 = 39; 
  int dim2 = 41;
  BitMatrix bm( dim1, dim2 );
  std::cout << "size in bytes is: " << bm.sizeInBytes() << "\n";

  for( int i = 0; i < dim1; ++i ){
    for( int j = 0; j < dim2; ++j ){
      if(  ((i+j) % 2) == 0  ){
	bm.set( i, j );
      }
      else{
	bm.clear( i, j );
      }
    }
  }

  std::cout << bm;

  bm.setCurCol( 1 );

  std::cout << "dumping middle row\n";
  for( int i = 0; i < dim2; ++i ){
    std::cout << bm(i);
  }
  std::cout << std::endl;


  BitMatrixFast bmf( dim1, dim2 );

  std::cout << "\n--- No Asserts After Here! ---\n";
  for( int i = 0; i < dim1; ++i ){
    for( int j = 0; j < dim2; ++j ){
      if(  ((i+j) % 2) == 0  ){
	bmf.set( i, j );
      }
      else{
	bmf.clear( i, j );
      }
    }
  }

  std::cout << bmf;

}

} // end namespace cbrc


int main(){
  cbrc::tryBitMatrix();
  return 1;
}

