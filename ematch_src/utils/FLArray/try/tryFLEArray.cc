/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.19
 *  Last Modified: $Date: 2008/06/30 21:56:45 $
 *
 *  Description: Try out FLEArray class
 */
#include <iostream>
#include <vector>
#include "../FLEArray.hh"

void tryFLEArray(){
  size_t arraySize = 10;
  cbrc::FLEArray<double> fla( arraySize, 6.4 );

  cbrc::FLEArray<double> fla2( fla );
  std::vector<double> v( arraySize );
  for( size_t i = 0; i < arraySize; ++i ) v[i] = i*3.2;
  cbrc::FLEArray<double> fla3;
  fla3 = v;

  cbrc::FLEArray<double> fla4;
  fla4.setSize( arraySize );
  for( size_t i = 0; i < fla3.size(); ++i ) fla4[i] = fla3[i];

  std::cout << "size of v is: " << v.size() << std::endl;
  for( size_t i = 0; i < v.size(); ++i ){
    std::cout << "v[" << i << "] = " << v[i] << std::endl << std::flush;
  }

  std::cout << "print fla3 using iterators: " << std::endl;
  for( cbrc::FLEArray<double>::const_iterator p = fla3.begin(); p != fla3.end(); ++p ){
    std::cout << *p << ", ";
  }
  std::cout << std::endl;

  for( size_t i = 0; i < v.size(); ++i ){
    std::cout << "v[" << i << "] = " << v[i] << std::endl << std::flush;
  }

  std::cout << "dumping fla3\n";
  std::cout << fla3 << std::endl;
  std::cout << "----------\n";

  std::cout << "dumping fla4\n";
  std::cout << fla4 << std::endl;
  std::cout << "----------\n";
  
  for( size_t i = 0; i < fla2.size(); ++i ){
    std::cout << "fla2[" << i << "] = " << fla2[i] << std::endl << std::flush;
  }

  std::cout << "now test FLENumArray\n";

  std::cout << "size of v is: " << v.size() << std::endl;
  for( size_t i = 0; i < v.size(); ++i ){
    std::cout << "v[" << i << "] = " << v[i] << std::endl << std::flush;
  }

  
  /* To check if conditional compiled ranging checking of FLENumArrayFast
   * is working, change type of fln to cbrc::FLENumArrayFast<double>
   * and define CBRC_DEBUG (which sets FLEARRAY_ALWAYS_CHECK_INDICES)
   */
//cbrc::FLENumArrayFast<double> fln(arraySize);
  cbrc::FLENumArray<double> fln(arraySize);
  
  fln = v;
  std::cout << "fln: " << fln << std::endl;
  std::cout << "sum of nums is: " << fln.sum() << std::endl;
  std::cout << "max of fln: is: " << fln.max() << std::endl;
  std::cout << "max index of fln is: " << fln.firstMaxIndex() << std::endl;

  fln /= fln.sum();
  std::cout << "normalized fln: " << fln << std::endl;
  std::cout << "now sum is: " << fln.sum() << std::endl;


  std::cout << "Do not freak out about the following error message"
	    << "from _validIndex, it is intentional.\n";

  fln[arraySize] = 6.4; // index arraySize is one too big to be in range!
}



int main(){
  tryFLEArray();
  return 1;
}
