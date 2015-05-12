/*  
 *  Author: Please set BPLT_AUTHOR environment variable
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Please set BPLT_AUTHOR environment variable, All rights reserved.
 *  Creation Date: 2008.1.15
 *  Last Modified: $Date: 2008/01/16 13:47:43 $
 *
 *  Purpose: try code involving LabeledAsciiSeq
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../LabeledAsciiSeq.hh"

namespace cbrc{
  void tryLabeledAsciiSeq(){
    LabeledAsciiSeq testSeq(  "ataggctcgctagcctagcgtagcctctcagt",
			      "testSeq_name"  );

    std::cout << "testSeq:\n" << testSeq;
  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args." );
  
  args.dieIfUnusedArgs();
  cbrc::tryLabeledAsciiSeq();
  return 1;
}

