/*
 *  Author: Paul B. Horton
 *
 *  Organization: Computational Biology Research Center, Japan
 *  
 *  Creation Date: 2003.6.6
 *  
 *  Copyright: All rights reserved
 *  
 *  Purpose: Provide storage for three unsigned integer values. The first val0 using two bytes
 *           and the second and third val1, val2 using three bytes. The value of val0 is used when
 *
 *  Description: The storage looks like this:
 *
 *              |-------------------------val2------------------val2--------------------|----------val1---------
 *               63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32
 *
 *               -------------------val1------------------------|------------val0------------val0---------------|
 *               31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 *  The class method int cmpAscendVal0( const void* a, const void* b ) is provided for sorting with qsort.
 *
 */
#ifndef _UINT332_HH_
#define _UINT332_HH_

namespace cbrc{

class UInt332 {
public:
  UInt332(){ q = 0; }
  UInt332( unsigned short int val0, unsigned int val1, unsigned int val2 ){
    setVal0( val0 ); setVal1( val1 ); setVal2( val2 );
  }
  UInt332( const UInt332& ui332 ){ q = ui332.q; }
  void setVal0( unsigned short int val0 ){
    unsigned long long int r = val0;
    q &= 0xFFFFFFFFFFFF0000LL;
    q |= r;
  }
  void setVal1( unsigned int val1 ){
    unsigned long long int r = val1;
    r <<= 16;
    q &= 0xFFFFFF000000FFFFLL;
    q |= r;
  }
  void setVal2( unsigned int val2 ){
    unsigned long long int r = val2;
    r <<= 40;
    q &= 0x000000FFFFFFFFFFLL;
    q |= r;
  }
  unsigned short int getVal0() const{
    return( (unsigned short int) q & 0x000000000000FFFFLL );
  }    
  unsigned int getVal1() const{
    unsigned long long int r = q & 0x000000FFFFFF0000LL;
    return( (unsigned int) (r >> 16) );
  }
  unsigned int getVal2() const{
    unsigned long long int r = q & 0xFFFFFF0000000000LL;
    return( (unsigned int) (r >> 40) );
  }
  UInt332 operator=( const UInt332& ui332 ){ q = ui332.q; return *this; }
  bool operator==( const UInt332& ui332 ) const{ return( q == ui332.q ); }
  bool operator!=( const UInt332& ui332 ) const{ return( q != ui332.q ); }

  // for sorting. compares with val0 as the major key. val1, val2 used in that order for tie breaking.
  static int cmpAscendVal0( const void* a, const void* b ){
    short unsigned int v0a = ((UInt332*) a)->getVal0();
    short unsigned int v0b = ((UInt332*) b)->getVal0();
    if( v0a > v0b ) return 1;
    if( v0a < v0b ) return -1;
    unsigned int v1a = ((UInt332*) a)->getVal1();
    unsigned int v1b = ((UInt332*) b)->getVal1();
    if( v1a > v1b ) return 1;
    if( v1a < v1b ) return -1;
    unsigned int v2a = ((UInt332*) a)->getVal2();
    unsigned int v2b = ((UInt332*) b)->getVal2();
    if( v2a > v2b ) return 1;
    if( v2a < v2b ) return -1;
    return 0;
  }
  friend std::ostream& operator<<( std::ostream& os, const UInt332& ui332 );
  friend void testUInt332();
private:
  unsigned long long int q;
};

std::ostream& operator<<( std::ostream& os, const UInt332& ui332 ){
  os << "(" << ui332.getVal0() << "," << ui332.getVal1() << "," << ui332.getVal2() << ")";
  return os;
}

};
#endif // defined _UINT332_HH_
