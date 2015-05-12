/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, Japan
 *  Copyright: All rights reserved
 *  Creation Date: : 2006.7.26
 *  Last Modified: $Date: 2008/02/24 02:26:34 $
 *  Description: See header file.
 */
#include "BitMatrix.hh"

namespace cbrc{

  void BitMatrix::writeAsPbm( std::ostream& os ){
    os << "P4 " << size0() << " " << size1() << std::endl;
    os.write( (const char*) m, sizeInBytes() );
  }

} // end namespace cbrc

