/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003,2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.8
 *  Last Modified: $Date: 2008/06/08 05:49:12 $
 *
 *  Description: The not yet adopted "<<<" operator. Achitecture specific.
 *
 */
#ifndef _BITLEFTROTATE_HH
#define _BITLEFTROTATE_HH
#include <iostream>

namespace cbrc{

#if defined(__i386__) || defined(__x86_64__)
#define BIT_ROTATE_LEFT( x )\
__asm__( "mov %1, %0\n\t"\
	 "rol %1\n\t"\
	 "mov %0, %1"\
	 : "=q" (x)\
	 : "0" (x) )
#else // unless defined(__i386__) || defined(__x86_64__)
#define BIT_ROTATE_LEFT( x ) (x = (x >> 31 | x << 1))
#endif 


}; // end namespace cbrc

#endif // defined _BITLEFTROTATE_HH_
