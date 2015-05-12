/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.29
 *  Last Modified: $Date: 2006/04/30 13:39:43 $
 *
 *  Description: Simplest imaginable implementation of a histogram.
 *               uses simple array to hold counts of unsigned integers
 *               from 0 to n.
 */

#ifndef _TRIVIALUNSIGNEDHISTOGRAM_HH_
#define _TRIVIALUNSIGNEDHISTOGRAM_HH_
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

template <typename T> // T should be some kind of unsigned integer type.
class TrivialUnsignedHistogram{
public:
  TrivialUnsignedHistogram( const FLENumArray<T>& data,
			    const size_t startingCount = 0 ){
    size_t dataMax = data.max();
    initialize( dataMax, startingCount );
    addCountsRangeSafe( data );
  }
  TrivialUnsignedHistogram( const size_t dataMax,
			    const size_t startingCount = 0 ){
    initialize( dataMax, startingCount );
  }
  TrivialUnsignedHistogram( std::istream& is = std::cin ){
    count.read( is );
  }
  void incrementCount( size_t index ){ ++count[index]; }
  const size_t operator()( const size_t& index ) const{ return count[index]; }
  size_t size() const{ return count.size(); }
  void write( std::ostream& os ){
    count.write( os );
  }
  void printNonzeroEntries( std::ostream& os = std::cout,
			    const char fieldSepartor = ' ',
			    const char entrySeparator = '\n' ) const{
    bool firstOutput = true;
    for( size_t i = 0; i < count.size(); ++i ){
      if( count[i] ){
	if( firstOutput )  firstOutput = false;
	else               os << entrySeparator;
	os << i << fieldSepartor << count[i];
      }
    }
  }
private:
  void initialize( const size_t& dataMax,
		   const size_t& startingCount ){
    count.resize( dataMax+1 );
    count.fill( startingCount );
  }
  void addCountsRangeSafe( const FLENumArray<T>& data ){
    for( size_t i = 0; i < data.size(); ++i ){
      ++count[ data[i] ];
    }
  }
  FLENumArray<size_t> count;
};

template <typename T>
inline std::ostream& operator<<( std::ostream& os,
				 const TrivialUnsignedHistogram<T>& histo ){
  for( size_t i = 0; i < histo.size(); ++i ){
    if( i )  os << " ";
    os << histo(i);
  }
  os << std::endl;
  return os;
}

}; // end namespace cbrc
#endif // defined _TRIVIALUNSIGNEDHISTOGRAM_HH_

