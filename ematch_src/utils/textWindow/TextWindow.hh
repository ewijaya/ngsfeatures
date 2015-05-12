/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: 2005.12.8
 *  Last Modified: $Date: 2005/12/19 12:54:15 $
 *
 *  Description: Represent a window into a "text". I use the word text here
 *               because this class was created for use with DNA sequences.
 *               Window here is synonymous with substring.
 *               However the template array T does not need to be a character
 *               type so in some instances the term "text" might be quite unnatural.
 *
 *  Purpose: Originally created for background model used in motif
 *           discovery. But later a specialized class ResidueIndexWindow
 *           was made for that.
 *
 *  Design Issues: This implementation stores four variables for
 *                 each instance: start and end of both text and window.
 *                 Another idea would be to just store one pointer to a
 *                 text object that could return the start and end.
 *
 *  Caveat: <T> should a fixed length type. Note that it might be okay for
 *          T to contain pointers to variable length data, for example std::string,
 *          If the size of the basic struct is fixed (I am not sure if std::string
 *          guarantees this or not...)
 *
 *
 *
 */
#ifndef _TEXTWINDOW_HH_
#define _TEXTWINDOW_HH_
#include "utils/gdb/gdbUtils.hh"
#include "utils/FLArray/FLArray.hh"
#include "utils/FLArray/FLEArray.hh"

namespace cbrc{

template <typename T>
class TextWindow{
public:
  TextWindow( const T* const textPtr,
	      const size_t windowStartIdx, // index from text start.
	      const size_t windowEndIdx,   // index from text start. One past last position.
	      const size_t& textEndIdx     // index from text start.
	      )
    : _textPtr(textPtr),  _windowPtr(textPtr+windowStartIdx), _windowEndIdx(windowEndIdx), textEndIdx(textEndIdx) {
#if CBRC_OPTIMIZE <= 2
    GDB_ASSERT( windowEndIdx >= windowStartIdx );
    GDB_ASSERT( textEndIdx >= windowEndIdx );
#endif // CBRC_OPTIMIZE <= 2
  }
  TextWindow( const TextWindow& tw ) // copy constructor.
    : _textPtr(tw._textPtr), 
      _windowPtr(tw._windowPtr), 
      _windowEndIdx(tw._windowEndIdx),
      textEndIdx(tw.textEndIdx){
#if CBRC_OPTIMIZE <= 2
    GDB_ASSERT( _windowEndIdx >= windowStartIdx() );
    GDB_ASSERT( textEndIdx >= _windowEndIdx );
#endif // CBRC_OPTIMIZE <= 2
  }
  TextWindow( const TextWindow& tw, 
	      const int& windowStartRelativeIdx, // index relative to window start
	      const size_t& newWindowSize )
    : _textPtr(tw._textPtr),
      _windowPtr(tw._windowPtr + windowStartRelativeIdx),
      _windowEndIdx(_windowPtr - _textPtr + newWindowSize ),
      textEndIdx(tw.textEndIdx){
#if CBRC_OPTIMIZE <= 2
    GDB_ASSERT(  validRelativeIdx( windowStartRelativeIdx )  );
    GDB_ASSERT(  validRelativeIdx( windowStartRelativeIdx + size() - 1 )  );
#endif // CBRC_OPTIMIZE <= 2
  }
  size_t size() const{ return( _windowEndIdx - windowStartIdx() ); }
  size_t leftContextSize() const{ return windowStartIdx(); }
  size_t rightContextSize() const{ return( textEndIdx - _windowEndIdx ); }
  const T* const& textRef() const{ return _textPtr; }
  const T* const windowPtr() const{ return _windowPtr; }
  const T* const windowLastPtr() const{ return( _windowPtr+size() ); }
  const size_t textSize() const{ return textEndIdx; }
  const size_t windowStartIdx() const{ return( _windowPtr - _textPtr ); }
  const size_t windowEndIdx() const{ return _windowEndIdx; }
  FLArray<T> getFLArray() const{
    return FLArray<T>( size(), _windowPtr );
  }
  // Achtung! No check to ensure T is suitable to put into an FLEArray object.
  FLEArray<T> getFLEArray() const{
    return FLEArray<T>( size(), _windowPtr );
  }
  void operator++(){ // slide right.
    assert( windowEndIdx() < textSize() );
    ++_windowPtr;
    ++_windowEndIdx;
  }
  void operator--(){ // slide left.
    assert( windowStartIdx() );
    --_windowPtr;
    --_windowEndIdx;
  }
  const T& operator()( int idx ) const{
#ifdef CBRC_DEBUG
    assert( validRelativeIdx(idx) );
#endif // defined CBRC_DEBUG
    return *(_windowPtr+idx);
  }
  bool validRelativeIdx( int idx ){
    if( idx < -static_cast<int>(windowStartIdx()) ){
      std::cerr << __FILE__ << ":" << __LINE__ << "; Error: : idx = " 
		<< idx << " less than -windowStartIdx() = " 
		<< -static_cast<int>(windowStartIdx()) << std::endl;
      return false;
    }
    if( idx >= static_cast<int>(textSize())
	- static_cast<int>(windowStartIdx()) ){
      std::cerr << __FILE__ << ":" << __LINE__ << "; Error: : idx =" 
		<< idx << " greater than testSize()=" 
		<< static_cast<int>(textSize()) << " minus "
		<< " windowStartIdx()="
		<< static_cast<int>(windowStartIdx()) << std::endl;
      return false;
    }
    return true;
  }
private:
  const T* const _textPtr; // pointer to start of text.
  const T* _windowPtr; // pointer to start of window.
  size_t _windowEndIdx; // index from text start of first position PAST the window
  const size_t& textEndIdx;
};

}; // end namespace cbrc
#endif // defined _TEXTWINDOW_HH_
