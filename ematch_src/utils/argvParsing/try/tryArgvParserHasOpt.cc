/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2007/05/13 01:06:33 $
 *
 *  Purpose: try or run code involving ArgvParser::hasOpt()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "\
$0 flag opt...\n\
\n\
Tries out ArgvParser::hasOpt\n\
\n\
for example, try:\n\
\n\
$0 -w 34\n\
$0 -h 30 -w\n\
$0 --weight 39 -p\n\
$0 the cat -height 10 in the hat\n\
etc...\n" 
);

  // >>>>>>>>>>>>>>> Test hasFlag( std::string, std::string ) <<<<<<<<<<<<<<<
  std::cout << "is opt flagged by \"-w\" or \"--weight\" present?\n";
  if(   args.hasOpt( "-w", "--weight" )   ){
    std::cout << "Yes, option present\n";
  }else{
    if(   args.hasFlag( "-w", "--weight" )    ){
      std::cout << "No, flag was last arg and therefore does not mark an opt\n";
    }else{
      std::cout << "No, flag absent\n";
    }
  }

//   // >>>>>>>>>>>>>>> Test hasFlag( std::vector<std::string ) <<<<<<<<<<<<<<<
  std::vector<std::string> heightFlags;
  heightFlags.push_back( "-h" );
  heightFlags.push_back( "--height" );
  std::cout << "is opt flagged by \""
	    << heightFlags[0]
	    << "\" or \""
	    << heightFlags[1]
	    << "\" present?\n";

  if(   args.hasOpt( heightFlags )   ){
    std::cout << "Yes, option present\n";
  }else{
    if(   args.hasFlag( heightFlags )    ){
      std::cout << "No, flag was last arg and therefore does not mark an opt\n";
    }else{
      std::cout << "No, flag absent\n";
    }
  }

}

