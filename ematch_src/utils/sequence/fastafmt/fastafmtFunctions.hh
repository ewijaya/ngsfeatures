/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.4.27
 *  Last Modified: $Date: 2009/04/29 08:42:15 $
 *
 *  Description: Functions for working with fastafmt.
 *
 *  See Also: FastaReader
 *
 */
#ifndef FASTAFMTFUNCTIONS_HH_
#define FASTAFMTFUNCTIONS_HH_
#include <iostream>

namespace cbrc{

namespace fastafmtFunctions{

  // return number of fastafmt records in string iStream.
  size_t countRecords( std::istream& iStream );

  // skip up to numToSkip records in iStream (stop if EOF encountered)
  // assumes iStream currently points to start of record or EOF
  // returns number of records skipped.
  size_t skipRecords( std::istream& iStream, const size_t& numToSkip );

  // records from iStream until EOF encountered.
  // assumes iStream currently points to start of record or EOF
  // read records are output to oStream
  // returns number of output records.
  size_t printRecords( std::istream& iStream,
		       std::ostream& oStream = std::cout
		       );

  // read up to numToPrint records from iStream (stop if EOF encountered)
  // assumes iStream currently points to start of record or EOF
  // read records are output to oStream
  // returns number of output records.
  size_t printRecords( /***/ std::istream&  iStream,
		       const size_t&        numToPrint,
		       /***/ std::ostream&  oStream = std::cout
		       );

} // end namespace fastafmtFunctions


} // end namespace cbrc
#endif // FASTAFMTFUNCTIONS_HH_



#if 0 // ** start pod **
=pod



=head1 NAME

fastafmtFunctions



=head1 SYNOPSIS

Utility functions to process fasta format files.



=head1 DESCRIPTION


Utility functions to process fasta format files. 

=head2 fastafmt

The term "fastafmt" is a generalization of multifasta format sequence
files. A fastafmt file only assumes that records start with ">" as the
first byte of the stream or directly following a new line.



=head1 FUNCTIONS

    size_t countRecords( std::istream& iStream );

Returns the number of fastafmt records in stream iStream.

    size_t skipRecords( std::istream& iStream, const size_t& numToSkip );

Skips up to numToSkip records in iStream (stop if EOF encountered)
assumes iStream currently points to start of record or EOF.
returns number of records skipped.

    size_t printRecords( std::istream& iStream,
                         std::ostream& oStream = std::cout
                         );

    size_t printRecords( /***/ std::istream&  iStream,
                         const size_t&        numToPrint,
                         /***/ std::ostream&  oStream = std::cout
		         );

These functions read records from I<iStream> and print them to
I<oStream>. They stop after reading I<numToPrint> records (if given)
or after encountering an EOF.  The assume iStream currently points to
start of record or EOF. and return the number of records output.


=head2 Argument order and defualt values

For functions that input streams or files that argument comes first. This gives
the functions a semi-object oriented feel. Output stream reference arguments are
given a default value of std::cout where possible, but input reference arguments
are not given a default value.



=head1 BUG REPORTS

Send bug reports to horton-p at aist.go.jp
Please put 'fastafmtFunctions' in the email subject line.



=head1 AUTHOR

Paul Horton



=head1 COPYRIGHT

Copyright (C) 2009

=cut

#endif // ** end of pod **

