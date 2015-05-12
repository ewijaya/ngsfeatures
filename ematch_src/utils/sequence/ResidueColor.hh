/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright (C) 2004,2009, Paul Horton, All rights reserved.
 *  Creation Date: 2004.11.26
 *  Last Modified: $Date: 2009/11/01 07:04:19 $
 *  
 *  Description: Give Colors to Residues.
 *
 */

#ifndef _RESIDUECOLOR_HH_
#define _RESIDUECOLOR_HH_

namespace cbrc{

class ResidueColor{
public:
  virtual std::string getColor(  const char& residue  ) const = 0;
  virtual ~ResidueColor(){}
private:
};

}; // end namespace cbrc
#endif // defined _RESIDUECOLOR_HH_

