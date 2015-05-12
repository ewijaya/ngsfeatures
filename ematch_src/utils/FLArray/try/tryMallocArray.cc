/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.19
 *  
 *  Last Modified: 2003.7.15
 *  
 *  Purpose: test MallocArray.
 */
#include <iostream>
#include <vector>
#include "../MallocArray.hh"

class StructLikeClass{
public: 
  StructLikeClass( double x, double y ) : x(x), y(y){}
  double getY() const{ return y; }
private:
  double x;
  double y;
};

void tryMallocArray(){
  cbrc::MallocArray<double> fla( 10, 6.4 );
  //  cbrc::MallocArrayFast<double> fla( 10, 6.4 );
  cbrc::MallocArray<double> fla2( fla );
  std::vector<double> v( 10 );
  for( size_t i = 0; i < 10; ++i ) v[i] = i*3.2;
  cbrc::MallocArray<double> fla3;
  fla3 = v;

  std::cout << "test negative index, fla[-1] = " << fla[-1] << std::endl;

  cbrc::MallocArray<double> fla4;
  fla4.setSize( 10 );
  for( size_t i = 0; i < fla3.size(); ++i ) fla4[i] = fla3[i];

  std::cout << "size of v is: " << v.size() << std::endl;
  for( size_t i = 0; i < v.size(); ++i ){
    std::cout << "v[" << i << "] = " << v[i] << std::endl << std::flush;
  }

  std::cout << "print fla3 using iterators: " << std::endl;
  for( cbrc::MallocArray<double>::const_iterator p = fla3.begin(); p != fla3.end(); ++p ){
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

  std::cout << "now test MallocNumArray\n";

  std::cout << "size of v is: " << v.size() << std::endl;
  for( size_t i = 0; i < v.size(); ++i ){
    std::cout << "v[" << i << "] = " << v[i] << std::endl << std::flush;
  }


  cbrc::MallocNumArray<double> fln(10);
  //cbrc::MallocNumArrayFast<double> fln(10);
  fln = v;
  std::cout << "fln: " << fln << std::endl;
  std::cout << "sum of nums is: " << fln.sum() << std::endl;
  std::cout << "max of fln: is: " << fln.max() << std::endl;
  std::cout << "max index of fln is: " << fln.maxIndex() << std::endl;

  fln /= fln.sum();
  std::cout << "normalized fln: " << fln << std::endl;
  std::cout << "now sum is: " << fln.sum() << std::endl;

  //  std::cout << "** The next statement should fail if FLNumArrray is used **\n";
  //  fln[10] = 6.4;

  std::cout << "*** now testing with non built-in class\n";
  cbrc::MallocNumArray<StructLikeClass> structs( 2 );
  structs[0] = StructLikeClass( 0.6, 7.8 );
  std::cout << "y value of struct[0] is: " << structs[0].getY() << std::endl;
  structs.resize(7);
  structs[6] = StructLikeClass( 0.9, 12 );
  std::cout << "y value of struct[0] is: " << structs[0].getY() << std::endl;
  std::cout << "y value of struct[6] is: " << structs[6].getY() << std::endl;
}

int main(){
  tryMallocArray();
  return 1;
}
