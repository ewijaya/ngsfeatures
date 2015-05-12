/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2005.12.8
 *  Last Modified: $Date: 2007/10/17 21:48:43 $
 *  
 *  Purpose: run code involving TextWindow.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../TextWindow.hh"

namespace cbrc{
  void runTextWindow( std::string text, size_t windowSize ){
    size_t textSize = text.size();
    TextWindow<char> w( text.data(), 0, windowSize, textSize );
    std::cout << "first Array: ";
    std::cout << w.getFLArray() << std::endl;

    TextWindow<char> v( w, 0, w.size()-1 );
    std::cout << "first Array except the last char: ";
    std::cout << v.getFLArray() << std::endl;

    TextWindow<char> r( w, 1, w.size()-1 );
    std::cout << "first Array except the first char: ";
    std::cout << r.getFLArray() << std::endl;

    std::cout << "texting the increment operator\n";
    std::cout << w.getFLArray() << std::endl;
    while( w.rightContextSize() ){
      ++w;
      std::cout << w.getFLArray() << std::endl;
    }
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "text windowSize" );

  std::cout << "Warning: Perhaps you should be looking at the ResidueIndexWindow class?\n";

  size_t curArg = 0;
  std::string text = args[++curArg];

  size_t windowSize;
  args.setOrDie( windowSize, ++curArg );

  if( windowSize < 2 )  args.die( "try a window size bigger than 2" );
  args.dieIfUnusedArgs();
  cbrc::runTextWindow( text, windowSize );
  return 1;
}

