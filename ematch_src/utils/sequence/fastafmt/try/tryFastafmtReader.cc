/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2008/02/03 23:20:25 $
 *  
 *  Purpose: try out FastafmtReader.
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/stl/stlUtils.hh"
#include "../FastafmtReader.hh"
#define QQ(x) #x
#define Q(x) QQ(x)
#define DELIMITER_FLAG  -d|--delimiter

namespace cbrc{
  void tryFastafmtReader( std::istream& fastafmtStream, const std::string& delimiter ){
    
    // construct FastafmtReader and set parameters
    FastafmtReader fr;

    if( delimiter.size() ){
      fr.fastafmtInputOptionsRef().setBetweenLabelsDelimiterRegex( boost::regex(delimiter) );
    }

    // read and dump records
    while(   fr.readRecord( fastafmtStream )   ){
      std::cout << "name: " << fr.curRecord().name()     << std::endl;

      std::cout << "labels:";
      for( size_t i = 0; i < fr.curRecord().labels().size(); ++i ){
	std::cout << " |" << fr.curRecord().labels()[i] << "|";
      }
      std::cout << std::endl;

      std::cout << "seq: "  << fr.curRecord().seq() << std::endl;
    }
  }
}; // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage: $0 ["Q(DELIMITER_FLAG)" betweenLabelsDelimiter] [fastafmtfile]\n\
\n\
input taken from std::cin if fastafile not given\n\
\n\
OPTIONS:\n\
\n\
  "Q(DELIMITER_FLAG)" betweenLabelsDelimiter\n\
\n\
   betweenLabelsDelimiter is a boost::regex string.\n\
\n\
\n\
EXAMPLE:\n\
\n\
  $0 -d \"\\\\s+\" ../exampleData/tabSeparated.fasta" );

  std::string delimiter("");
  args.set( delimiter, Q(DELIMITER_FLAG) );

  std::istream& fastaStream(   args.getIstream(1)   );

  args.dieIfUnusedArgs();
  cbrc::tryFastafmtReader( fastaStream, delimiter );
  return 1;
}

