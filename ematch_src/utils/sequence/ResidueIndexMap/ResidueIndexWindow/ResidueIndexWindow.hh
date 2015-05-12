/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2005.12.8
 *  Last Modified: $Date: 2007/08/11 08:21:27 $
 *
 *  Description: Represent a window into a sequence. 
 *
 *  Purpose: Originally created for background model used in motif
 *           discovery.
 *
 *  Design Issues: This implementation stores four variables for
 *                 each instance: start and end of both text and window.
 *                 Another idea would be to just store one pointer to a
 *                 text object that could return the start and end.
 *
 */
#ifndef _RESIDUEINDEXWINDOW_HH_
#define _RESIDUEINDEXWINDOW_HH_
#include "utils/gdb/gdbUtils.hh"
#include "utils/FLArray/FLArray.hh"
#include "utils/FLArray/FLEArray.hh"
#include "../ResidueIndexMap.hh"
#include "ConstResidueIndexSequenceBoundaries.hh"

namespace cbrc{

class ResidueIndexSequence;

class ResidueIndexWindow{
  typedef ResidueIndexMap::indexT indexT;
public:
  ResidueIndexWindow( const ConstResidueIndexSequenceBoundaries* const seqBoundariesPtr,
		      const size_t& windowStartIdx,
		      const size_t& _windowEndIdx   // index from text start. One past last position.
		      );
  ResidueIndexWindow( const ResidueIndexWindow& tw ); // copy constructor.
  ResidueIndexWindow( const ResidueIndexWindow& tw, 
		      const int& windowStartRelativeIdx, // index relative to window start
		      const size_t& newWindowSize );

  size_t size() const{ return( _windowEndIdx - windowStartIdx() ); }
  size_t leftContextSize() const{ return windowStartIdx(); }
  size_t rightContextSize() const{ return( seqEndIdx() - _windowEndIdx ); }
  const indexT* const seqStartPtr() const{ return seqBoundariesPtr->startPtr(); }
  const indexT* const windowStartPtr() const{ return _windowStartPtr; }
  const indexT* const windowLastPtr() const{ 
    return( seqBoundariesPtr->startPtr() + windowEndIdx() );
  }
  const size_t seqSize() const{ return seqEndIdx(); }
  const size_t windowStartIdx() const{ 
    return( _windowStartPtr - seqBoundariesPtr->startPtr() );
  }
  const size_t windowEndIdx() const{ return _windowEndIdx; }
  const size_t seqEndIdx() const{ return seqBoundariesPtr->size(); }
  FLEArray<indexT> getFLEArray() const{
    return FLEArray<indexT>( size(), windowStartPtr() );
  }
  bool increment(){ // slide right or return false if already at right boundary.
    if( windowEndIdx() >= seqSize() )  return false;
    ++_windowStartPtr;
    ++_windowEndIdx;
    return true;
  }
  bool decrement(){ // slide left or return false if already at left boundary.
    if( !windowEndIdx() )  return false;
    --_windowStartPtr;
    --_windowEndIdx;
    return true;
  }
  const indexT& operator()( const int& relativeIdx ) const{
#if CBRC_OPTIMIZE <= 2
    assert( validRelativeIdx(relativeIdx) );
#endif // if CBRC_OPTIMIZE <= 2
    return( *(_windowStartPtr + relativeIdx) );
  }
  size_t relativeIdxToSeqIdx( const int& relativeIdx ){
#if CBRC_OPTIMIZE <= 2
    assert( validRelativeIdx(relativeIdx) );
#endif // if CBRC_OPTIMIZE <= 2
    return (size_t) ( (int) windowStartIdx() + relativeIdx );
  }
  bool validRelativeIdx( const int& relativeIdx ) const;
  bool validBoundaries() const{ // self-consistency check
    return( validRelativeIdx( 0 )
	    && validRelativeIdx( windowEndIdx() - windowStartIdx() - 1 ) );
  }
  friend std::ostream& operator<<( std::ostream& os, const ResidueIndexWindow& riw );
private:
  const ConstResidueIndexSequenceBoundaries* seqBoundariesPtr;
  const indexT* _windowStartPtr; // pointer to start of window.
  size_t _windowEndIdx; // index from text start of first position PAST the window
};

inline std::ostream& operator<<( std::ostream& os, const ResidueIndexWindow& riw ){
  for( const ResidueIndexWindow::indexT* r = riw.windowStartPtr();
       r - riw.windowStartPtr() < static_cast<int>(riw.size());
       ++r ){
    os << riw.seqBoundariesPtr->residueIndexMap().toResidue( *r );
  }
  return os;
}

}; // end namespace cbrc
#endif // defined _RESIDUEINDEXWINDOW_HH_
