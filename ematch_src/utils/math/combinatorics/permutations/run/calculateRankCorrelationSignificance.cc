/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.4.24
 *  Last Modified: $Date: 2008/04/26 07:17:36 $
 *
 *  Purpose: try out code involving PermutationGenerator.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/histogram/TrivialHistogram.hh"
#include "../PermutationGenerator.hh"

namespace cbrc{

  // return max possible sum squared difference for size permSize.
  size_t maxSumSquaredDifferences( const size_t permSize ){
    size_t sumD2 = 0;
    for( size_t i = 0; i < permSize; ++i ){
      size_t diff = (permSize - 1 - i) - i;
      sumD2 += diff * diff;
    }
    return sumD2;
  }

  size_t sumSquaredDifferences( const FLENumArray<size_t>& a ){
    size_t sumD2 = 0;
    for( size_t i = 0; i < a.size(); ++i ){
      size_t diff = a[i] - i;
      sumD2 += diff * diff;
    }
    return sumD2;
  }

  void calculatePermutationSumOfSquaredDifferencesDistribution( const size_t permSize ){
    TrivialHistogram<size_t> histo( maxSumSquaredDifferences(permSize) );
    PermutationGenerator pg( permSize );
    
    histo.incrementCount(  sumSquaredDifferences( pg.arrayRef() )  );
    while( pg.next() ){
      histo.incrementCount(  sumSquaredDifferences( pg.arrayRef() )  );
    }

    size_t cumulative = 0;
    double histoTotal = histo.sum();
    std::cout << "# results for size " << permSize << std::endl;
    std::cout << "# ssd     one-sided           two-sided\n";
    size_t maxSSD = maxSumSquaredDifferences( permSize );
    FLENumArray<size_t> twoWayCumulative( maxSSD / 2 );
    twoWayCumulative.zero();
    for( size_t i = histo.firstNonzeroIndex(); i < histo.size();
	 i = histo.nextNonzeroIndex(i) ){
      size_t curTwoWayCumulative;
      cumulative += histo(i);
      if( i < (maxSSD / 2) ){
	curTwoWayCumulative = 2 * cumulative;
	twoWayCumulative[i] = curTwoWayCumulative;
      }else if( i == maxSSD / 2 ){
	curTwoWayCumulative = (size_t) histoTotal;
      }else{
	curTwoWayCumulative = twoWayCumulative[maxSSD - i];
      }
      printf( "%4zu  %18.16f  %18.16f\n", i, cumulative / histoTotal,
	      curTwoWayCumulative / histoTotal );
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "permutation_size" );

  size_t permSize;
  args.setOrDie( permSize, 1 );
  args.dieIfUnusedArgs();
  cbrc::calculatePermutationSumOfSquaredDifferencesDistribution( permSize );
  return 1;
}

