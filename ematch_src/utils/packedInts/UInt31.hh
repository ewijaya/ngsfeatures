/*
 *  Author: Paul B. Horton
 *
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.6
 *  
 *  Copyright: All rights reserved
 *  
 *  
 *  Purpose: Provide storage for a three byte unsigned int, val1, and an unsigned char, val0.
 *               
 *
 *  Description: The storage looks like this:
 *
 *              |-----------------val1---------------------val1-------------------------|---------val0----------|
 *               31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 *  The class method int cmpAscendVal0( const void* a, const void* b ) is provided for sorting with qsort.
 */

#ifndef _UINT31_HH_
#define _UINT31_HH_

namespace cbrc{

class UInt31 {
public:
  UInt31(){ q = 0; }
  UInt31( unsigned int val0, unsigned char val1 ){
    setVal0( val0 ); setVal1( val1 );
  }
  UInt31( const UInt31& ui31 ){ q = ui31.q; }
  void setVal0( unsigned char val0 ){
    q &= 0xFFFFFF00;
    q |= val0;
  }
  void setVal1( unsigned int val1 ){
    q &= 0x000000FF;
    q |=  ( val1 << 8 );
  }
  unsigned char getVal0() const{
    return( (unsigned char) q & 0x000000FF );
  }
  unsigned int getVal1() const{
    return( q >> 8 );
  }
  UInt31 operator=( const UInt31& ui31 ){ q = ui31.q; return *this; }
  bool operator==( const UInt31& ui31 ) const{ return q == ui31.q; }
  bool operator!=( const UInt31& ui31 ) const{ return q != ui31.q; }
  
  // for sorting. compares with val0 as the major key. val1 used for tie breaking.
  static int cmpAscendVal0( const void* a, const void* b ){
    unsigned char v0a = ((UInt31*) a)->getVal0();
    unsigned char v0b = ((UInt31*) b)->getVal0();
    if( v0a > v0b ) return 1;
    if( v0a < v0b ) return -1;
    unsigned int v1a = ((UInt31*) a)->getVal1();
    unsigned int v1b = ((UInt31*) b)->getVal1();
    if( v1a > v1b ) return 1;
    if( v1a < v1b ) return -1;
    return 0;
  }
  friend std::ostream& operator<<( std::ostream& os, const UInt31& ui31 );
  void friend testUInt31();
private:
  unsigned int q;
};


std::ostream& operator<<( std::ostream& os, const UInt31& ui31 ){
  os << "(" << (unsigned int) ui31.getVal0() << "," << ui31.getVal1() << ")";
  return os;
}

}; // end namespace
#endif // defined _UINT31_HH_

