/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2005, 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2005.8.6
 *  Last Modified: $Date: 2009/09/23 11:12:26 $
 *
 *  Description: See pod at bottom of this file
 *
 */
#ifndef _GDBUTILS_HH_
#define _GDBUTILS_HH_
#include <cstdarg>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>


// a back-slash as the first position of a message, means just print message,
// skipping File and Line number info.
bool inline shouldNotPrintFileLineInfo( const char* message ){
     return(  *message == '\\'  );
}


/* ********** FUNCTIONAL MACROS ********** */

#ifdef NDEBUG
#define RUNTIME_WARN( message )  /* noop */
#else // if NDEBUG not defined
#define RUNTIME_WARN( message )                                             \
  do{                                                                       \
    if(  shouldNotPrintFileLineInfo( message )  ){                          \
      fprintf( stderr, 1+message );  /* "1+" to skip leading backslash */   \
    }                                                                       \
    else{                                                                   \
      fprintf( stderr, "%s:%d Warning: %s", __FILE__, __LINE__, message );  \
    }                                                                       \
  } while(0)
#endif // NDEBUG defined or not


#ifdef NDEBUG
#define IF_WARNF( exp, args... )  /* noop */
#else // if NDEBUG not defined
#define IF_WARNF( exp, args... )                                         \
  do{                                                                    \
    if( exp ){           						 \
      fprintf( stderr, "%s:%d (cerr) Warning: ", __FILE__, __LINE__ );   \
      fprintf( stderr, "\"%s\"\n", #exp );                          	 \
      if(  !gdbUtils::argListEmpty( args )  ){				 \
	fprintf( stderr, " " args );                                     \
	fprintf( stderr, "\n" );					 \
      }                                                                  \
    }									 \
  } while(0)
#endif // NDEBUG defined or not


#ifdef NDEBUG
#define ASSERTF( exp, args... )  /* noop */
#else
#define ASSERTF( exp, args... )			                                   \
  do{                                                                              \
    if( !(exp) ){							           \
      fprintf( stderr, "%s:%d (cerr) Error; ", __FILE__, __LINE__ );	           \
      fprintf( stderr, " assert true failed on the expression: \"%s\"\n", #exp );  \
      fprintf( stdout, "%s:%d (cout) Error; ", __FILE__, __LINE__ );	           \
      fprintf( stdout, " assert true failed on the expression: \"%s\"\n", #exp );  \
      if(  !gdbUtils::argListEmpty( args )  ){				           \
	fprintf( stderr, "(cerr) " args );                                         \
	fprintf( stderr, "\n" );					           \
	fprintf( stdout, "(cout) " args );                                         \
	fprintf( stdout, "\n" );					           \
      }                                                                            \
      exit(-1);								           \
    }									           \
  } while(0)
#endif // NDEBUG defined or not


#ifdef NDEBUG
#define GDB_ASSERTF( exp, args... )  /* noop */
#else
#define GDB_ASSERTF( exp, args... )			                           \
  do{                                                                              \
    if( !(exp) ){							           \
      fprintf( stderr, "%s:%d (cerr) Error; ", __FILE__, __LINE__ );	           \
      fprintf( stderr, " assert true failed on the expression: \"%s\"\n", #exp );  \
      fprintf( stdout, "%s:%d (cout) Error; ", __FILE__, __LINE__ );	           \
      fprintf( stdout, " assert true failed on the expression: \"%s\"\n", #exp );  \
      if(  !gdbUtils::argListEmpty( args )  ){				           \
	fprintf( stderr, "(cerr) " args );                                         \
	fprintf( stderr, "\n" );					           \
	fprintf( stdout, "(cout) " args );                                         \
	fprintf( stdout, "\n" );					           \
      }                                                                            \
      gdbUtils::gdbExit(-1);                                                       \
    }									           \
  } while(0)
#endif // NDEBUG defined or not



// like GDB_ASSERTF, but expression EXP is always executed, not just
// during debugging. This means side effects of EXP can be used.
#define DO_OR_DIEF( exp, args... )			                           \
  do{                                                                              \
    if( !(exp) ){							           \
      fprintf( stderr, "%s:%d (cerr) Error; ", __FILE__, __LINE__ );	           \
      fprintf( stderr, " assert true failed on the expression: \"%s\"\n", #exp );  \
      if(  !gdbUtils::argListEmpty( args )  ){				           \
	fprintf( stderr, " " args );                                               \
	fprintf( stderr, "\n" );					           \
      }                                                                            \
      fprintf( stdout, "%s:%d (cout) Error; ", __FILE__, __LINE__ );	           \
      fprintf( stdout, " assert true failed on the expression: \"%s\"\n", #exp );  \
      if(  !gdbUtils::argListEmpty( args )  ){				           \
	fprintf( stdout, " " args );                                               \
	fprintf( stdout, "\n" );					           \
      }                                                                            \
      gdbUtils::gdbExit( - 1 );                                                    \
    }									           \
  } while(0)



// MESSAGE may be std::string or cstring constant
#define GDB_DIE( message )				              \
  do{                                                                 \
   fprintf( stderr, "%s:%d (cerr) Error; ", __FILE__, __LINE__ );     \
   std::cerr << message << std::endl;                                 \
                                                                      \
   fprintf( stdout, "%s:%d (cout) Error; ", __FILE__, __LINE__ );     \
   std::cout << message << std::endl;                                 \
                                                                      \
   gdbUtils::gdbExit( -1 );                                           \
} while(0)



#define GDB_DIEF( args... )				              \
  do{                                                                 \
   fprintf( stderr, "%s:%d (cerr) Error;", __FILE__, __LINE__ );      \
   fprintf( stderr, " " args );					      \
   fprintf( stderr, "\n" );                                           \
   fprintf( stdout, "%s:%d (cout) Error;", __FILE__, __LINE__ );      \
   fprintf( stdout, " " args );					      \
   fprintf( stdout, "\n" );                                           \
   gdbUtils::gdbExit( -1 );                                           \
} while(0)



/* ********** DEPRECATED MACROS ********** */
#define GDB_ASSERT( exp )  GDB_ASSERTF( exp )


/* ********** PRIVATE FUNCTIONS ********** */

namespace gdbUtils{

  /* 
   * the following two argListEmpty() overloaded functions collectively
   * have the effect of detecting if a macro argument ARG is defined
   * in the expression "argListEmpty( ARG )"
   */
  inline bool argListEmpty(){
    return true;
  }
  inline bool argListEmpty( const char* const& format, ... ){
    return false;
  }

  // if GDB_DEBUG defined, go into infinite loop;  otherwise do nothing.
  inline void gdbTrap(){
#ifdef GDB_DEBUG
    std::cerr << "gdbAssert failure trapped. cntl-c to break.\n";
    for(;;);  // trap, so that one can get a backtrace from gdb.
#endif // defined GDB_DEBUG
  }

  // trap if GDB_DEBUG, otherwise exit with STATUS
  inline void gdbExit( const int status = -1 ){
    gdbTrap();
    exit( status );
  }
  
} // end namespace gdbUtils


/* ********** DEPRECATED CODE ********** */
/* Achtung! the functions in namespace cbrc are deprecated!! */
namespace cbrc{
  inline void gdbExit( const int status = -1 ){
    std::cerr << "Warning: calling deprecated function \"gdbExit\"!\n";
    exit( status );
  }
} // end namespace cbrc


#endif // defined _GDBUTILS_HH_
#if 0 // ** start pod **
=pod


=head1 NAME

B<gdbUtils>


=head1 SYNOPSIS

Some macros useful for debugging.

=head1 DESCRIPTION

Provides some functionality useful for debugging. Some specific to
gdb.  Macros are used, which enables the calling source file and line
to be automatically included in the error messages.

Functional macros print filename and line number information of the
calling program, along with optional user provided B<printf> format
error messages. Error message are printed to both stdout and stderr,
minimizing the chance of an error going unnoticed; for example when
the program is run in the background or as part of a pipe in an
unwatched terminal.

=head2 Terminology

"gdb-trap-exit" means:

If macro B<GDB_DEBUG> defined,
fall into an infinite loop. This is useful to get a backtrace in gdb.
if B<GDB_DEBUG> is not defined, terminate with C<exit(-1)>. 


=head1 FUNCTION MACROS

  GDB_DIEF( [format], [args...] )

Print error message giving the current file and line. If I<format> is
given, C<printf( format, args )> is called to add to the
error message. Finally, gdb-trap-exit.

  GDB_DIE( message )

Like GDB_DIEF with single argument, except that I<message> can
be a C<std::string>.

  GDB_ASSERTF( expression, [format], [args...] )

If I<expression> evaluates to true, do nothing. Otherwise generate and
output an error message similar to L<GDB_DIEF>, but mentioning the
I<expression> tested as well, then gdb-trap-exit.
Compiles to noop if B<NDEBUG> is defined.

  DO_OR_DIEF( expression, [format] [args...] )

Like B<GDB_ASSERTF>,
except I<expression> is executed even if B<NDEBUG> is defined.

  ASSERTF( expression, [format], [args...] )

Like B<GDB_ASSERTF>,
except the code uses orginary C<exit(-1)> instead of gdb-trap-exit.

=head2 Warn Macros

  RUNTIME_WARN( message )

Unless B<NDEBUG> is defined, print source file and line number followed
by I<message> to std::cerr.

  IF_WARNF( expression, [format], [args...] )

If I<expression> evaluates to true, print warning message
including the filename and line of the calling program. If
I<format> is given, C<printf( format, args )> is called to add to the
warning message. The warning message is printed to B<stderr> only.
This macro does not gdb-trap-exit.

=head2 Suppressing the Printing of File and Line Number

Some macros defined here can be made to print just the message
by prepending a backslash to the message:

  #RUNTIME_WARNING( "\\Calling copy constructor for class Foo\n" );

Note that inside a C-string constant, two backslash characters are needed
to stipulate one actual backslash.


=head1 CONTROL MACROS

  NDEBUG

Set B<NDEBUG> to remove the overhead (and checking of course!) of B<GDB_ASSERTF>.

  GDB_DEBUG

Set B<GDB_DEBUG> to cause the L<FUNCTIONAL MACROS> to go into an infinite
loop instead of exiting. This is useful to get a backtrace when executing
in gdb.

=head1 EXAMPLES

Below examples are given for B<GDB_ASSERTF>. Except for the lack of
an expression to test for, the usage of B<GDB_DIEF> is almost exactly the same.

=head2 Full argument list

  GDB_ASSERTF( pi == 3.14,
	       "incorrect value of pi = \"%f\", and %s is falling!",
	       pi, fallingThing.c_str() );

Prints something like:

  tryGdbUtils_assertf.cc:27 (cerr) Error;  assert true failed on the expression: "pi == 3.14"
   incorrect value of pi = "3.141590", and the sky is falling!
  tryGdbUtils_assertf.cc:27 (cout) Error;  assert true failed on the expression: "pi == 3.14"
   incorrect value of pi = "3.141590", and the sky is falling!

=head2 Single error message argument

  GDB_ASSERTF( pi == 3.14, "incorrect value of pi!" );

Prints something like:

  tryGdbUtils_assertf.cc:31 (cerr) Error;  assert true failed on the expression: "pi == 3.14"
   incorrect value of pi!
  tryGdbUtils_assertf.cc:31 (cout) Error;  assert true failed on the expression: "pi == 3.14"
   incorrect value of pi!

=head2 No error message argument

  GDB_ASSERTF( pi == 3.14 );

Prints something like:

  tryGdbUtils_assertf.cc:35 (cerr) Error;  assert true failed on the expression: "pi == 3.14"
  tryGdbUtils_assertf.cc:35 (cout) Error;  assert true failed on the expression: "pi == 3.14"


=head1 BUGS

When the I<format> argument of the functional macros is defined, a single
space character is prepended to it. This is due to a technical detail
of the macro implementation. Please let the author know if you know of
a way to avoid this minor snag.

=head1 COPYRIGHT

Copyright (C) 2005, 2008.

=head1 LICENSE

This file can be used under the GNU Public library license.

=head1 AUTHOR

Paul Horton

=cut

#endif // ** end of pod **
