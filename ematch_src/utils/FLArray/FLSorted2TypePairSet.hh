/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004-2007 Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.9.10
 *  Last Modified: $Date: 2009/08/25 21:13:04 $
 *  
 *  Description: Pair Set class (maps ordered pairs to bool) implemented with FLSorted class.
 *               The name "2Type" indicates that the ordered pairs can be of different types.
 *
 *  Purpose: Originally made for WoLF
 *
 *  Implementation:
 *      Terminology: See pod below.
 *
 *      DataStructure
 *          lhsToIndex: maps items from the lhs set to a serial number index
 *          lhsIndexToRhsSorted: holds the rhs of each lhs item, indexed with
 *                               the serial numbers held in lhsToIndex.
 *
 */

#ifndef _FLSORTEDPAIRSET_HH_
#define _FLSORTEDPAIRSET_HH_
#include <fstream>
#include <iostream> // temp
#include "utils/perlish/perlish.hh"
#include "utils/FLArray/FLSorted.hh"

namespace cbrc{



template <typename T0, typename T1>
class FLSorted2TypePairSet{
public:
  friend void testFLSorted2TypePairSet();

  /* ********** Constructors ********** */
  FLSorted2TypePairSet( std::ifstream& ifs, const std::string& delimiter="\t" ){
    readFromTextFile( ifs, delimiter );
  }

  FLSorted2TypePairSet(){}

  // The order of initialLhsList and correspondingRightHandSideList MUST coincide
  FLSorted2TypePairSet( const std::vector<T0>& initialLhsList,
			const std::vector< std::vector<T1> >& correspondingRightHandSideList )
    : lhsToIndex(initialLhsList)
  {

    // *************** Reality check args ***************
    if( lhsToIndex.size() != initialLhsList.size() ){
      std::cout << __FILE__ << ":" << __LINE__ 
		<< " Error: leftHandSideList passed to FLSorted2TypePairSet constructor has duplicate entries\n";
#ifdef GDB_DEBUG
      for(;;);
      exit( -1 );
#endif
    }

    if( lhsToIndex.size() != correspondingRightHandSideList.size() ){
      std::cout << __FILE__ << ":" << __LINE__ 
		<< " Error: FLSorted2TypePairSet constructor; Size of leftHandSideList = "
		<< lhsToIndex.size()
		<< " not equal to size of correspondingRightHandSideList = "
		<< correspondingRightHandSideList.size()
		<< std::endl;
#ifdef GDB_DEBUG
      for(;;);
#endif
      exit( -1 );
    }

    // build lhsIndexToRhsSorted data structure.
    _initLhsIndexToRhsSorted( initialLhsList, correspondingRightHandSideList );
  }

  /* **************************************************
   * Construct from text file. Format of text file is
   * key1  value  [value]...
   * key2  value  [value]...
   *
   * The fields should be tab separated.
   * Each key should appear as a key in only one line
   * ************************************************** */
  void readFromTextFile( std::ifstream& ifs, const std::string& delimiter="\t" );

  bool operator() ( const T0& lhs, const T1& rhs ) const{
    if( !lhsToIndex.has(lhs) ) return false;
    size_t rhIndex = lhsToIndex( lhs );
    return lhsIndexToRhsSorted[rhIndex].has( rhs );
  }


  // data accessed by operator<<, would prefer to declare as private if possible.
  FLSorted<T0> lhsToIndex;
  FLArray< FLSorted<T1> > lhsIndexToRhsSorted;
private:
  void _initLhsIndexToRhsSorted( const std::vector<T0>& initialLhsList,
				 const std::vector< std::vector<T1> >& correspondingRightHandSideList ){

    lhsIndexToRhsSorted.setSize( lhsToIndex.size() );
    for( size_t i = 0; i < lhsToIndex.size(); ++i ){

      // the indices of initialLhsList and correspondingRightHandSideList should be in sync.
      size_t indexAfterSorting = lhsToIndex( initialLhsList[i] );

      lhsIndexToRhsSorted[indexAfterSorting].assign( correspondingRightHandSideList[i].begin(),
						     correspondingRightHandSideList[i].end() );
    }
  }
}; // end class FLSorted2TypePairSet


template <typename T0, typename T1>
inline void FLSorted2TypePairSet<T0, T1>::readFromTextFile( std::ifstream& ifs,
							    const std::string& delimiter ){
  if( lhsToIndex.size() ){
    std::cout << __FILE__ << ":" << __LINE__ 
	      << " Programming Error: FLSorted2TypePairSet, expected pair set to be empty when calling readFromTextFile\n";
    exit( -1 );
  }

  std::string line;
  {
    std::vector<T0> lhsVector;
    while(  (line = perlish::slurpLine( ifs )).size()  ){
      lhsVector.push_back( perlish::split( delimiter, line )[0] );
    }

    lhsToIndex = lhsVector;
    
    GDB_ASSERTF(  lhsToIndex.size() == lhsVector.size(),
		  "lhsToIndex.size() = %zu, ≠ lhsVector.size() = %zu\n"
		  "Perhaps an lhs term (first field in line) was duplicated?",
		  lhsToIndex.size(), lhsVector.size()  );

  }
  lhsIndexToRhsSorted.setSize( lhsToIndex.size() );
  ifs.clear(); ifs.seekg( 0, std::ios::beg );
  while(  (line = perlish::slurpLine( ifs )).size()  ){
    std::vector<std::string> fields = perlish::split( delimiter, line );
    T0 key = fields[0];

    assert(  lhsToIndex.has( key )  );
    size_t rhIndex = lhsToIndex( key );
    lhsIndexToRhsSorted[rhIndex].assign( fields.begin()+1, fields.end() );
  }
} // end readFromTextFile



template <typename T0, typename T1>
inline std::ostream& operator<<( std::ostream& os, const FLSorted2TypePairSet<T0,T1>& pairSet ){
  std::cout << "keys: ";
  for( size_t i = 0; i < pairSet.lhsToIndex.size(); ++i ) std::cout << " " << pairSet.lhsToIndex[i];
  std::cout << std::endl;
  std::cout << "values:\n";
  for( size_t i = 0; i < pairSet.lhsToIndex.size(); ++i ){
    for( size_t j = 0; j < pairSet.lhsIndexToRhsSorted[i].size(); ++j ){
      std::cout << "  |" 
		<< pairSet.lhsToIndex[i] 
		<< "| |" 
		<< pairSet.lhsIndexToRhsSorted[i][j] 
		<< "| " 
		<< pairSet( pairSet.lhsToIndex[i], pairSet.lhsIndexToRhsSorted[i][j] ) 
		<< std::endl;
    }
  }
  return os;
} // end operator<<

} // end namespace cbrc

#if 0 // ** start pod **
=pod

=head1 NAME

FLSorted2TypePairSet

=head1 SYNOPSIS

Template class, implementing a set of ordered pairs.

=head1 DESCRIPTION

B<FLSorted2TypePairSet><T0,T1> represents a set of ordered pairs. Each
ordered pair has a left hand side of type I<T0> and a right hand side
of type I<T1>.

B<FLSorted2TypePairSet> is intended for sets of ordered pairs which
are compiled in advance, rather than incrementally added to. Currently
no methods are provided for incremental addition and even if they
are added in the future, they will not be fast. See L<"implementation">.

=head1 IMPLEMENTATION

B<FLSorted2TypePairSet> is implemented with sorted lists which are
very memory efficient and reasonably fast for access, but probably slower
than a good hash implementation. Sorted lists are not well suited
for incremental addition of data.

=head1 TERMINOLOGY

Defining a few terms here will simplify the description and make the method
arguments easier to understand.

=over 4

=item left hand side, or "lhs"

The item appearing in the left hand side of an order pair. The
left hand side of (a, b) is "a".

=item right hand side, or "rhs"

The item appearing in the right hand side of an order pair. The
right hand side of (a, b) is "b".

=item leftHandSideList

I<leftHandSideList> is a list (without duplicates) of all the items
appearing in the left hand side of an ordered pair.

=item correspondingRightHandSideList

I<correspondingRightHandSideList> is a list in the I<same> order
as I<leftHandSideList>. Each element of I<correspondingRightHandSideList>
is itself a list, containing (without duplicates) all of the
right hand side items of pairs with the given left hand side.

=back

Together, leftHandSideList[i] and correspondingRightHandSideList[i]
contain the information for all ordered pairs starting with the
item "leftHandSideList[i]".

=head2 Example

Given set { (a,b), (a,c), (b,c) }:

    The leftHandSideList of lhs is (a,b)

    The correspondingRightHandSideList of "a" is:
            correspondingRightHandSideList[0] -> (b,c)

    The correspondingRightHandSideList of "b" is:
            correspondingRightHandSideList[1] -> (c)

=head1 CONSTRUCTORS

In addition to the null constructor, the following constructors
are provided:

=head2 constructor from std::vector

 Name:  FLSorted2TypePairSet<T0,T1>

 Args:  const std::vector<T0>& leftHandSideList
        const std::vector< std::vector<T1> >& correspondingRightHandSideList

The indices of leftHandSideList must be in sync with correspondingRightHandSideList.

=head2 constructor from std:ifstream

 Name:  FLSorted2TypePairSet<T0,T1>

 Args:  std::ifstream& ifs
	const std::string delimiter="\t"

Read ordered pairs data in from text file.

=head2 readFromTextFile

  void readFromTextFile( std::ifstream& ifs, const std::string& delimiter="\t" );

Read data in from text stream. This method is a pseudo-constructor, which can
only be called for empty objects (e.g. objects created with the null constructor).

=head1 INPUT TEXT FORMAT

By default, the input file is in tab separated fields:

    lhs rhs [rhs]...
    lhs rhs [rhs]...

The first field is a left hand side item. The second and
following fields are corresponding right hand side items.
Thus {(a,b), (a,c), (b,c)} could be represented as:

    a b c
    b c

Each lhs key should appear on only one line.
Argument I<delimiter> can be used to change the field separator
from tab to something else.


=head1 METHODS

=head2 operator()

  Decl: bool operator() ( const T0& lhs, const T1& rhs ) const

  Args: const T0& lhs
        const T1& rhs

Returns true iff the ordered pair (I<lhs>, I<rhs>) are contained
in the set.


=head1 I/O

=head2 operator<< for printing

     std::ostream& operator<<( std::ostream& os, const FLSorted2TypePairSet<T0,T1>& pairSet );


=head1 HISTORY

  created: 2004.9.10
  last Modified: $Date: 2009/08/25 21:13:04 $

=head1 AUTHOR

Paul B. Horton

=cut

#endif // ** end of pod **
#endif // defined _FLSORTEDPAIRSET_HH_
