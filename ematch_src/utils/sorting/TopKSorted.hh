/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.6.3
 *  Last Modified: $Date: 2008/08/11 14:24:03 $
 *
 *  Purpose: Compute the top k items in sorted order for a series of numbers that may be too large
 *           to fit in memory.
 *
 *  Example: To find the top 10 doubles of a large set of numbers using no more than 1000 doubles
 *           worth of memory do the following:
 *
 *           TopKSorted topK<double>( 10, 1000 );
 *           // now topK.getK() == 10
 *           for each double value val{
 *               topK.insertValueIfInTopK( val );
 *           }
 *           double* sortedList = topK.getSortedList();
 *           for( size_t i = 0; i < topK.getK(); ++i ){
 *             std::cout << sortedList[i] << " ";
 *           }
 *
 *  Caveat: insertValueIfInTopK(T) must be called at least k times for getSortedList() to return
 *          a valid list.
 */
#ifndef TOPKSORTED_HH_
#define TOPKSORTED_HH_
#include <assert.h>
#include <algorithm>
#include <functional>
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

template < typename T, typename BinaryPredicate = std::greater_equal<T> >
class TopKSorted {
public:

  /* ********** CONSTRUCTORS ********** */
  TopKSorted( const size_t& k, const size_t& maxStackSize )
  : _k(k), maxStackSize(maxStackSize){
    assert( k > 0 );
    assert( maxStackSize > k );
    _stack.setSize( maxStackSize );
    top = 0;
  }

  TopKSorted( const TopKSorted<T, BinaryPredicate>& topK )
    : _k( topK.k() ),
      maxStackSize(topK.maxStackSize){
    _stack.setSize( maxStackSize );
    top = topK.top;
  }


  ~TopKSorted(){}


  /* ********** ACCESSORS ********** */
  const size_t& k() const{ return _k; }

  const FLEArray<T>& stack() const{ return _stack; }

  const T& operator()( const size_t& i ) const{
    assert( i < k() );
    assert( i < top );
    return( _stack[i] );
  }


  // returns true when VAL cannot possibly in the top k.
  bool cannotBeInTopK( const T& val ) const{

    if( top < k() ){
      return false;  // have not even seen k values yet!
    }

    return cmp( minAcceptableVal, val );
  }


  void insertValueIfInTopK( const T& val ){
    // if < k elements in stack, insert val unconditionally.
    if( top < k() ){
      if( top == 0 ) minAcceptableVal = val;
      if( cmp( minAcceptableVal, val ) )  minAcceptableVal = val;
      _stack[top++] = val;
      return; // ** method exit point **
    }

    // stack already has ≧ k elements
    if( !cmp( val, minAcceptableVal ) ){
      return; // ** method exit point **
    }

    // val is in top k so far.
    if( top < maxStackSize ){
      _stack[top++] = val;
      return; // ** method exit point **
    }

    // assert( top == maxStackSize ), also top >= k.
    // val should be added but stack is full, so sort stack to put the top k items at the front.
    std::nth_element( _stack.begin(), _stack.begin() + k()-1, _stack.begin() + top, cmp );
    minAcceptableVal  =  _stack[ k()-1 ];
    top = k();
    insertValueIfInTopK( val ); /* Achtung! Recursive call. */
  }


  void sort(){ // sort the top k elements.
    std::partial_sort( _stack.begin(),
		       _stack.begin()  +  std::min( k(), top ), _stack.end(),
		       cmp );
  }

  void clear(){  top=0;  }

  TopKSorted<T, BinaryPredicate> operator=( const TopKSorted<T, BinaryPredicate>& topK ){
    TopKSorted<T, BinaryPredicate> retVal( topK );
    return retVal;
  }
private:
  size_t top;
  const size_t _k;
  const size_t maxStackSize;
  FLEArray<T> _stack;
  T minAcceptableVal;
  BinaryPredicate cmp;
};



}; // end namespace cbrc

#endif // defined TOPKSORTED_HH_
#if 0  // skip pod
=pod

=head1 NAME

TopKSorted


=head1 SYNOPSIS

Datastructure offering heap-like functionality.

=head1 DESCRIPTION


TopKSorted receives values and stores as necessary such
that the best I<k> values seen so far can be retrieved
in sorted order.

In other words TopKSorted is basically a heap, although it is
implemented with C<std::partial_sort>, rather than a tree-based heap
data structure.


=head1 TEMPLATE ARGUMENTS

=over

=item T

Type of data stored.

=item BinaryPredicate

Comparison functor used to sort type I<T> items.

=back


=head1 CONSTRUCTORS

  TopKSorted( const size_t& k, const size_t& maxStackSize )

Construct object which keeps top I<k> items, using space
proportional to I<maxStackSize>, C<k E<lt> maxStackSize>.

  TopKSorted( const TopKSorted<T, BinaryPredicate>& topK )

Copy Constructor.


=head1 ACCESSORS

  const size_t&      k()     const;

Does what you expect.

  const FLEArray<T>& stack() const;
  const T& operator()( const size_t& i ) const;

These access the size I<maxStackSize> stack,
guaranteed to hold the top I<k> values;
not necessarily sorted at beginning of the stack
unless after a call to L<sort> without intervening
calls to L<insertValueIfInTopK>.


=head1 METHODS

  void insertValueIfInTopK( const T& val );

Insert value I<val> if it is good enough that
it might be in the top I<k>.

  bool cannotBeInTopK( const T& val ) const;

Return false if, based on values already seen, I<val>
cannot be one of the top I<k> values.

  void sort();

Sort stack.

  void clear();

Clear stack. Values seen after invokation of
C<clear()> are completely forgotten.


=head1 COPYRIGHT

Copyright (C) 2008, Paul B. Horton, All rights reserved.


=head1 AUTHOR

Paul B. Horton

=cut
#endif // skip pod
