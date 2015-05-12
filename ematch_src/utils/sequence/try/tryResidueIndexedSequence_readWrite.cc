/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul Horton, All rights reserved.
 *  Creation Date: 2007.9.14
 *  Last Modified: $Date: 2009/06/13 07:44:21 $
 *
 *  Purpose: try or run code involving ResidueIndexedSequence_readWrite
 */
#include <iostream>
#include "utils/argvParsing/ArgvParser.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefinedAlphabets.hh"
#include "utils/sequence/readers/fasta/FastaRecordReader.hh"
#include "../ResidueIndexedSequence.hh"

namespace cbrc{
  void tryResidueIndexedSequence_readWrite( std::istream& fastaSeqStream,
					    const std::string& tempFilename ){

    const ResidueIndexMap& rim = DNAExtendedResidueIndexMap;

    { // write encapsulation block

      /* ***** write sequence to tempFile ***** */

      // read in sequence
      FastaRecordReader fr( fastaSeqStream );

      const FastaRecord* const  recordPtr  = fr.nextRecordOrDie();

      // construct ResidueIndexedSequence
      ResidueIndexedSequence indSeq( rim, recordPtr->seq(), recordPtr->name() );


      /* ***** Write ResidueIndexedSequence to testFile ***** */

      // open testFile
      std::ofstream os;
      perlish::openCautiously( os, tempFilename );

      std::cout << "Writing sequence to outfile...\n";

      indSeq.write( os );

      os.close();
    } // end write encapsulation block



    { // read encapsulation block
      
      /* ***** read sequence from tempFile ***** */
      
      // open file
      std::ifstream ifs;
      perlish::openOrDie( ifs, tempFilename );

      // read sequence
      ResidueIndexedSequence newIndSeq( ifs );

      // print sequence residues
      std::cout << "Sequence read in"     << std::endl;
      std::cout << newIndSeq.toResidues() << std::endl;
      

    } // end read encapsulation block
    


  }
} // end namescape cbrc


int main( int argc, const char* argv[] ){
  cbrc::ArgvParser args( argc, argv, "fastafmtDNASequenceFile writableTempFile" );
  
  size_t curArg = 0;

  std::ifstream fastaFile;
  args.setOrDie( fastaFile, ++curArg );

  std::string tempFilename;
  args.setOrDie( tempFilename, ++curArg );

  args.dieIfUnusedArgs();
  cbrc::tryResidueIndexedSequence_readWrite( fastaFile, tempFilename );
  return 1;
}

