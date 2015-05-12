/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.14
 *  Last Modified: $Date: 2006/12/02 15:54:29 $
 *
 *  Description: Provide some utility functions for matrices implemented
 *               with nested std::vectors.
 *
 */
#ifndef _VECTORMATRIX_HH
#define _VECTORMATRIX_HH
#define vectorMatrixFromCMatrix( m ) \
vectorMatrix::fromCMatrix( (m), sizeof(m)/sizeof((m)[0]) )
#include <vector>
#include <iostream>

namespace cbrc{

namespace vectorMatrix{

  template <typename T, size_t numCols>
  std::vector< std::vector<T> > fromCMatrix( T cmatrix[][numCols],
					     const size_t numRows ){
    std::vector< std::vector<T> > retval;
    for( size_t i = 0; i < numRows; ++i ){
      size_t offset = numCols * i;
      std::vector<T> curRow( *cmatrix + offset, *cmatrix + offset + numCols );
      retval.push_back( curRow );
    }
    return retval;
  }


  // Achtung! This only works for fixed length, non-pointer type T, in particular
  // In particular it will not work for type string
  template <typename T>
  void write( const std::vector< std::vector<T> >& m, std::ostream& os ){
    size_t s = m.size();
    os.write( (char*)&s, sizeof(s) );
    for( size_t i = 0; i < m.size(); ++i ){
      s = m[i].size();
      os.write( (char*)&s, sizeof(s) );
      os.write( (char*) &m[i][0], s * sizeof(T) );
    }
  }


  // Achtung! This only works for fixed length, non-pointer type T, in particular
  // In particular it will not work for type string
  // Example use: std::vector< std::vector<int> > m = vectorMatrix::read<int>( ifstream );
  template <typename T>
  std::vector< std::vector<T> > read( std::ifstream& is ){
    std::vector< std::vector<T> > retVal;
    size_t s;
    is.read( (char*)&s, sizeof(s) );
    retVal.resize(s);
    for( size_t i = 0; i < retVal.size(); ++i ){
      is.read( (char*)&s, sizeof(s) );
      retVal[i].resize(s);
      is.read( (char*) &retVal[i][0], s * sizeof(T) );
    }
    return retVal;
  }


  template <typename T>
  // true if size of columns are all equal.
  bool isRectangular( const std::vector< std::vector<T> >& m ){
    if( !m.size() )  return true;
    size_t numCols = m[0].size();
    for( size_t i = 1; i < m.size(); ++i ){
      if( m[i].size() != numCols )  return false;
    }
    return true;
  }
}; // end namespace vectorMatrix


template <typename T>
std::ostream& operator<<( std::ostream& os, const std::vector< std::vector<T> >& m ){
  for( size_t i = 0; i < m.size(); ++i ){
    for( size_t j = 0; j < m[i].size(); ++j ){
      if( j )  os << '\t';
      os << m[i][j];
    }
    os << std::endl;
  }
  return os;
}

}; // end namespace cbrc
#endif // _VECTORMATRIX_HH
