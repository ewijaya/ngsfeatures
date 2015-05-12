/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004,2009, Paul Horton, All rights reserved.
 *  Creation Date: 2004.11.26
 *  Last Modified: $Date: 2009/11/01 07:02:08 $
 *  
 *  Description: genebee colors for amino acids.
 *
 *  Purpose: Useful for displaying multiple alignments in html.
 *
 *  Reference: http://www.genebee.msu.su/services/aligraf/help.html#GROUPS
 *             I do not know if they took their colors from some other source or developed their own.
 *  Usage: residueColorGenebee( 'A' ) --> returns color string for Alanine.
 */
#ifndef _RESIDUECOLORGENEBEE_HH_
#define _RESIDUECOLORGENEBEE_HH_
#include <string.h>
#include "ResidueColor.hh"


namespace cbrc{

class ResidueColorGenebee : public ResidueColor{
public:
  std::string getColor(  const char& residue  ) const{
    return(
	   strchr( "AG",     residue) ?  "#95CC95"   :
	   strchr( "C",      residue) ?  "#6EA56E"   :
	   strchr( "DENQBZ", residue) ?  "#4A814A"   :
	   strchr( "ILMV",   residue) ?  "#64A8FF"   :
	   strchr( "FWY",    residue) ?  "#645AFF"   :
	   strchr( "H",      residue) ?  "#6400C0"   :
	   strchr( "KR",     residue) ?  "#FFC300"   :
	   strchr( "P",      residue) ?  "#FF8600"   :
	   strchr( "ST",     residue) ?  "#FF1200"   :
	   /* fallback color */          "black"
	   );
  }
private:
};


// predined ResidueColorGenebee object.
static const ResidueColorGenebee  residueColorGenebee;


}; // end namespace cbrc
#endif // defined _RESIDUECOLORGENEBEE_HH_
