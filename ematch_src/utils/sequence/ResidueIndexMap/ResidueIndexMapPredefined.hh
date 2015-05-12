/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.7
 *  Last Modified: $Date: 2009/05/17 10:59:41 $
 *
 *  Description: Defined constants useful for ResidueIndexMap
 *
 *  Achtung!  Some members are in namespace ResidueIndexMapPredefined
 *            but some are not.
 *            The members which contain "ResidueIndexMap" were left in the
 *            global cbrc namespace to reduce code verbosity, since their
 *            names are enough to indicate what file they are defined in.
 *
 *  History: Renamed from the longer "ResidueIndexMapPredefinedAlphabets.hh"
 */
#ifndef _RESIDUEINDEXMAPPREDEFINED_HH
#define _RESIDUEINDEXMAPPREDEFINED_HH
#include "ResidueIndexMap.hh"
#include <iostream>
#define CBRC_DNA_ALPHABET_PROPER "acgt"
#define CBRC_DNA_ALPHABET_WITH_N "acgtn"
#define CBRC_DNA_ALPHABET_EXTENDED "acgtbdhkmnrsvwy"
#define CBRC_DNA_ALPHABET_SIZE 4
#define CBRC_RNA_ALPHABET_PROPER "acgu"
#define CBRC_AMINO_ALPHABET        "ARNDBCQEZGHILKMFPSTWYVUX"
#define CBRC_AMINO_SIGMA 24  /* must equal strlen( CBRC_AMINO_ALPHABET ) */
#define CBRC_AMINO_ALPHABET_PROPER "ARNDCQEGHILKMFPSTWYV"
#define CBRC_AMINO_SIGMA_PROPER 20  /* must equal strlen( CBRC_AMINO_ALPHABET_PROPER ) */
#define CBRC_A_TO_Z_ALPHABET       "abcdefghijklmnopqrstuvwxyz"
#define CBRC_A_TO_Z_SIGMA       26  // must equal strlen( CBRC_A_TO_Z_ALPHABET )


namespace cbrc{


/* ** Note: The constants below are in not wrapped in their own namespace ** */
const ResidueIndexMap DNAProperResidueIndexMap( CBRC_DNA_ALPHABET_PROPER );
const ResidueIndexMap DNAWithNResidueIndexMap( CBRC_DNA_ALPHABET_PROPER, CBRC_DNA_ALPHABET_WITH_N );
const ResidueIndexMap DNAExtendedResidueIndexMap( CBRC_DNA_ALPHABET_PROPER, CBRC_DNA_ALPHABET_EXTENDED );
const ResidueIndexMap RNAProperResidueIndexMap( CBRC_RNA_ALPHABET_PROPER );
const ResidueIndexMap aminoResidueIndexMap( CBRC_AMINO_ALPHABET );
const std::string ResidueIndexMapAminoAlphabetProper( CBRC_AMINO_ALPHABET_PROPER );

const ResidueIndexMap A_to_Z_residueIndexMap( CBRC_A_TO_Z_ALPHABET );


namespace ResidueIndexMapPredefined{
  typedef enum { amino, DNA, RNA, unknown } seqTypeT;

  inline seqTypeT guessSeqType( const std::string& seq ){
    if( aminoResidueIndexMap.validResidues( seq ) )      return amino;
    if( DNAProperResidueIndexMap.validResidues( seq ) )  return DNA;
    if( RNAProperResidueIndexMap.validResidues( seq ) )  return RNA;
    return unknown;
  }
}


/* ** Achtung! The methods below are in not wrapped in their own namespace ** */

inline const ResidueIndexMap& residueIndexMapGuessedFromResidues( const std::string& residues ){
  if( DNAProperResidueIndexMap.validResidues( residues ) )  return DNAProperResidueIndexMap;
  if(  DNAWithNResidueIndexMap.validResidues( residues ) )  return DNAWithNResidueIndexMap;
  if( RNAProperResidueIndexMap.validResidues( residues ) )  return RNAProperResidueIndexMap;
  if(     aminoResidueIndexMap.validResidues( residues ) )  return aminoResidueIndexMap;
  std::cout << "Error: failed to guess sequence type:\n";
  exit( -1 );
}


inline const ResidueIndexMap& residueIndexMapFromName( const std::string& seqTypeName ){
  if( seqTypeName == "DNA"   )  return DNAProperResidueIndexMap;
  if( seqTypeName == "dna"   )  return DNAProperResidueIndexMap;
  if( seqTypeName == "RNA"   )  return RNAProperResidueIndexMap;
  if( seqTypeName == "rna"   )  return RNAProperResidueIndexMap;
  if( seqTypeName == "amino" )  return aminoResidueIndexMap;
  std::cout << "Error: unknown sequence type name: \"" << seqTypeName << "\"\n";
  exit( -1 );
}


inline
const ResidueIndexMap&
residueIndexMapFromSeqType( const ResidueIndexMapPredefined::seqTypeT& seqType ){
  if( seqType == ResidueIndexMapPredefined::DNA )    return DNAProperResidueIndexMap;
  if( seqType == ResidueIndexMapPredefined::RNA )    return RNAProperResidueIndexMap;
  if( seqType == ResidueIndexMapPredefined::amino )  return aminoResidueIndexMap;
  std::cout << "Error: unknown sequence type: \"" << seqType << "\"\n";
  exit( -1 );
}



}; // end namespace cbrc
#endif // _RESIDUEINDEXMAPPREDEFINED_HH

