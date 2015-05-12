/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.6.9
 *  Last Modified: $Date: 2009/06/13 07:17:36 $
 *
 *  Purpose: try code involving LabeledSequence
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "../LabeledSequence.hh"

namespace cbrc{
  void tryLabeledSequence(){

    std::string seq( "MSSRKKNILKVIILGDSGVGKTSLMHRYVNDKYSQQYKATIGADFLTKEVTVDGDKVATMQVWDTAGQERFQSLGVAFYRGADCCVLVYDVTNASSFENIKSWRDEFLVHANVNSPETFPFVILGNKIDAEESKKIVSEKSAQELAKSLGDIPLFLTSAKNAINVDTAFEEIARSALQQNQADTEAFEDDYNDAINIRLDGENNSCSC" );

    std::vector<std::string> labels;
    labels.push_back( "YPT7_YEAST" );
    labels.push_back( "vacu" );

    LabeledSequence labSeq( seq, labels, aminoResidueIndexMap );


    std::cout << "name: "      << labSeq.name()      << std::endl;

    std::cout << "className: " << labSeq.className() << std::endl;

    std::cout << "residues: "  << labSeq.residues()  << std::endl;

    std::cout << "residue Indicies:\n";

    for( size_t i = 0; i < labSeq.length(); ++i ){
      std::cout << (i?",":"") << (size_t) labSeq.residueIndices()[i];
    }
    std::cout << std::endl;

  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "no args." );

  args.dieIfUnusedArgs();

  cbrc::tryLabeledSequence();

  return 1;
}

