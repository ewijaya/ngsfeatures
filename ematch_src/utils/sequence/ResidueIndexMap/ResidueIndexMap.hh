/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2006, Paul Horton, All rights reserved.
 *  Creation Date: 2003.03.15
 *  Last Modified: $Date: 2009/09/20 10:00:29 $
 *
 *  Purpose: Converter for converting back and forth from char and serial
 *           number representation of residues.
 *
 *  Usage: ResidueIndexMap* rim = new ResidueIndexMap( CBRC_DNA_ALPHABET );
 *         residueIndex r = rim.toResidueIndex( "a" );
 *         char c = rim.toResidue( r ); // returns "a"
 *
 *  Description: Defined one to one mapping between ascii representations of residues and a series
 *               of residue indices. e.g. DNA (a,c,g,t) mapped to (0,1,2,3)
 *
 */
#ifndef _RESIDUEINDEXMAP_HH_
#define _RESIDUEINDEXMAP_HH_
#include <string>
#include <vector>
#include <ctype.h>
#include <stdexcept>
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{


class ResidueIndexMap{
public:
  /* ********** typedefs ********** */
  
  typedef  char                residueT;
  typedef  unsigned char         indexT;
  typedef  FLENumArray<indexT>   arrayT;
  typedef  std::vector<indexT>  vectorT;


  /* *************** Constructors *************** */

  ResidueIndexMap( const std::string& alphabet,
		   const bool& caseSensitive = false ) throw( std::invalid_argument );

  ResidueIndexMap( const std::string& alphabetProper,
		   const std::string& alphabet,
		   const bool& caseSensitive = false ) throw( std::invalid_argument );

  // this explicit version necessary, because C-strings convert to bool before std::string
  ResidueIndexMap( const char* const& alphabetProper,
		   const char* const& alphabet,
		   const bool& caseSensitive = false )
    throw( std::invalid_argument );


  ResidueIndexMap( std::istream& is )
    throw( std::invalid_argument ){
    read( is );
  }


  // null and copy constructor
  ResidueIndexMap() : _caseSensitive(false){}

  ResidueIndexMap( const ResidueIndexMap& rim ) : _alphabetProper(rim._alphabetProper),
						  _alphabet(rim._alphabet),
						  _caseSensitive(rim._caseSensitive){}

  // destructor
  virtual ~ResidueIndexMap(){}


  /* ********** Modifying methods ********** */

  // add residue to end of alphabet, see pod for more
  void extendAlphabet( const char& residue ) throw( std::invalid_argument );


  /* ********** Predicates about residues and residue indices ********** */

  bool validResidue( const char& residue ) const;
  bool validResidues( const std::string& residues ) const;

  bool validResidueIndex( const indexT& residueIndex ) const{
    return( residueIndex < sigma() );
  }

  bool properResidue( const char& residue ) const;
  bool properResidues( const std::string& residues ) const;

  bool properResidueIndex( const indexT& residueIndex ) const{
    return(  residueIndex < alphabetProper().size()  );
  }


  /* ********** residue and residue index converters ********** */

  indexT toResidueIndex( const char& residue ) const throw( std::invalid_argument );

  char toResidue( const indexT& residueIndex ) const throw( std::invalid_argument );

  vectorT toResidueIndicesVector( const std::string& residues ) const
    throw( std::invalid_argument );

  arrayT  toResidueIndices(       const std::string& residues ) const
    throw( std::invalid_argument );

  // sets residueIndices
  void assignResidueIndices( arrayT& residueIndices, const std::string& residues ) const
    throw( std::invalid_argument );

  void assignResidueIndices( vectorT& residueIndicesVec, const std::string& residues ) const
    throw( std::invalid_argument );

  void assignResidueIndices( const arrayT::iterator& residueIndicesBegin,
			     const std::string::const_iterator& residuesBegin,
			     const std::string::const_iterator& residuesEnd ) const
    throw( std::invalid_argument );

  std::string toResidues( const std::string& residueIndices ) const
    throw( std::invalid_argument );

  std::string toResidues( const arrayT& residueIndices ) const
    throw( std::invalid_argument );

  std::string toResidues( const arrayT::const_iterator& residueIndicesBegin,
			  const arrayT::const_iterator& residueIndicesEnd)
    const throw( std::invalid_argument );

  
  std::string
  vectorToResidues( const vectorT& residueIndicesVector ) const throw( std::invalid_argument );


  /* ********** Accessors and related methods ********** */
  const std::string& alphabet()       const{ return _alphabet; }
  
  const std::string& alphabetProper() const{ return _alphabetProper; }

  // "Σ" is commonly used to denote alphabet size in the string algorithm literature
  size_t sigma() const{  return alphabet().size();  } 

  indexT maxIndex() const{ return static_cast<indexT>(alphabet().size()-1); }

  bool caseSensitive() const{  return _caseSensitive;  }


  /* ********** Methods related to handling case ********** */

  // return true if residue0 == residue1  (possibly ignoring case)
  bool residuesAreEquivalent( const char& residue0, const char& residue1 ) const;

  // canonicalize case methods, convert characters to case of first residue
  void canonicalizeCase( std::string& sequence ) const{
    for( size_t i = 0; i < sequence.size(); ++i ) sequence[i] = canonicalizeCase( sequence[i] );
  }

  // Return character RESIDUE, case converted to have the same case as alphabet[0].
  char canonicalizeCase( const char& residue ) const{
    return(   islower( alphabet()[0] ) ? tolower(residue) : toupper(residue)   );
  }


  /* ********** Binary I/O ********** */
  static const std::string& binaryStreamSignature(){
    static const std::string s( "RSIM000\n" );
    return s;
  }

  // should only be called from empty object (created with null constructor)
  // dies on IO error, throws exception is alphabets are invalid.
  void read( std::istream& is ) throw( std::invalid_argument );

  // dies on IO error.
  void write( std::ostream& os ) const;


  /* ********** Other methods ********** */
  std::vector<vectorT> splitIntoProperOnlySubstrings( const vectorT& residueIndicesVector ) const;
private:
  void _assertAlphabetProperIsPrefix()   const throw( std::invalid_argument );
  void _assertAlphabetCharactersUnique() const throw( std::invalid_argument );

  /* ********** Private class methods ********** */
  // used by a sort in _assertAlphabetCharactersUnique
  static bool _caseInsensitiveSortPredicate( const char& c0, const char& c1 );

  // object data
  std::string _alphabetProper;
  std::string _alphabet;
  bool        _caseSensitive;
};


// equality test operator
bool inline operator== ( const ResidueIndexMap& rim1, const ResidueIndexMap& rim2 ){
  if( rim1.alphabetProper() != rim2.alphabetProper() )  return false;
  if( rim1.alphabet()       != rim2.alphabet()       )  return false;
  if( rim1.caseSensitive()  != rim2.caseSensitive()  )  return false;
  return true;
}


/* ********** Functions using ResidueIndexMap ********** */
std::ostream& operator<<( std::ostream& os, const ResidueIndexMap& rim );

}; // end namespace cbrc
#endif // defined _RESIDUEINDEXMAP_HH_
#if 0 // ** start pod **
=pod

=head1 NAME

ResidueIndexMap

=head1 SYNOPSIS

Class for mapping characters (representing residues in biosequences) to serial numbers.

=head1 DESCRIPTION

B<ResidueIndexMap> is a class for the common task of mapping characters to
indices for array access. For example mapping the DNA bases (a,c,g,t) to
(0,1,2,3) etc. The characters are referred to as "residues" and the integers
they map to as "residue indices".

This class only supports a one to one mapping between residues and
integers starting with 0.

A subset of the residues can be designated as representing "proper" residues.
A typical use of this is to distinguish between characters which represent
actually residues and others. Using DNA as an example, {a,c,g,t} would be
proper residues but "n" would not.


=head1 types

  typedef  char                residueT;
  typedef  unsigned char         indexT;
  typedef  FLENumArray<indexT>   arrayT;
  typedef  std::vector<indexT>  vectorT;

A residue index is of type B<indexT>, a sequence of residue indices can be
contained in a B<arrayT> or a B<vectorT>. The type of a residue, B<residueT>
is also implicit in the method signatures which use B<std::string> to represent
a sequence of residues.


=head1 Constructors

  ResidueIndexMap( const std::string& alphabet,
                   const bool& caseSensitive = false ) throw( std::invalid_argument );

  ResidueIndexMap( const std::string& alphabetProper,
                   const std::string& alphabet,
                   const bool& caseSensitive = false ) throw( std::invalid_argument );

  ResidueIndexMap( const char* const& alphabetProper,
                   const char* const& alphabet,
                   const bool& caseSensitive = false ) throw( std::invalid_argument );

The arguments given respect follow two conditions:
1. I<alphabetProper> must be a prefix of I<alphabet>, (it is set to I<alphabet> if omitted).

2. The characters in I<alphabet> must each be unique. The position of each residue
in I<alphabet> is the residue index assigned the residue.

A B<std::invalid_argument> exception is thrown if the arguments violate those conditions.


If constructed with I<caseSensitive> = true, characters, e.g. ("a", "A"), which
differ only in case will be considered equivalent.


The constructors are easily illustrated with an example. The constant
DNAExtendedResidueIndexMap, defined in
L<ResidueIndexMapPredefinedAlphabets>, is constructed by

  ResidueIndexMap( "acgt", "acgtbdhkmnrsvwy" )

Note that first argument is a prefix of the second argument. This
is required. If the second argument is omitted all residues are
considered to be proper.


In addition, null and copy constructors and the following
constructor from a binary stream are provided.

  ResidueIndexMap( std::istream& is ) throw( std::invalid_argument );


=head1 Modifying Methods

  void extendAlphabet( const char& residue ) throw( std::invalid_argument );

append I<residue> to the end of the alphabet. alphabetProper is not changed.
If the addition causes alphabet to violate the conditions given in
L<"Constructors">, a B<std::invalid_argument> exception is thrown.



=head1 Predicates about residues and residue indices


=head2 in range checks for alphabet

  bool validResidue( const char& residue ) const;
  bool validResidues( const std::string& residues ) const;
  bool validResidueIndex( const indexT& residueIndex ) const;

B<validResidue> returns true iff I<residue> is in alphabet.

B<validResidues> returns true iff each residue in I<residues> is in alphabet.

B<validResidueIndex> returns true iff I<residueIndex> is the residue
index of some residue in alphabet.


=head2 in range checks for alphabetProper

  bool properResidue( const char& residue ) const;
  bool properResidues( const std::string& residues ) const;
  bool properResidueIndex( const indexT& residueIndex ) const;

B<properResidue> returns true iff I<residue> is in alphabetProper.

B<properResidues> returns true iff each residue in I<residues> is in alphabetProper.

B<properResidueIndex> returns true iff I<residueIndex> is the residue index of some
residue in alphabetProper.


=head1 Converters

These methods convert residues to residue indices or vice versa. Each
method throws a B<std::invalid_argument> exception if their argument is
out of range.

  indexT toResidueIndex( const char& residue ) const throw( std::invalid_argument );

Return residue index of I<residue>.

  char toResidue( const indexT& residueIndex ) const throw( std::invalid_argument );
  vectorT toResidueIndicesVector( const std::string& residues ) const throw( std::invalid_argument );
  arrayT  toResidueIndices(       const std::string& residues ) const throw( std::invalid_argument );

B<toResidue> returns the residue corresponding to I<residueIndex>.

B<toResidueIndicesVector> returns a B<vectorT> holding the sequence of
residueIndices corresponding to I<residues>.

B<toResidueIndices> returns a B<arrayT> holding the sequence of
residueIndices corresponding to I<residues>.

  void assignResidueIndices( arrayT& residueIndices, const std::string& residues ) const
    throw( std::invalid_argument );

  void assignResidueIndices( const arrayT::iterator& residueIndicesBegin,
			     const std::string::const_iterator& residuesBegin,
			     const std::string::const_iterator& residuesEnd ) const
    throw( std::invalid_argument );

B<assignResidueIndices> is like B<toResidueIndices> except that it assigns the sequence
of residue indices to B<residueIndices>, potentially more efficient by avoiding creating
a temporary copy of the residue indices sequence.

  std::string toResidues( const std::string& residueIndices ) const throw( std::invalid_argument );
  std::string toResidues( const arrayT& residueIndices )      const throw( std::invalid_argument );
  std::string toResidues( const arrayT::const_iterator& residueIndicesBegin,
			  const arrayT::const_iterator& residueIndicesEnd)
    const throw( std::invalid_argument );

  std::string vectorToResidues( const vectorT& residueIndicesVector ) const throw( std::invalid_argument );


B<toResidues> returns a B<std::string> holding the residues corresponding to the residues in
I<residueIndices>.

B<vectorToResidues> is like B<toResidues>, but the residue indices argument I<residueIndicesVector> is of type B<vectorT>.

=head1 Accessors and Related Methods

  const std::string& alphabet()       const;
  const std::string& alphabetProper() const;

Return a reference to their respective alphabet.

  size_t sigma() const;
  indexT maxIndex() const;

B<sigma> returns alphabet().size(). "Σ" is commonly used to denote alphabet size in the string algorithm literature (I prefer using lower case "σ", because the upper case version looks like the summation sign).

B<maxIndex> returns 1 - B<sigma>.

  bool caseSensitive() const;



=head1 Methods Related to Handling Case


  bool residuesAreEquivalent( const char& residue0, const char& residue1 ) const;

Return true iff I<residue0> is equal to I<residue1>, but possibly of different
case if caseSensitive is false.

  void canonicalizeCase( std::string& sequence ) const;
  char canonicalizeCase( const char& residue ) const;

B<canonicalizeCase> converts the residue I<residue> or residues I<sequence> to
have the same case the first characters in alphabet.



=head1 binary I/O

  static const std::string& binaryStreamSignature()

for binary I/O operations, the contents of the object is proceeded by
the bytes returned by B<binaryStreamSignature>, which allows
format version information to be included in the file.

  void read( std::istream& is ) throw( std::invalid_argument );

B<read> throws a std::invalid_argument if the read contents fail the
consistency conditions listed in L<"Converters">.

B<read> reads an B<ResidueIndexMap> object from the istream, throwing
a B<std::invalid_argument> if the read contents fail the
consistency conditions listed in L<"Converters">.

  void write( std::ostream& os ) const;

B<write> the B<ResidueIndexMap> object to the ostream.

The B<read> and B<write> functions print error message and exit upon I/O error,


=head1 Other Methods

  std::vector<vectorT> splitIntoProperOnlySubstrings( const vectorT& residueIndicesVector ) const;

Return a vector of the decomposition of I<residueIndicesVector> into maximal length substrings
whose residues are all in the alphabetProper.

=head2 example

  ResidueIndexMap rim( "acgt", "acgtn" );
  
  rim.splitIntoProperOnlySubstrings( "acctxattgx" ) --> ( "acct", "attg" ).

=head1 Computation Time

Some methods and their computational complexity in terms of alphabet size are given here:

  validResidue( const char& residue )               linear
  properResidue( const char& residue )              linear (alphabetProper)
  validResidueIndex( const indexT& residueIndex )   constant
  properResidueIndex( const indexT& residueIndex )  constant
  toResidueIndex( const char& residue )             constant
  toResidue( const indexT& residueIndex )           linear


It would be easy to implement a constant or log time algorithm for the linear time
methods above. This would probably be desirable for alphabet sizes >> 10, especially
if those methods are found in inner loops.


=head1 AUTHOR

Paul B. Horton

=cut

#endif // ** end of pod **
