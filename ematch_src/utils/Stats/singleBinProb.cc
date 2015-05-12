/*
 *  Author: Paul B. Horton
 *  
 *  Creation Date: 2003.7.8
 *  
 *  Copyright: All rights reserved
 *  
 *  Problem: groupSize marbles are chosen, without replacement, at random from n bins
 *  such that each remaining marble has the same probability of being chosen at each step.
 *  
 *  This program calculates the probability that all of the chosen marbles are from the same bin.
 */
#include <iostream>

int main(){
  const int groupSize = 12;
  const int n = 16;
  double binSize[n] = {107, 424, 168, 815, 284, 130, 562, 282, 220, 379, 334, 535, 483, 417, 565, 491};
  double totalMarbles = 0;
  for( int i = 0; i < n; ++i ){
    //    std::cout << "binSize[" << i << "] = " << binSize[i] << std::endl;
    totalMarbles += binSize[i];
  }
  double totalProb = 0;


  for( int i = 0; i < n; ++i ){ // for each bin
    double prob = 1.0;

    // can't possibly have a cluster of more than binSize[i] on chromosome i.
    if( groupSize > binSize[i] ) continue; 

    for( int j = 0; j < groupSize; ++j ){
      prob *= (binSize[i] - j) / (totalMarbles-j);
    }
    totalProb += prob; // add probability that all marbles are chosen from bin i.
  }

  std::cout << "prob of group of size " << groupSize;
  std::cout << " being chosen from just one bin is: " << totalProb << std::endl;
}


