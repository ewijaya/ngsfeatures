/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.10.7
 *  Last Modified: $Date: 2009/10/08 01:43:12 $
 *
 *  Description: complement DNA residues
 *
 *  Usage:  complementDNA('a') -->  returns 't'
 */
#ifndef COMPLEMENTDNA_HH_
#define COMPLEMENTDNA_HH_
#include <iostream>
#include "utils/gdb/gdbUtils.hh"


namespace cbrc{


static  const char  toComplement[]  =
  {3,    // 0 (a) --> 3 (t)
   2,    // 1 (c) --> 2 (g)
   1,    // 2 (g) --> 1 (c)
   0,    // 3 (t) --> 0 (a)
   255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
   84,   //  A --> T
   255,
   71,   //  C --> G
   255,255,255,
   67,   //  G --> C
   255,255,255,255,255,255,255,255,255,255,255,255,
   65,   //  T --> A
   255,255,255,255,255,255,255,255,255,255,255,255,
   116,  //  a --> t
   255,
   103,  //  c --> g
   255,255,255,
   99,   //  g --> c
   255,255,255,255,255,255,255,255,255,255,255,255,
   97,   //  t --> a
   255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
  };


   
class ComplementDNA{
public:
  typedef  unsigned  u;

  ComplementDNA(){}

  char operator()(  const char& residue  ) const{

    GDB_ASSERTF(  255 >  (u) toComplement[ (u) residue],
		  "complement of residue %c (%u) not defined", residue, (u) residue  );

    return toComplement[ (unsigned) residue];
  }

}; // end class complementDNA


const  ComplementDNA  complementDNA;


} // end namespace cbrc
#endif // COMPLEMENTDNA_HH_
