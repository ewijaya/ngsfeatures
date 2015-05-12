/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.7.2
 *  Last Modified: $Date: 2008/07/24 13:45:26 $
 *
 *  Purpose: try code involving IF_WARNF macro in gdbUtils.hh
 */
#include <iostream>
#include "../gdbUtils.hh"


void tryGdbUtils_if_warnf(){
  double pi = 3.1416;
  pi -= 0.00001;

  std::string fallingThing( "the sky" );


#if    TEST_CASE == 0
#warning "Compiling to try with full argument list"
  std::cout << "testing with full argument list...\n";
  IF_WARNF( pi != 3.14,
	       "incorrect value of pi = \"%f\", and %s is falling!",
	       pi, fallingThing.c_str() );

#elif  TEST_CASE == 1
#warning "Compiling to try with single argument"
  std::cout << "testing with single argument...\n";
  IF_WARNF( pi != 3.14, "incorrect value of pi!" );

#else
#warning "Compiling to try with no argument"
  std::cout << "testing with no argument...\n";
  IF_WARNF( pi != 3.14 );
#endif // TEST_CASE switch

  std::cout << "\nTest finished. Have a nice day.\n";
}


int main( int argc, const char* argv[] ){

  if( argc > 1 ){
    std::cout << "No command line args needed.\n"
	      << "\n"
	      << "Try compiling with:\n"
	      << "  * TEST_CASE  defined as 0, 1 or 2\n"
	      << "  * NDEBUG     defined or not\n\n";
    exit( 1 );
  }
  
  tryGdbUtils_if_warnf();
  return 1;
}
