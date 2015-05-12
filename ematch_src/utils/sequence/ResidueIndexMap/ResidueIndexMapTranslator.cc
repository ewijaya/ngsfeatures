/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.12
 *  Last Modified: $Date: 2008/08/13 12:52:52 $
 *  Description: See header file.
 */
#include "utils/perlish/perlish.hh"
#include "ResidueIndexMapTranslator.hh"

#define UNDERTERMINED_CODON_INDEX  255
#define          STOP_CODON_INDEX  254


namespace cbrc{


ResidueIndexMapTranslator
::ResidueIndexMapTranslator(  const ResidueIndexMap& nucleicResidueIndexMap,
			      const ResidueIndexMap&   aminoResidueIndexMap,
			      const std::string& codonTableDescriptor  )
  : nucleicResidueIndexMap( nucleicResidueIndexMap ),
      aminoResidueIndexMap(   aminoResidueIndexMap ),
    codonTableDescriptor( codonTableDescriptor )
{

  /* ** initialize text parsing constants ** */
  const std::string interCodonDelimiter(  "\t+"  );

  const std::string intraCodonDelimiter(  ":"   );

  const std::string stopCodonLiteral(  "stop"  );


  // initialize toAminotide
  toAminotide.setSize(  numPossibleCodonIndices_upperBound()  );
  toAminotide.fill(  UNDERTERMINED_CODON_INDEX  );
  

  stringVecT codonDescriptors
    = perlish::split( interCodonDelimiter, codonTableDescriptor );

  for( stringVecT::const_iterator curCodonPtr = codonDescriptors.begin();
       curCodonPtr != codonDescriptors.end();
       ++curCodonPtr ){

    /* ** separate strings describing codon and amino acid ** */
    stringVecT codonAminoPair
      = perlish::split( intraCodonDelimiter, *curCodonPtr );

    assert(  codonAminoPair.size() == 2  ); // codon & amino acid

    const std::string& codonString        = codonAminoPair[0];
    const std::string& aminoResidueString = codonAminoPair[1];


    // residue indexed version of current codon
    const ResidueIndexMap::arrayT residueIndexedCodon
      = nucleicResidueIndexMap.toResidueIndices( codonString );

    // index into toAminotide for current codon
    const size_t curCodonIndex = codonIndex( &residueIndexedCodon[0] );


    if(  aminoResidueString == stopCodonLiteral  ){

      toAminotide[curCodonIndex]  =  STOP_CODON_INDEX;

    }
    else{ 
      /* ** If codon for ordinary amino acid residue ** */

      GDB_ASSERTF(  aminoResidueString.size() == 1,
		    "expected single character representing an amino acid,"
		    "but instead got string \"%s\"", aminoResidueString.c_str()
		    );

      const ResidueIndexMap::residueT aminoResidue = aminoResidueString[0];

      GDB_ASSERTF(  aminoResidueIndexMap.validResidue( aminoResidue ),
		    "expected single character representing an amino acid,"
		    "but instead got character (%c), with ascii code (%u)",
		    aminoResidue, (unsigned) aminoResidue
		    );

      // set appropriate entry of toAminotide, to map codon to its amino acid residueIndex
      toAminotide[curCodonIndex]  =  aminoResidueIndexMap.toResidueIndex( aminoResidue );

    } // end stop/ordinary codon if

  } // end for current codon descriptor

} // end constructor ResidueIndexMapTranslator( nucleicResidueIndexMap,, )

    

void ResidueIndexMapTranslator
::translateInPlace(  const residueIndexT* const& nucleicBeg,
		     const residueIndexT* const& nucleicEnd,
		     /***/ residueIndexT* const& aminoBeg,
		     const residueIndexT*& /***/ aminoEnd  ) const{

  assert(  nucleicEnd >= nucleicBeg  );

  /* ** Initailize pointers ** */
  residueIndexT* curAminoPtr  = aminoBeg;

  const residueIndexT* finalCodonEnd
    =  nucleicEnd  -  ( (nucleicEnd - nucleicBeg) % 3 );

  const residueIndexT* curNucleotidePtr = nucleicBeg;


  /* ** Loop, processing one codon at a time ** */
  while( curNucleotidePtr < finalCodonEnd ){

    // LOOP PROGRESS happens in nextCodonIndex.
    const size_t curCodonIndex = nextCodonIndex( curNucleotidePtr );

    GDB_ASSERTF(  toAminotide( curCodonIndex )  !=  UNDERTERMINED_CODON_INDEX,
		  "encountered undetermined codon, corresponding bytes are: %u:%u:%u",
		  (unsigned) *curNucleotidePtr,
		  (unsigned) *(1+curNucleotidePtr),
		  (unsigned) *(2+curNucleotidePtr)
		  );

    if(  toAminotide( curCodonIndex )  ==  STOP_CODON_INDEX  ){
      break;  // encountered stop codon, so EXIT.
    }

    // else translate codon and advance curAminoPtr.
    *curAminoPtr++  =  toAminotide( curCodonIndex );

  } // end open reading frame while loop

  aminoEnd = curAminoPtr;  // "return" value of aminoEnd to caller.
  
} // end method translateInPlace.



size_t ResidueIndexMapTranslator::numPossibleCodonIndices_upperBound() const{

    size_t retVal =  nucleicResidueIndexMap.sigma()
      * nucleicResidueIndexMap.sigma()
      * nucleicResidueIndexMap.sigma();

    GDB_ASSERTF(  retVal < STOP_CODON_INDEX,
		  "σ^3 (%zu) ≧ %u, exceeding the range of ResidueIndexMap::indexT",
		  retVal, STOP_CODON_INDEX
		  );

    return retVal;
} // end method numPossibleCodonIndices



const size_t ResidueIndexMapTranslator
::codonIndex( const residueIndexT* const nucleotidePtr ) const{

  const size_t sigma = nucleicResidueIndexMap.sigma();
  /***/ size_t codonIndex;

  codonIndex  =  sigma * *nucleotidePtr;
  codonIndex +=  *(1+nucleotidePtr);
  codonIndex *=  sigma;
  codonIndex +=  *(2+nucleotidePtr);

  return  codonIndex;
} // end method advanceCodon_returnIndex



const size_t& ResidueIndexMapTranslator
::nextCodonIndex( const residueIndexT*& nucleotidePtr ) const{


  static const size_t sigma = nucleicResidueIndexMap.sigma();
  static /***/ size_t codonIndex;

  codonIndex  =  sigma * *nucleotidePtr++;
  codonIndex +=  *nucleotidePtr++;
  codonIndex *=  sigma;
  codonIndex +=  *nucleotidePtr++;

  return  codonIndex;
} // end method advanceCodon_returnIndex


} // end namespace cbrc

