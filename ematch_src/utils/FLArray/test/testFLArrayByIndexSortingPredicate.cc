/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.1.28
 *
 *  Last Modified: 2004.1.28
 *  
 *  Purpose: test FLArrayByIndexSortingPredicate.
 */
#include <iostream>
#include "../FLArrayByIndexSortingPredicate.hh"

namespace cbrc{
  void testFLArrayByIndexSortingPredicate(){
    int a[] = { 4, 6, 1, 5, 6, 2, 4 };
    FLArray<int> b( sizeof(a)/sizeof(a[0]), a );
    FLArray<size_t> perm = FLArrayIdentityPermutation( b.size() );

    std::cout << "b is: " << b << std::endl;
    std::stable_sort( perm.begin(), perm.end(), FLArrayByIndexSortingPredicate<int>(b) );
    std::cout << "perm is: " << perm << std::endl;

    //    FLArray<size_t> perm2 = FLArraySortedPermutation< int,std::greater<int> >(b);
    FLArray<size_t> perm2 = FLArraySortedPermutation< int, std::greater<int> >(b);
    std::cout << "perm2 is: " << perm2 << std::endl;
  }
}; // end namescape cbrc

int main(){
  cbrc::testFLArrayByIndexSortingPredicate();
  return 1;
}

