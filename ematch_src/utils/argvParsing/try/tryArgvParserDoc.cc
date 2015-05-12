/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.23
 *  Last Modified: $Date: 2006/09/23 08:31:19 $
 *
 *  Purpose: try code involving ArgvParser document strings.
 *           In particular test locale functionality.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../ArgvParser.hh"

namespace cbrc{
  void tryArgvParserDoc(){
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "[OPTIONS]..." );
  args.setDoc( "--help",
"\n\
NAME\n\
\n\
    $0 - this is just to test ArgvParser doc functionality.\n\
\n\
SYNOPSIS\n\
\n\
    $0 [OPTIONS] ... [FASTA_FILE]\n\
...\n\
" );

  args.setDoc( "--help",
"\n\
名前\n\
    $0 - ArgvParser doc機能を試してみるだけ\n\
\n\
書式\n\
    $0 [OPTIONS] ... [FASTA_FILE]\n\
...\n\
", "ja_JP.eucjp" );
  args.dieIfUnusedArgs();
  cbrc::tryArgvParserDoc();
  return 1;
}

