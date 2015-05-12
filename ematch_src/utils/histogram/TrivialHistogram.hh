/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.29
 *  Last Modified: $Date: 2006/05/22 23:42:18 $
 *
 *  Description: Simplest imaginable implementation of a histogram.
 *               uses simple array to hold counts of integers
 *               from 0 to n.
 */

#ifndef _TRIVIALHISTOGRAM_HH_
#define _TRIVIALHISTOGRAM_HH_
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

template <typename T> // T should be some kind of unsigned integer type.
class TrivialHistogram{
public:
  TrivialHistogram( const FLENumArray<T>& data,
		    const size_t startingCount = 0 ){
    size_t dataMax = data.max();
    initialize( dataMax, startingCount );
    addCountsRangeSafe( data );
  }
  TrivialHistogram( const size_t dataMax,
			    const size_t startingCount = 0 ){
    initialize( dataMax, startingCount );
  }
  TrivialHistogram( std::istream& is = std::cin ){
    count.read( is );
  }
  TrivialHistogram( const TrivialHistogram<T>& histo ) : 
    count( histo.count ){}
  void incrementCount( size_t index ){ ++count[index]; }
  const size_t operator()( const size_t& index ) const{ return count[index]; }
  size_t size() const{ return count.size(); }
  T sum() const{ return count.sum(); }
  void write( std::ostream& os ){
    count.write( os );
  }
  size_t firstNonzeroIndex(){ // returns size() if no non-zero element found.
    for( size_t i = 0; i < size(); ++i ) if( count[i] ) return i;
    return size();
  }
  // returns next non-zero element after i, or size() if none found.
  size_t nextNonzeroIndex( size_t i ){
    for( ++i; i < size(); ++i ) if( count[i] ) return i;
    return size();
  }
  void printNonzeroEntries( std::ostream& os = std::cout,
			    const char fieldSepartor = ' ',
			    const char entrySeparator = '\n' ) const;
private:
  void initialize( const size_t& dataMax,
		   const size_t& startingCount ){
    count.resize( dataMax+1 );
    count.fill( startingCount );
  }
  void addCountsRangeSafe( const FLENumArray<T>& data ){
    for( size_t i = 0; i < data.size(); ++i ){
      count[i] += data[i];
    }
  }
  FLENumArray<size_t> count;
};

template <typename T>
inline std::ostream& operator<<( std::ostream& os,
				 const TrivialHistogram<T>& histo ){
  for( size_t i = 0; i < histo.size(); ++i ){
    if( i )  os << " ";
    os << histo(i);
  }
  os << std::endl;
  return os;
}

}; // end namespace cbrc
#endif // defined _TRIVIALHISTOGRAM_HH_

