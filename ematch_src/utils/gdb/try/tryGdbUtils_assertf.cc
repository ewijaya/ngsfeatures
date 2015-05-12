/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.7.2
 *  Last Modified: $Date: 2008/07/24 10:11:22 $
 *
 *  Purpose: try code involving GDB_ASSERTF macro in gdbUtils.hh
 */
#include <iostream>
#include "../gdbUtils.hh"


void tryGdbUtils_assertf(){
  double pi = 3.1416;
  pi -= 0.00001;

  std::string fallingThing( "the sky" );


#if    TEST_CASE == 0
#warning "Compiling to try with full argument list"
  std::cout << "testing with full argument list...\n";
  GDB_ASSERTF( pi == 3.14,
	       "incorrect value of pi = \"%f\", and %s is falling!",
	       pi, fallingThing.c_str() );

#elif  TEST_CASE == 1
#warning "Compiling to try with single argument"
  std::cout << "testing with single argument...\n";
  GDB_ASSERTF( pi == 3.14, "incorrect value of pi!" );

#else
#warning "Compiling to try with no argument"
  std::cout << "testing with no argument...\n";
  GDB_ASSERTF( pi == 3.14 );
#endif // TEST_CASE switch
}


int main( int argc, const char* argv[] ){

  if( argc > 1 ){
    std::cout << "No command line args needed.\n"
	      << "\n"
	      << "Try compiling with:\n"
	      << "  * TEST_CASE  defined as 0, 1 or 2\n"
	      << "  * GDB_DEBUG  defined or not\n"
	      << "  * NDEBUG     defined or not\n\n";
    exit( 1 );
  }
  
  tryGdbUtils_assertf();
  return 1;
}
