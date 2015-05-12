/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2005.12.19
 *  Last Modified: $Date: 2006/09/18 02:00:38 $
 *
 *  Description: See header file.
 */
#include "ResidueIndexWindow.hh"

namespace cbrc{

ResidueIndexWindow::ResidueIndexWindow( const ConstResidueIndexSequenceBoundaries* const seqBoundariesPtr,
					const size_t& windowStartIdx,
					const size_t& _windowEndIdx // one past last text position.
					)
  : seqBoundariesPtr(seqBoundariesPtr),
    _windowStartPtr(seqBoundariesPtr->startPtr()+windowStartIdx),
    _windowEndIdx(_windowEndIdx){
#if CBRC_OPTIMIZE <= 2
    GDB_ASSERT( _windowEndIdx >= windowStartIdx );
    GDB_ASSERT( seqBoundariesPtr->size() >= _windowEndIdx );
#endif // CBRC_OPTIMIZE <= 2
}


ResidueIndexWindow::ResidueIndexWindow( const ResidueIndexWindow& tw ) // copy constructor.
  : seqBoundariesPtr(tw.seqBoundariesPtr),
    _windowStartPtr(tw._windowStartPtr), 
    _windowEndIdx(tw._windowEndIdx){
#if CBRC_OPTIMIZE <= 2
  GDB_ASSERT( _windowEndIdx >= windowStartIdx() );
  GDB_ASSERT( seqEndIdx() >= _windowEndIdx );
#endif // CBRC_OPTIMIZE <= 2
}


ResidueIndexWindow::ResidueIndexWindow( const ResidueIndexWindow& tw, 
					const int& windowStartRelativeIdx, // index relative to window start
					const size_t& newWindowSize )
  : seqBoundariesPtr(tw.seqBoundariesPtr), 
    _windowStartPtr(tw._windowStartPtr + windowStartRelativeIdx)
{
  _windowEndIdx = _windowStartPtr - seqStartPtr() + newWindowSize;
#if CBRC_OPTIMIZE <= 2
    GDB_ASSERT(  tw.validBoundaries() );
#endif // CBRC_OPTIMIZE <= 2
}


bool ResidueIndexWindow::validRelativeIdx( const int& idx ) const{
  if( idx < -static_cast<int>(windowStartIdx()) ){
    std::cerr << __FILE__ << ":" << __LINE__ << "; Error: : idx = " 
	      << idx << " less than -windowStartIdx() = " 
	      << -static_cast<int>(windowStartIdx()) << std::endl;
    return false;
  }
  if( idx >= static_cast<int>(seqEndIdx())
      - static_cast<int>(windowStartIdx()) ){
    std::cerr << __FILE__ << ":" << __LINE__ << "; Error: : idx =" 
	      << idx << " greater or equal to seqSize()=" 
	      << static_cast<int>(seqEndIdx()) << " minus "
	      << " windowStartIdx()="
	      << static_cast<int>(windowStartIdx()) << std::endl;
    return false;
  }
  return true;
}

} // end namescape cbrc.
