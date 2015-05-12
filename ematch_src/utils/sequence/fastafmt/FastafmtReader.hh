/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.10.20
 *  Last Modified: $Date: 2008/02/03 23:24:43 $
 *
 *  Description: Class for reading fasta format sequence files.
 *
 *  Caveats: Comment lines are currently only partially supported.
 *           a commentMarker string (defaults to "#") can be
 *           supplied and any lines starting with that string are ignored.
 *
 *           In the future, I intend to save the comments in an array and
 *           provide accessor functions for them, similar to the Perl module
 *           I recently wrote. PH. 2007/01/30.
 *
 */

#ifndef _FASTAFMTREADER_HH_
#define _FASTAFMTREADER_HH_
#include <string>
#include <vector>
#include <iosfwd>
#include "FastafmtConstants.hh"
#include "FastafmtInputOptions.hh"
#include "FastafmtRecord.hh"

namespace cbrc{

class FastafmtReader{
public:
  // Constructor
  FastafmtReader( FastafmtInputOptions options = defaultFastaInputOptions )
    : _fastafmtInputOptions( options ),
    _lineLengthForPrintingSequence(60)
  {
    nextRecordHead = "";
    _commentMarker = "#";
  }


  /* ******* methods which read from the stream ******* */
  bool readRecord( std::istream& is ); // returns false if no more records left in stream.

  std::vector<FastafmtRecord> slurpSeqs( std::istream& is );

  // call this when the stream is changed between readRecord() calls.
  void resetToReadFreshStream(){ nextRecordHead = ""; }



  /* #################### ACCESSORS #################### */

  /* ***** accessors to current record data variables ***** */
  // first line of record as it appeared in input stream.
  const std::string& curHead() const{ 
    return curRecordHead;
  }

  FastafmtRecord curRecord() const{
    return _curRecord;
  }

  const std::string& seq() const{
    return curRecord().seq();
  }

  /* ***** accessors to object parameters ***** */
  const FastafmtInputOptions& fastafmtInputOptions() const{
    return _fastafmtInputOptions;
  }

  FastafmtInputOptions& fastafmtInputOptionsRef(){
    return _fastafmtInputOptions;
  }

  const size_t& lineLengthForPrintingSequence() const{
    return _lineLengthForPrintingSequence; }

  const std::string& commentMarker() const{
    return _commentMarker; }

  /* Change the sequence to all uppercase (MCF 2007-11-13) */
  void upcaseSequence();


private:
	
  // get next non-comment line or return false.
  bool getNoncommentLine( std::istream& is, std::string& line ) const;

  bool isComment( const std::string& line ) const;

  bool readRecordAux( std::istream& is );

  /* ******* object data ******* */
  // Parameters
  FastafmtInputOptions _fastafmtInputOptions;

  size_t      _lineLengthForPrintingSequence;

  // lines starting with commentMarker are interpreted as comments.
  // null string, means no line is considered a comment.
  std::string _commentMarker; 


  // data for current record.

  FastafmtRecord _curRecord;
  std::string curRecordHead;
  std::string nextRecordHead;
};


// Print current record to os
std::ostream& operator<<( std::ostream& os, const FastafmtReader& fr );
  

} // end namespace cbrc.
#endif // defined _FASTAFMTREADER_HH_
#if 0 // ** start pod **
=pod

=head1 NAME

B<FastafmtReader>

=head1 SYNOPSIS

Reader of (multi)fastafmt streams.

=head1 DESCRIPTION

Reader of (multi)fastafmt streams.

=head1 Definitions

By our definition of the fastafmt file format, a fastafmt stream consists of a series
of I<records>, possibly interspersed with I<comments>. A record consists
of a I<head>, which is one line starting with the character '>'. The head
(sans the initial '>') and any following white space characters is considered
to be a list of fields separated by the C<delimiterPattern>. The first
field is called the I<name> and is sometimes considered mandatory.

=head1 Constructor

  FastafmtReader( std::string delimiterPattern = "[ \t]+" );

The only argument is the optional C<delimiterPattern> argument, which defaults to
C<"[ \t]+">, splitting on any continguous block of white space. To allow
spaces in (tab separated) fields use C<FastafmtReader( "\t+" )>.

=head1 Relation to other classes

Some methods return records at L<LabeledAsciiSeq> and others
L<LabeledSequence>.  L<LabeledAsciiSeq> and L<LabeledSequence>. differ
in that L<LabeledSequence> converts the sequence into a
L<ResidueIndexMap>ed form. L<LabeledAsciiSeq> was created for the LAST
project, while L<LabeledSequence> was created for WoLF PSORT.

=head1 Reading Records

=head2 One Record at a time

  bool readRecord( std::istream& is ); // returns false if no more records left in stream.

The terminal '*' character sometimes found appended to the sequences in fastafmt files,
is removed if present.

=head2 Entire stream at once

Two methods are provided to "slurp" an entire fastafmt stream in at once.

  std::vector<LabeledAsciiSeq> slurpSeqs( std::istream& is );

  std::vector<LabeledSequence>
  slurpLabeledSequences( std::istream& is,
			 const seqTypes::seqType& seqT = seqTypes::unknown );

L<LabeledAsciiSeq> and L<LabeledSequence> differ in that L<LabeledSequence>
converts the sequence into a L<ResidueIndexMap>ed form. L<LabeledAsciiSeq> was
created for the LAST project, while L<LabeledSequence> was created for
WoLF PSORT.

=head1 Accessors

=head2 Sequence

  const std::string& sequence() const;
  void setSequence( const std::string& sequence );

These do what you think.

  LabeledAsciiSeq getLabeledAsciiSeq() const;
  LabeledSequence getLabeledSequence( seqTypes::seqType seqT ) const;

These return classes which contain the labels and sequence.

=head2 Other parts of record

  const std::string& head() const;

Returns the first line of the record as it appeared in the input stream.

  const std::vector< std::string >& labels() const;

Returns the labels for current record as a vector of strings.

  const std::string& name() const;
  const std::string& ID() const;

Return the first label. C<ID()> is a deprecated synonym for C<name()>.

=head2 Parameter Accessors

  const std::string& delimiterPattern() const;
  void setDelimiterPattern( const std::string& delimiterPattern );
  const size_t& lineLengthForPrintingSequence() const;
  const std::string& commentMarker() const;

I<delimiterPattern> is discussed in L<"Definitions">. I<lineLengthForPrintingSequence>
is the line length used by C<operator<<> to print the sequence of the current record.
I<commentMarker> is the string which demarcates comment lines. When set to
the empty string, only blank lines are considered comment lines.
The default value is "#".

=head2 Sundry

  static bool delimiterPatternSupported( const std::string& delimiterPattern );

Returns whether I<delimiterPattern> is supported as the pattern for separating labels
in the record head line. Splitting on blocks of space characters, tabs, or either
is currently supported.

  void upcaseSequence();

upcases the characters in the sequence.

=head1 AUTHOR

Paul B. Horton

=cut

#endif // ** end of pod **

