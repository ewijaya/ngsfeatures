/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.3
 *  Last Modified: $Date: 2009/04/26 07:28:46 $
 *  Description: See header file.
 */
#include "utils/FLArray/ConstantLabeledSquareDoubleFLENumMatrix.hh"
#include "utils/perlish/perlish.hh"

namespace cbrc{

void ConstantLabeledSquareDoubleFLENumMatrix::setFromStream( std::istream& is,
							    const std::string inputDelimiter ){
  std::string format( "first line is: label0 label1 ... labelN\n"
		      "next line contains the vector for label0 separated by spaces"
		      "following line contains the vector for label1 separated by spaces"
		      "...\n"
		      "empty lines and lines starting with \"#\" are ignored" );
  std::string line = perlish::slurpLine( is, perlish::isSharpCommentOrEmpty );
  std::vector< std::string > fields = perlish::split( inputDelimiter, line );

  if( fields.size() < 2 ){
    std::cout << __FILE__ << ":" << __LINE__ << " Error: ";
    std::cout << " not enough fields in first line. Format is:\n" << format << std::endl;
    assert( 0 );
  }

  for( size_t i = 0; i < fields.size(); ++i ){
    GDB_ASSERTF(  !labels().has( fields[i] ),
		  "the field name \"%s\" appears to be duplicated", fields[i].c_str()  );
    _labels.getSerialNumber( fields[i] );
  }

  m.setSize( labels().size(), labels().size() );

  for( size_t i = 0; i < labels().size(); ++i ){
    line = perlish::slurpLine( is, perlish::isSharpCommentOrEmpty );
    fields = perlish::split( inputDelimiter, line );
    if( fields.size() != labels().size() ){
      std::cout << " not enough fields in line: " << line << "\nFormat is:\n" << format << std::endl;
      assert( 0 );
    }
    for( size_t j = 0; j < fields.size(); ++j )  m( i, j ) = strtod( fields[j].c_str(), NULL );
  }
}


/* labelsNewOrder should be a permutation of a subset of the labels.
 * the rows and column are reordered such that the labels contained in labelsNewOrder come first.
 * if there are labels not contained in labelsNewOrder they will remain accessible, but their
 * indices will start with labelsNewOrder.size()
 */
void ConstantLabeledSquareDoubleFLENumMatrix
::renumLabels( const SerialNumbersForStrings<size_t>& labelsNewOrder ){

  /* ***** Construct new SerialNumbersForStrings for labels ***** */
  SerialNumbersForStrings<size_t> newLabels( labelsNewOrder );

  // add labels in this->labels, to end if not already present in labelsNewOrder
  for( size_t i = 0; i < labels().size(); ++i ){
    newLabels.getSerialNumber( labels()(i) );
  }

  // die if labelsNewOrder was not a subset of this->labels
  if( labels().size() != newLabels.size() ){
    _renumLabels_die( newLabels );
  }


  /* ***** transfer elements to the a new matrix with the new ordering of labels ***** */
  FLENumMatrixFast<double> newM( m.size0(), m.size1() );

  for( size_t i = 0; i < newLabels.size(); ++i ){
    for( size_t j = 0; j < newLabels.size(); ++j ){
      newM( i, j ) = (*this)( newLabels(i), newLabels(j) );
    }
  }

  // copy new matrix to this object data
  m = newM;
  _labels = newLabels;
} // end renumLabels



/* die with error message.
 * Called by renumLabels when newLabels contains labels not found in this->labels
 */
void ConstantLabeledSquareDoubleFLENumMatrix
::_renumLabels_die( const SerialNumbersForStrings<size_t>& newLabels ) const{
    std::cout << __FILE__ << ":" << __LINE__ << " Error: labels().size(): " << labels().size() << " != ";
    std::cout << " newLabels.size(): " << newLabels.size() << std::endl;
    std::cout << "Array labels: (* only present in array labels)" << std::endl;
    for( size_t j = 0; j < labels().size(); ++j ){
      if( j ) std::cout << " ";
      std::cout << labels()(j);
      bool unique = true;
      for( size_t k = 0; k < newLabels.size(); ++k ){
	if( labels()(j) == newLabels(k) ) unique = false;
      }
      if( unique )  std::cout << "*";
    }
    std::cout << std::endl;
    std::cout << "Array newLabels: (* only present in newLabels)" << std::endl;
    for( size_t j = 0; j < newLabels.size(); ++j ){
      if( j ) std::cout << " ";
      std::cout << newLabels(j);
      bool unique = true;
      for( size_t k = 0; k < labels().size(); ++k ){
	if( newLabels(j) == labels()(k) ) unique = false;
      }
      if( unique )  std::cout << "*";
    }
    std::cout << std::endl;
    exit( -1 );
} // end _renumLabels_die



bool ConstantLabeledSquareDoubleFLENumMatrix::symmetric() const{
  for( size_t i = 0; i < size0(); ++i ){
    for( size_t j = 0; j < i; ++j ){
      if( this->operator()( i, j ) != this->operator()( j, i ) ){
	return false;
      }
    }
  }
  return true;
}


void
ConstantLabeledSquareDoubleFLENumMatrix::validateLabels( const std::string& label0,
							 const std::string& label1 ) const{
  GDB_ASSERTF(  labels().has( label0 ),
	        "index %s does not exist", label0.c_str()  );

  GDB_ASSERTF(  labels().has( label1 ),
	        "index %s does not exist", label1.c_str()  );

} // end validateLabels



std::ostream& operator<<( std::ostream &os, const ConstantLabeledSquareDoubleFLENumMatrix& mat ){

  if( mat.labels().size() == 0 ) return os; // empty matrix.

  std::cout << mat.outputDelimiter() << mat.labels()(0);
  for( size_t i = 1; i < mat.labels().size(); ++i ){
    std::cout << mat.outputDelimiter() << mat.labels()(i);
  }
  std::cout << std::endl;

  for( size_t i = 0; i < mat.size0(); ++i ){
    std::cout << mat.labels()(i) << mat.outputDelimiter();
    std::cout << mat.m( i, 0 );
    for( size_t j = 1; j < mat.size0(); ++j ){
      std::cout << mat.outputDelimiter() << mat.m(i,j);
    }
    std::cout << std::endl;
  }
  return os;
}

}; // end namespace
