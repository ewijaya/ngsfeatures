/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.12.2
 *  Last Modified: $Date: 2007/09/14 04:49:53 $
 *
 *  Purpose: try or run code involving VectorReadWrite
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/perlish/perlish.hh"
#include "../vectorMatrix.hh"

namespace cbrc{
  void tryVectorReadWrite( std::string& filename ){
    if(   perlish::exists( filename )   ){
      std::cout << "Error: exiting because filename: \"" << filename << "\" already exists\n";
      exit(1);
    }
    std::ofstream ofile;
    ofile.open( filename.c_str() );

    std::vector< std::vector<int> > m;
    std::vector<int> v;
    v.push_back( 3 ); v.push_back( 5 );
    m.push_back( v );
    v.clear();
    v.push_back( 7 );
    m.push_back( v );
    std::cout << "Matrix before writing\n";
    std::cout << m;

    vectorMatrix::write( m, ofile );
    ofile.close();

    std::ifstream ifile( filename.c_str() );
    std::vector< std::vector<int> > mFromFile = vectorMatrix::read<int>( ifile );

    std::cout << "Matrix after reading\n";
    std::cout << mFromFile;
    
  }
}; // end namescape cbrc

int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "filename" );
  std::string filename = args.shiftOrDie();
  args.dieIfUnusedArgs();
  cbrc::tryVectorReadWrite( filename );
  return 1;
}

