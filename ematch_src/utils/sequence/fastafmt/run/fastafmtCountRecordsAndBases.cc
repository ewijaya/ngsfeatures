/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.1.12
 *  Last Modified: $Date: 2008/07/27 05:27:48 $
 *
 *  Description: Count number of records of a multifasta format file.
 *
 *               See main for details.
 *     
 */
#include <iostream>
#include <utility>
#include "utils/perlish/perlish.hh"
#include "utils/argvParsing/ArgvParser.hh"
#include "../fastafmtConst.hh"



typedef std::pair<unsigned int, unsigned int> baseAndRecordCountPairT;

baseAndRecordCountPairT
fastafmtCountRecordsAndBases( std::istream& fastaInputStream,
			      const char commentChar /* no effect when set to
							FASTAFMT_RECORD_MARKER_CHAR */
			      ){
  std::string line = "";
  unsigned int numRecords = 0;
  unsigned int numBases = 0;
  while( std::getline( fastaInputStream, line ) ){
    if( line.empty() ) continue; /* not actually necessary for usual the
				    implementation of std::string */
    if( line[0] == FASTAFMT_RECORD_MARKER_CHAR ){
      ++numRecords;
    }else if( line[0] == commentChar ){
      continue;
    }else{ // body line. We treat a-zA-Z as bases.
      for( size_t i = 0; i < line.size(); ++i ){
	char c( line[0] );
	if( ((c >= 'a') && (c <= 'z')) ||
	    ((c >= 'A') && (c <= 'Z')) ){
	  ++numBases;
	}
      }
    }
  } // get next line.

  return std::make_pair( numRecords, numBases );
}


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "Usage:\n\n  $0 --help\n $0 [--comment-char COMMENT-CHAR [FILE]..." );
  args.setDoc( "--help",
"$0: Count number of records and total sequence bases of a multifasta format file.\n"\
"No error checking is done on input format.\n"\
"\n"\
"    $ $0 foo.fasta\n"\
"\n"\
"is similar to , but faster than:\n"\
"\n"\
"    $ grep \"^>\" foo.fasta | wc \n"\
"\n"\
"Command line arguments are more or less defined to act like\n"\
"the unix command \"wc\".\n"\
"OPTIONS\n"\
"    --comment-char COMMENT-CHAR|none\n"\
"\n"\
"        Lines beginning with the character COMMENT-CHAR are ignored. If \"none\"\n"\
"        is given no input lines are considered comment lines.\n"
	       );
  args.printDoc();

  std::ifstream fastaStream;
  baseAndRecordCountPairT countResult;
  std::string commentCharOpt( "#" ); // default.
  args.set( commentCharOpt, "--comment-char" );

  if( commentCharOpt == "none" ){
    commentCharOpt = FASTAFMT_RECORD_MARKER_CSTRING; // This means no comment lines.
  }

  if( commentCharOpt.size() != 1 ){
    args.die( "Error: expected single character or \"none\" for COMMENT-CHAR\n" );
  }


  if( args.unusedArgs().size() == 0 ){
    countResult = fastafmtCountRecordsAndBases( std::cin, commentCharOpt[0] );
    std::cout << countResult.first << " " << countResult.second << std::endl;
  }else{
    size_t numFilesGiven = args.unusedArgs().size();
    bool cinAlreadyUsed = false;

    // loop over filename command line arguments
    for( size_t i = 1; i <= numFilesGiven; ++i ){
      std::string curFilename = args[i];
      
      // use standard in.
      if( curFilename == "-" ){ 
	if( cinAlreadyUsed ){
	  args.die( "Error: only \"-\" argment (for standard input) is allowed" );
	}
	countResult = fastafmtCountRecordsAndBases( std::cin, commentCharOpt[0] );
      }

      // ordinary curFilename given
      else{
	if(  cbrc::perlish::isDirectory( curFilename )  ){
	  std::cout << curFilename << ": is a directory\n";
	  continue;
	}
	args.set( fastaStream, i );
	fastaStream.copyfmt(std::cin);
	std::cin.rdbuf(fastaStream.rdbuf());

	countResult = fastafmtCountRecordsAndBases( std::cin, commentCharOpt[0] );
	fastaStream.close();
      }

      // Output counts.
      if( numFilesGiven > 0 ){
	std::cout << curFilename << " " << countResult.first << " " << countResult.second << std::endl;
      }else{
	std::cout << countResult.first << " " << countResult.second << std::endl;
      }
    } // to next filename command line argument
  }      
  args.dieIfUnusedArgs();
  return 1;
}

