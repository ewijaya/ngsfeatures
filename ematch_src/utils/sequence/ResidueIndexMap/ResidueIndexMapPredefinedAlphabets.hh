/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2007.9.7
 *  Last Modified: $Date: 2008/06/25 11:00:03 $
 *
 *  Description: Defined constants useful for ResidueIndexMap
 *
 *  Deprecated. Renamed to the slighly less verbose "ResidueIndexMapPredefined"
 *
 *  Achtung!  Some members are in namespace ResidueIndexMapPredefinedAlphabets
 *            but some are not.
 *            The members which contain "ResidueIndexMap" were left in the
 *            global cbrc namespace to reduce code verbosity, since their
 *            names are enough to indicate what file they are defined in.
 *
 */
#ifndef _RESIDUEINDEXMAPPREDEFINEDALPHABETS_HH
#define _RESIDUEINDEXMAPPREDEFINEDALPHABETS_HH
#include "ResidueIndexMap.hh"
#include <iostream>
#define CBRC_DNA_ALPHABET_PROPER "acgt"
#define CBRC_DNA_ALPHABET_WITH_N "acgtn"
#define CBRC_DNA_ALPHABET_EXTENDED "acgtbdhkmnrsvwy"
#define CBRC_DNA_ALPHABET_SIZE 4
#define CBRC_RNA_ALPHABET "acgu"
#define CBRC_AMINO_ALPHABET        "ARNDBCQEZGHILKMFPSTWYVUX"
#define CBRC_AMINO_SIGMA 24  /* must equal strlen( CBRC_AMINO_ALPHABET ) */
#define CBRC_AMINO_ALPHABET_PROPER "ARNDCQEGHILKMFPSTWYV"
#define CBRC_AMINO_SIGMA_PROPER 20  /* must equal strlen( CBRC_AMINO_ALPHABET_PROPER ) */

namespace cbrc{


/* ** Achtung! The constants below are in not wrapped in their own namespace ** */
const ResidueIndexMap DNAProperResidueIndexMap( CBRC_DNA_ALPHABET_PROPER );
const ResidueIndexMap DNAWithNResidueIndexMap( CBRC_DNA_ALPHABET_PROPER, CBRC_DNA_ALPHABET_WITH_N );
const ResidueIndexMap DNAExtendedResidueIndexMap( CBRC_DNA_ALPHABET_PROPER, CBRC_DNA_ALPHABET_EXTENDED );
const ResidueIndexMap RNAResidueIndexMap( CBRC_RNA_ALPHABET );
const ResidueIndexMap aminoResidueIndexMap( CBRC_AMINO_ALPHABET );
const std::string ResidueIndexMapAminoAlphabetProper( CBRC_AMINO_ALPHABET_PROPER );


namespace ResidueIndexMapPredefinedAlphabets{
  typedef enum { amino, DNA, RNA, unknown } seqTypeT;

  inline seqTypeT guessSeqType( const std::string& seq ){
    if( aminoResidueIndexMap.validResidues( seq ) )     return amino;
    if( DNAProperResidueIndexMap.validResidues( seq ) ) return DNA;
    if( RNAResidueIndexMap.validResidues( seq ) )       return RNA;
    return unknown;
  }
}


/* ** Achtung! The methods below are in not wrapped in their own namespace ** */

inline const ResidueIndexMap& residueIndexMapGuessedFromResidues( const std::string& residues ){
  if( DNAProperResidueIndexMap.validResidues( residues ) )  return DNAProperResidueIndexMap;
  if(  DNAWithNResidueIndexMap.validResidues( residues ) )  return DNAWithNResidueIndexMap;
  if(       RNAResidueIndexMap.validResidues( residues ) )  return RNAResidueIndexMap;
  if(     aminoResidueIndexMap.validResidues( residues ) )  return aminoResidueIndexMap;
  std::cout << "Error: failed to guess sequence type:\n";
  exit( -1 );
}


inline const ResidueIndexMap& residueIndexMapFromName( const std::string& seqTypeName ){
  if( seqTypeName == "DNA"   )  return DNAProperResidueIndexMap;
  if( seqTypeName == "dna"   )  return DNAProperResidueIndexMap;
  if( seqTypeName == "RNA"   )  return RNAResidueIndexMap;
  if( seqTypeName == "rna"   )  return RNAResidueIndexMap;
  if( seqTypeName == "amino" )  return aminoResidueIndexMap;
  std::cout << "Error: unknown sequence type name: \"" << seqTypeName << "\"\n";
  exit( -1 );
}


inline const ResidueIndexMap& residueIndexMapFromSeqType( const ResidueIndexMapPredefinedAlphabets::seqTypeT& seqType ){
  if( seqType == ResidueIndexMapPredefinedAlphabets::DNA )    return DNAProperResidueIndexMap;
  if( seqType == ResidueIndexMapPredefinedAlphabets::RNA )    return RNAResidueIndexMap;
  if( seqType == ResidueIndexMapPredefinedAlphabets::amino )  return aminoResidueIndexMap;
  std::cout << "Error: unknown sequence type: \"" << seqType << "\"\n";
  exit( -1 );
}



}; // end namespace cbrc
#endif // _RESIDUEINDEXMAPPREDEFINEDALPHABETS_HH

