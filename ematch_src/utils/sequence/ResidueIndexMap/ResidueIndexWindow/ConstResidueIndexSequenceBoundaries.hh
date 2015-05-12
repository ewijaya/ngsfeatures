/*  
 *  Author: Please set Paul B. Horton environment variable
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.17
 *  Last Modified: $Date: 2008/04/14 12:11:29 $
 *
 *  Description: Pointer into sequence for use with ResidueIndexWindow.
 *               Includes information to access residues and determine
 *               sequence boundaries.
 *
 *  Purpose: Created for use with a background model used for DNA motif discovery.
 *
 */
#ifndef _CONSTRESIDUEINDEXSEQUENCEBOUNDARIES_HH_
#define _CONSTRESIDUEINDEXSEQUENCEBOUNDARIES_HH_
#include "../ResidueIndexMap.hh"

namespace cbrc{

class ConstResidueIndexSequenceBoundaries{
public:
  /* ********** CONSTRUCTORS ********** */
  ConstResidueIndexSequenceBoundaries( const ResidueIndexMap& rim,
				       const ResidueIndexMap::indexT* const& _startPtr,
				       const size_t& _size ) :
    rim(rim), _startPtr(_startPtr), _size(_size){}

  ConstResidueIndexSequenceBoundaries( const ConstResidueIndexSequenceBoundaries& sb ) :
    rim(sb.rim), _startPtr(sb._startPtr), _size(sb._size){}

  /* ********** ACCESSORS ********** */
  const ResidueIndexMap& residueIndexMap() const{ return rim; }

  const ResidueIndexMap::indexT* startPtr() const{ return _startPtr; }

  const ResidueIndexMap::indexT* endPtr()   const{ return _startPtr + size(); }

  const size_t size() const{ return _size; }

private:
  // object data
  const ResidueIndexMap& rim;
  const ResidueIndexMap::indexT* const _startPtr;
  const size_t _size;
};


}; // end namespace cbrc
#endif // defined _CONSTRESIDUEINDEXSEQUENCEBOUNDARIES_HH_

