/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.12
 *  Last Modified: $Date: 2008/08/13 12:51:35 $
 *
 *  Description: Class to translate nucleic acid sequences into amino acid
 *               sequences, working directly with the residueIndex mapped
 *               representation of the sequences.
 *
 *               Designed with enough flexibility to allow for special codon tables.
 *
 *  Purpose: Created to help map genomic sequences onto protein sequences.
 *
 */
#ifndef RESIDUEINDEXMAPTRANSLATOR_HH_
#define RESIDUEINDEXMAPTRANSLATOR_HH_
#include <iostream>
#include "utils/gdb/gdbUtils.hh"
#include "ResidueIndexMap.hh"

namespace cbrc{

class ResidueIndexMapTranslator{
public:
  /* ********** TYPEDEFS ********** */
  typedef  ResidueIndexMap::indexT   residueIndexT;

  typedef  std::vector<std::string>  stringVecT;


  /* ********** CONSTRUCTORS ********** */

  /* construct an object to translate codons from nucleic acid sequences indexed
   * with nucleicResidueIndexMap, to amino acid sequences indexed with aminoResidueIndexMap.
   *
   * codonTableDescriptor, gives the information to map codons to amino acid residues.
   * To understand the format, look at examples in ResidueIndexMapPredefinedTranslators.hh
   */
  ResidueIndexMapTranslator(  const ResidueIndexMap& nucleicResidueIndexMap,
			      const ResidueIndexMap&   aminoResidueIndexMap,
			      const std::string& codonTableDescriptor  );


  /* ********** METHODS ********** */

  /* translate nucleic acid in [nucleotideBeg, nucleotideEnd) up to
   * the first stop codon, or until final codon before nucleotideEnd
   * if no stop codon is present.
   * Destructively assign the translated sequence to the sequence pointed
   * to by aminoBeg.
   * Sets aminoEnd so that [aminoBeg, aminoEnd) bracket the translated sequence.
   */
  void translateInPlace(  const residueIndexT* const& nucleicBeg,
			  const residueIndexT* const& nucleicEnd,
			  /***/ residueIndexT* const& aminoBeg,
			  const residueIndexT*& /***/ aminoEnd  ) const;


private:
  // return upper bound on the number of distinct codon indices
  size_t numPossibleCodonIndices_upperBound() const;

  // arithmetically map codon starting from nucleotidePtr to index.
  const size_t codonIndex( const residueIndexT* const nucleotidePtr ) const;

  /* 1) arithmetically map codon starting from nucleotidePtr to index.
   * 2) advance nucleotidePtr to next codon.
   *    No check is made to ensure pointer is in range.
   */
  const size_t& nextCodonIndex( const residueIndexT*& nucleotidePtr ) const;

  // object data
  const ResidueIndexMap& nucleicResidueIndexMap;
  const ResidueIndexMap&   aminoResidueIndexMap;
  const std::string        codonTableDescriptor;
  FLENumArray<residueIndexT>        toAminotide; // holds mapping.
};

} // end namespace cbrc
#endif // RESIDUEINDEXMAPTRANSLATOR_HH_
