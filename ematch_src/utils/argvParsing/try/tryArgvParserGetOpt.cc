/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.5.12
 *  Last Modified: $Date: 2008/01/04 23:39:34 $
 *
 *  Purpose: try or run code involving ArgvParser::getOpt()
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"

// ***** define command line flags *****
#define QQ(x) #x
#define Q(x) QQ(x)
#define HEIGHT_FLAG  -h|--height
#define WEIGHT_FLAG  -w|--weight
#define COLOR_FLAG   -c|--color
#define COLOR_VALUES red|green|blue


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "\
$0 ["Q(WEIGHT_FLAG)" weight] ["Q(HEIGHT_FLAG)" height] ["Q(COLOR_FLAG)" color]\n\
\n\
Tries out ArgvParser::getOpt\n\
\n\
  OPTIONS\n\
\n\
      "Q(WEIGHT_FLAG)" weight\n\
\n\
      "Q(HEIGHT_FLAG)" height\n\
\n\
      "Q(COLOR_FLAG)" color\n\
          color must be one of: "Q(COLOR_VALUES)"\n\
\n\
for example, try:\n\
\n\
    $0 -w 34\n\
    $0 -h 30 -w\n\
    $0 --weight 39 -p\n\
    $0 -height 10 in the hat\n\
    $0 --color green\n\
    $0 --color turquoise\n\
etc...\n" 
);


  std::vector<std::string> heightFlags;
  heightFlags.push_back( "-h" );
  heightFlags.push_back( "--height" );
  std::cout << "is opt flagged by \""
	    << heightFlags[0]
	    << "\" or \""
	    << heightFlags[1]
	    << "\" present?\n";

  if(   args.hasOpt( heightFlags )   ){
    std::cout << "height is: " << args.getOpt( heightFlags ) << std::endl;
  }else{
    std::cout << "height not given!\n";
  }
  std::string weightOptString = args.getOpt( Q(WEIGHT_FLAG) );
  if(   weightOptString.size()   ){
    std::cout << "weight is: " << weightOptString << std::endl;
  }else{
    std::cout << "No, flag absent\n";
  }

  std::string color = args.getOpt(  Q(COLOR_FLAG), Q(COLOR_VALUES)  );

  std::cout << "color is: " << color << std::endl;

  args.dieIfUnusedArgs();
}


