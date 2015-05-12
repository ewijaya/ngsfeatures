/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003-2007, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.12.3
 *  Last Modified: $Date: 2008/07/07 07:24:41 $
 *
 *  Description: Square Matrix of double values with string labels.
 *               The ith label is for the ith row and ith column.
 *               Specialization of LabeledSquareFLENumMatrix.
 *
 *  Applications: used for utility matrix of WoLF
 *
 */
#ifndef _CONSTANTLABELEDSQUAREDOUBLEFLNUMMATRIX_HH_
#define _CONSTANTLABELEDSQUAREDOUBLEFLNUMMATRIX_HH_
#include <vector>
#include <string>
#include "utils/gdb/gdbUtils.hh"
#include "utils/FLArray/FLEMatrix.hh"
#include "utils/serialNumbers/SerialNumbersForStrings/SerialNumbersForStrings.hh"

namespace cbrc{

class ConstantLabeledSquareDoubleInterface{
public:
  virtual double operator()( const size_t& index0, const size_t& index1 ) const = 0;
  virtual double operator()( const std::string& label0, const std::string& label1 ) const = 0;
  virtual void renumLabels( const SerialNumbersForStrings<size_t>& labelsNewOrder ) = 0;
  virtual ~ConstantLabeledSquareDoubleInterface(){}
};


class ConstantLabeledSquareDoubleFLENumMatrix : public ConstantLabeledSquareDoubleInterface{
public:
  ConstantLabeledSquareDoubleFLENumMatrix( std::istream& is ){ 
    setFromStream( is ); }

  ConstantLabeledSquareDoubleFLENumMatrix( std::istream& is, const std::string inputDelimiter ){ 
    setFromStream( is, inputDelimiter ); }

  ConstantLabeledSquareDoubleFLENumMatrix( const ConstantLabeledSquareDoubleFLENumMatrix& matrixToCopy )
    : _labels( matrixToCopy.labels() ), m( matrixToCopy.m )
  {}

  void setFromStream( std::istream& is, const std::string inputDelimiter = "[ \t]+" );


  /* ***** accessors ***** */
  static const std::string outputDelimiter(){ return std::string( "\t" ); }

  double operator() ( const size_t& index0, const size_t& index1 ) const{
    return m( index0, index1 );
  }

  double operator() ( const std::string& label0, const std::string& label1 ) const{
    validateLabels( label0, label1 );
    return m( labels()(label0), labels()(label1) );
  }

  const SerialNumbersForStrings<size_t>& labels() const{
    return _labels;
  }

  const std::string& indexToLabel( const size_t& i ) const{
    return labels()(i);
  }

  // size0() returns the width (equivalently height) of the matrix, This
  // is not simple named size() because size() is used in FLENumMatrix to
  // mean the total number of elements, i.e. width x height.
  size_t size0() const{ return m.size0(); }



  // labelsNewOrder should be a permutation of the original labels
  void renumLabels( const SerialNumbersForStrings<size_t>& labelsNewOrder );

  bool symmetric() const;
  friend std::ostream& operator<<( std::ostream &os, const ConstantLabeledSquareDoubleFLENumMatrix& mat );
private:
  void validateLabels( const std::string& label0, const std::string& label1 ) const;

  void _renumLabels_die( const SerialNumbersForStrings<size_t>& newLabels ) const;

  // object data
  SerialNumbersForStrings<size_t> _labels;
  FLENumMatrixFast<double> m;
};


std::ostream& operator<<( std::ostream &os, const ConstantLabeledSquareDoubleFLENumMatrix& mat );

class LabeledIdentityDoubleMatrix : public ConstantLabeledSquareDoubleInterface{
public:
  double operator()( const size_t& index0, const size_t& index1 ) const{
    if( index0 == index1 ) return 1.0; return 0.0;
  }
  double operator()( const std::string& label0, const std::string& label1 ) const{
    if( label0 == label1 ) return 1.0; return 0.0;
  }  
  void renumLabels( const SerialNumbersForStrings<size_t>& labelsNewOrder ){} // noop.
};

const LabeledIdentityDoubleMatrix labeledIdentityDoubleMatrix;

}; // end namespace
#endif // defined _CONSTANTLABELEDSQUAREDOUBLEFLNUMMATRIX_HH_

