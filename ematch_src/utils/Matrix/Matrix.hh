/*
 *  Author: Wataru B. Fujibuchi
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Creation Date: 2003.06.12
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: General Matrix class for handling elements
 *
 *  Usage: (explain or give example of use, not necessary the same as the command line useage message)
 *
 *  Description: (reasonably detailed description of what this file does)
 *
 *  Reference: (Reference to relevant literature, only applicable sometimes)
 */

#ifndef _MATRIX_HH_
#define _MATRIX_HH_

#include <cstdlib>

namespace cbrc{

// You may use int, float, double, or even your own objects
template <class T>
class Matrix {
public:

  // The type of elements to store in this Matrix
  typedef T element_type;

  // Constructors
  Matrix(); //size()==0
  Matrix(size_t, size_t);
  Matrix(const Matrix<T>&); //copy

  // Destructors
  ~Matrix();

  // Operators
  Matrix<T>& operator=(const Matrix<T>&); //copy
  element_type* operator[](size_t) const;

  // Members
  size_t column_size();
  size_t row_size();
  void resize(size_t, size_t);
  bool check_valid(size_t r, size_t c) { return valid_flag[r][c]; }
  void set_valid(size_t r, size_t c, bool validity = 1) { valid_flag[r][c] = validity; }

private:
  element_type **mtrx;
  bool **valid_flag;
  size_t column, row;

  // Delete matrix
  void delete_matrix();
  void allocate_matrix(size_t, size_t);
};


// Constructors
template <class T>
Matrix<T>::Matrix() { mtrx = NULL; valid_flag = NULL; column = 0; row = 0; }

template <class T>
Matrix<T>::Matrix(size_t r, size_t c) {
  allocate_matrix(r, c);
  row = r;
  column = c;
}

// Copy
template <class T>
Matrix<T>::Matrix(const Matrix<T>& mm) {
  row = column = 0;
  *this = mm;
}

// Operators
template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& mm) {

  row = column = 0;
  resize(mm.row, mm.column);

  for(int i = 0; i < row; i++)
    for(int j = 0; j < column; j++) {
      mtrx[i][j] = mm.mtrx[i][j];
      valid_flag[i][j] = mm.valid_flag[i][j];
    }
  return *this;
}

template <class T>
T* Matrix<T>::operator[](size_t r) const {
  return mtrx[r];
}

// Members
template <class T>
size_t Matrix<T>::column_size() {
  return column;
}

template <class T>
size_t Matrix<T>::row_size() {
  return row;
}

template <class T>
void Matrix<T>::resize(size_t r, size_t c) {
  delete_matrix();
  allocate_matrix(r, c);
  row = r;
  column = c;
}

// Destructors
template <class T>
Matrix<T>::~Matrix() {
  delete_matrix();
}

// Private functions
template <class T>
void Matrix<T>::delete_matrix() {
  if(row) {
    for(int i = 0; i < row; i++) {
      delete [] mtrx[i];
      delete [] valid_flag[i];
    }
    if(mtrx) delete [] mtrx;
    if(valid_flag) delete [] valid_flag;
  }
}

template <class T>
void Matrix<T>::allocate_matrix(size_t r, size_t c) {
  mtrx = new element_type* [r];
  valid_flag = new bool* [r];
  for(int i = 0; i < r; i++) {
    mtrx[i] = new element_type [c];
    valid_flag[i] = new bool [c];
    memset(valid_flag[i], true, c);
  }
}


} // end namespace
#endif // defined _MATRIX_HH_
