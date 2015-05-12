/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.8.27
 *  Last Modified: $Date: 2008/02/24 01:54:26 $
 *
 *  Purpose: try code involving BitArrayStack
 */
#include <iostream>
#include "../BitArray.hh"


namespace cbrc{

void tryBitArrayStack(){
  int sz = 4;
  BitArrayStack bas( sz );

  bas.push( true );
  bas.push( false );
  bas.push( false );
  bas.push( false );

  while( !bas.empty() ){
    std::cout << bas.pop() << std::endl;
  }
}

} // end namespace cbrc


int main(){
  cbrc::tryBitArrayStack();
  return 1;
}
