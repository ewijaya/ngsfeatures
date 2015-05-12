/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003, 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.3
 *  Last Modified: $Date: 2009/04/26 08:17:25 $
 *
 *  Description: Square Matrix of elemT type values with string labels.
 *               The ith label is for the ith row and ith column.
 *
 *               elemT should be a numerical type such as float or int
 *
 *  Specialization: The class ConstantLabeledSquareDoubleFLENumMatrix is a
 *                  specialization of this class holding doubles
 *
 *  Applications: Currently used to hold confusion matrices for WoLF
 *
 *  to consider: * Making class hierarchy such as:
 *               FLENumMatrx --> SquareFLENumMatrx --> LabeledSquareFLENumMatrx
 *
 *               * consolidating this class with ConstantLabeledSquareDoubleFLENumMatrix.
 */
#ifndef _LABELEDSQUAREFLNUMMATRIX_HH_
#define _LABELEDSQUAREFLNUMMATRIX_HH_
#include <vector>
#include <string>
#include "boost/foreach.hpp"
#include "utils/gdb/gdbUtils.hh"
#include "utils/FLArray/FLEMatrix.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings.hh"

namespace cbrc{

template <typename elemT>

class LabeledSquareFLENumMatrix{
public:
  LabeledSquareFLENumMatrix( const SerialNumbersForStrings<size_t>& labels )
    : _labels(labels)
  { 
    m.setSize( labels.size(), labels.size() );
  }


  /* ***** accessors ***** */
  elemT& operator() ( const size_t& index0, const size_t& index1 ) const{
    return m( index0, index1 );
  }

  elemT& operator() ( const std::string& label0, const std::string& label1 ) const{
    validateLabels( label0, label1 );
    return (*this)(  toIndex( label0 ), toIndex( label1 )  );
  }

  const std::string& label( const size_t& i ) const{ return labels()(i); }

  const size_t& size() const{ return m.size0(); }


  /* ***** other methods ***** */
  void zero(){ m.zero(); }

  // reorder labels. the defaultValue is assigned to any cells involving labels found in
  // newOrderLabels but not in the old labels.
  void reorderLabels( const std::vector<std::string>& newOrderLabels,
		      const elemT defaultValue = 0 );

private:
  const SerialNumbersForStrings<size_t>& labels() const{  return _labels;  }

  // assumes S is a valid label.
  size_t toIndex( const std::string& s ) const {  return labels()(s);  }

  void validateLabels( const std::string& label0, const std::string& label1 ) const;

  // object data.
  SerialNumbersForStrings<size_t> _labels;
  FLENumMatrix<elemT> m;
};



/* ********** Method definitions ********** */

template <typename elemT>
void LabeledSquareFLENumMatrix<elemT>::validateLabels( const std::string& label0,
						       const std::string& label1 ) const{
  GDB_ASSERTF(  labels().has( label0 ),
	        "index %s does not exist", label0.c_str()  );

  GDB_ASSERTF(  labels().has( label1 ),
	        "index %s does not exist", label1.c_str()  );

} // end validateClassLabels



/* labelsNewOrder should be a permutation of a subset of the labels.
 * the rows and column are reordered such that the labels contained in labelsNewOrder come first.
 * if there are labels not contained in labelsNewOrder they will remain accessible, but their
 * indices will start with lablesNewOrder.size()
 */
template <typename elemT> void
LabeledSquareFLENumMatrix<elemT>::reorderLabels( const std::vector<std::string>& labelsNewOrder,
						 const elemT defaultValue ){
  SerialNumbersForStrings<size_t> newLabels( labelsNewOrder );

  // If labelsNewOrder is a proper subset of labels, 
  //   add the missing labels to the end of newLabels.
  for(  SerialNumbersForStrings<size_t>::const_iterator labelPtr = labels().begin();
        labelPtr != labels().end();  ++labelPtr  ){
    newLabels.getSerialNumber( *labelPtr );
  }


  /* construct temporary matrix and copy object data
   * into it in new label order
   */
  FLENumMatrix<elemT> newM;
  newM.setSize( newLabels.size(), newLabels.size() );

  for( size_t i = 0; i < newLabels.size(); ++i ){
    for( size_t j = 0; j < newLabels.size(); ++j ){
      if(  (labels().has( newLabels(i) )) && (labels().has( newLabels(j) )) ){
	newM( i, j ) = (*this)( newLabels(i), newLabels(j) );
      }else{
	newM( i, j ) = defaultValue;
      }
    }
  }

  // copy reordered matrix to this->m
  m.resizeDestructive( newM.size0(), newM.size1() );
  m = newM;
  _labels = newLabels;
}


template <typename elemT>
std::ostream& operator<<( std::ostream &os, const LabeledSquareFLENumMatrix<elemT>& mat ){
  if( mat.size() == 0 ) return os; // empty matrix.
  
  size_t maxLabelLen = mat.label(0).size();
  for( size_t j = 1; j < mat.size(); ++j ){
    if( mat.label(j).size() > maxLabelLen ) maxLabelLen = mat.label(j).size();
  }
  const std::string diagonalMarker( "*" );
  os << diagonalMarker;
  for( size_t i = diagonalMarker.size(); i < maxLabelLen; ++i ) os << " ";
  os << " " << mat.label(0);
  for( size_t i = 1; i < mat.size(); ++i ){
    os << " " << mat.label(i);
  }
  os << std::endl;
  for( size_t i = 0; i < mat.size(); ++i ){
    os << mat.label(i) << " ";
    for( size_t j = mat.label(i).size(); j < maxLabelLen; ++j ) os << " ";
    os << mat( i, 0 );
    for( size_t j = 1; j < mat.size(); ++j ){
      os << " " << mat(i,j);
    }
    os << std::endl;
  }
  return os;
}

}; // end namespace
#endif // defined _LABELEDSQUAREFLNUMMATRIX_HH_
