/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.5.20
 *  Last Modified: $Date: 2006/05/22 23:42:27 $
 *  Description: See header file.
 */
#include "TrivialHistogram.hh"


namespace cbrc{

  template <typename T> void TrivialHistogram<T>::printNonzeroEntries( 
       std::ostream& os,
       const char fieldSepartor,
       const char entrySeparator ) const{
  bool firstOutput = true;
  for( size_t i = 0; i < count.size(); ++i ){
    if( count[i] ){
      if( firstOutput )  firstOutput = false;
      else               os << entrySeparator;
      os << i << fieldSepartor << count[i];
    }
  }
  os << std::endl;
}

template class TrivialHistogram<size_t>;

}; // end namespace cbrc

