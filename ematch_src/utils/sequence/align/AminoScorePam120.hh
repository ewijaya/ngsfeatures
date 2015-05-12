/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: 2004, 2007, Paul B. Horotn, All rights reserved
 *  Creation Date: 2004.5.10
 *  Last Modified: $Date: 2009/04/15 12:57:03 $
 *  
 *  Description: PAM120 amino acid score matrix.
 *               Defines const aminoScorePam120
 *
 *  Caveat: The entries for selenocysteine
 *          (and possible for some other non-standard amino acid -- I do not remember)
 *          were added by Paul Horton in a mostly arbitrary way.
 */
#ifndef _AMINOSCOREPAM120_HH_
#define _AMINOSCOREPAM120_HH_

#include "utils/sequence/align/AminoScore.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMapPredefined.hh"

namespace cbrc{

const static alignScoreT PAM120[CBRC_AMINO_SIGMA][CBRC_AMINO_SIGMA] =
//        A  R  N  D  B  C  Q  E  Z  G  H  I  L  K  M  F  P  S  T  W  Y  V  U  X 
    {   { 3,-3,-1, 0,-0,-3,-1, 0,-0, 1,-3,-1,-3,-2,-2,-4, 1, 1, 1,-7,-4, 0, 1,-7}, // A  alanine
/* R */	{-3, 6,-1,-3,-3,-4, 1,-3,-3,-4, 1,-2,-4, 2,-1,-5,-1,-1,-2, 1,-5,-3,-1,-5}, // R  arginine
/* N */	{-1,-1, 4, 2, 2,-5, 0, 1,-0, 0, 2,-2,-4, 1,-3,-4,-2, 1, 0,-4,-2,-3, 1,-4}, // N  asparagine
/* D */	{ 0,-3, 2, 5, 2,-7, 1, 3, 1, 0, 0,-3,-5,-1,-4,-7,-3, 0,-1,-8,-5,-3, 0,-8}, // D  aspartic acid
/* B */	{-0,-3, 2, 2, 2,-7,-0, 1,-0,-0,-0,-3,-5,-1,-4,-7,-3,-0,-1,-8,-5,-3,-0,-8}, // B  N or D
/* C */	{-3,-4,-5,-7,-7, 9,-7,-7,-7,-4,-4,-3,-7,-7,-6,-6,-4, 0,-3,-8,-1,-3, 0,-8}, // C  cysteine
/* Q */	{-1, 1, 0, 1,-0,-7, 6, 2, 2,-3, 3,-3,-2, 0,-1,-6, 0,-2,-2,-6,-5,-3,-2,-7}, // Q  glutamine
/* E */	{ 0,-3, 1, 3, 1,-7, 2, 5, 2,-1,-1,-3,-4,-1,-3,-7,-2,-1,-2,-8,-5,-3,-1,-8}, // E  glutamic acid
/* Z */ {-0,-3,-0, 1,-0,-7, 2, 2, 2,-3,-1,-3,-4,-1,-3,-7,-2,-2,-2,-8,-5,-3,-2,-8}, // Z  Q or E, or yield E on acid hydrolysis
/* G */	{ 1,-4, 0, 0,-0,-4,-3,-1,-3, 5,-4,-4,-5,-3,-4,-5,-2, 1,-1,-8,-6,-2, 1,-7}, // G  glycine
/* H */	{-3, 1, 2, 0,-0,-4, 3,-1,-1,-4, 7,-4,-3,-2,-4,-3,-1,-2,-3,-3,-1,-3,-2,-4}, // H  histidine
/* I */	{-1,-2,-2,-3,-3,-3,-3,-3,-3,-4,-4, 6, 1,-3, 1, 0,-3,-2, 0,-6,-2, 3,-2,-6}, // I  isoleucine
/* L */	{-3,-4,-4,-5,-5,-7,-2,-4,-4,-5,-3, 1, 5,-4, 3, 0,-3,-4,-3,-3,-2, 1,-4,-7}, // L  leucine
/* K */	{-2, 2, 1,-1,-1,-7, 0,-1,-1,-3,-2,-3,-4, 5, 0,-7,-2,-1,-1,-5,-5,-4,-1,-7}, // K  lysine
/* M */	{-2,-1,-3,-4,-4,-6,-1,-3,-3,-4,-4, 1, 3, 0, 8,-1,-3,-2,-1,-6,-4, 1,-2,-6}, // M  methionine
/* F */	{-4,-5,-4,-7,-7,-6,-6,-7,-7,-5,-3, 0, 0,-7,-1, 8,-5,-3,-4,-1, 4,-3,-3,-7}, // F  phenylalanine
/* P */	{ 1,-1,-2,-3,-3,-4, 0,-2,-2,-2,-1,-3,-3,-2,-3,-5, 6, 1,-1,-7,-6,-2, 1,-7}, // P  proline
/* S */	{ 1,-1, 1, 0,-0,-0,-2,-1,-2, 1,-2,-2,-4,-1,-2,-3, 1, 3, 2,-2,-3,-2, 2,-4}, // S  serine
/* T */	{ 1,-2, 0,-1,-1,-3,-2,-2,-2,-1,-3, 0,-3,-1,-1,-4,-1, 2, 4,-6,-3, 0, 2,-6}, // T  threonine
/* W */	{-7, 1,-4,-8,-8,-8,-6,-8,-8,-8,-3,-6,-3,-5,-6,-1,-7,-2,-6, 12,2,-8,-2,-8}, // W  tryptophan
/* Y */	{-4,-5,-2,-5,-5,-1,-5,-5,-5,-6,-1,-2,-2,-5,-4, 4,-6,-3,-3,-2, 8,-3,-3,-6}, // Y  tyrosine
/* V */	{ 0,-3,-3,-3,-3,-3,-3,-3,-3,-2,-3, 3, 1,-4, 1,-3,-2,-2, 0,-8,-3, 5,-2,-8}, // V  valine
/* U */	{ 1,-1, 1, 0,-0,-0,-2,-1,-2, 1,-2,-2,-4,-1,-2,-3, 1, 2, 2,-2,-3,-2,12,-4}, // U  selenocysteine
/* X */ {-7,-5,-4,-8,-8,-8,-7,-8,-8,-7,-4,-6,-7,-7,-6,-7,-7,-4,-6,-8,-6,-8,-4,-8} };//X  unknown


const static alignScoreT PAM120MaxScore[CBRC_AMINO_SIGMA] =
//  A   R   N   D   B   C   Q   E   Z   G   H   I   L   K   M   F   P   S   T   W   Y   V   U   X
{   3,  6,  4,  5,  2,  9,  6,  5,  2,  5,  7,  6,  5,  5,  8,  8,  6,  3,  4, 12,  8,  5, 12, -4 };


class AminoScorePam120 : public AminoScore{
public:
  // should have a constructor that reads in a matrix.
  alignScoreT maxScore() const{ return  12; }

  // Minimun of { score for any residue pair, gapInitiation, and gapExtension }, algorithms rely on this!
  alignScoreT minScore() const{ return -10; }

  alignScoreT gapInitiation() const{ return -10; }
  alignScoreT gapExtension()  const{ return  -8; }

  alignScoreT score( const ResidueIndexMap::indexT& r1,
		     const ResidueIndexMap::indexT& r2 ) const{
    return PAM120[r1][r2];
  }

  alignScoreT maxScore( const ResidueIndexMap::indexT& r1 ) const{
    return PAM120MaxScore[r1];
  }

  const ResidueIndexMap& residueIndexMap() const{ return aminoResidueIndexMap; }
private:
};


const AminoScorePam120 aminoScorePam120;


} // end namespace cbrc

#endif // defined _AMINOSCOREPAM120_HH_
