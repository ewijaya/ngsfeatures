/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.21
 *  Last Modified: $Date: 2009/09/20 11:46:02 $
 *
 *  Description: Container to map serial numbers to DNA or RNA sequences.
 *
 *  Purpose: Created as part of RECOUNT project.
 *
 */
#ifndef TAGSET_HH_
#define TAGSET_HH_
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/universalTypedefs.hh"
#include "utils/sorting/sortingUtils.hh"
#include "utils/sequence/ResidueIndexMap/ResidueIndexMap.hh"
#include "utils/sequence/packedDNA/sigma4bitPackingUtils.hh"


namespace cbrc{

class TagSet{
public:


  /* --------------- TYPEDEFS ---------------- */
  typedef  enum {binary, text}                  fileFormatT;
  typedef  sigma4bitPackingUtils::unpackedVecT  unpackedSeqT;


  /* --------------- CONSTRUCTORS ------------ */
  TagSet( std::istream& iStream, fileFormatT fileFormat = text ){
    if( fileFormat == text )  readFromTextStream  ( iStream );
    else                      readFromBinaryStream( iStream );
  }

  ~TagSet(){
    delete[] a;
    delete[] mem;
  }


  /* --------------- PUBLIC ACCESSORS --------------- */

  const size_t&  size() const       {  return _size;       }

  const size_t&  totalSize() const  {  return _totalSize;  }

  // return size (length) of ith sequence 
  size_t  size(  const size_t&  i  ) const{
    return   a[ i+1 ]  -  a[i];
  }

  size_t  sizeInBytes(  const size_t&  i  ) const{
    return  sigma4bitPackingUtils::numBytesNeededToStore( size(i) );
  }


  bool  has(  const std::string&  asciiSeq  ){
    return  has(  toUnpackedSeq( asciiSeq )  );
  }

  bool  has(  const unpackedSeqT&  unpackedSeq  ) const{
    const size_t  upperBoundIdx   =   upperBound( unpackedSeq );
    return   equal(  upperBoundIdx, unpackedSeq  );
  }
  

  // returns the serial number of unpackedSeq, assuming it is in the tag set.
  // use has() to test if a tag is in the tag set.
  size_t operator()(  const std::string&  asciiSeq  ) const{
    return   upperBound(  toUnpackedSeq( asciiSeq )  );
  }


  // returns the serial number of unpackedSeq, assuming it is in the tag set.
  // use has() to test if a tag is in the tag set.
  size_t operator()(  const unpackedSeqT&  unpackedSeq  ) const{
    return  upperBound( unpackedSeq );
  }


  byte  operator()(  const size_t&  seqIdx,  const size_t&  charIdx  ) const{
    byte  retVal;
    sigma4bitPackingUtils::assignNthUnpacked( retVal, a[seqIdx], charIdx );
    return retVal;
  }


  // if unpackedSeq in set, return its index, otherwise die with error message
  size_t  getSerialNumberOrDie(   const std::string&  asciiSeq   ) const{
    const  unpackedSeqT  unpackedSeq  =  toUnpackedSeq( asciiSeq );
    GDB_ASSERTF(  has( unpackedSeq ),
		  "No serial number for string: \"%s\"", asciiSeq.c_str()  );
    return  upperBound( unpackedSeq );
  }


  /* ---------- CLASS CONSTANTS ---------- */

  static const ResidueIndexMap& residueIndexMap(){
    static const ResidueIndexMap  _residueIndexMap( "0123" );
    return _residueIndexMap;
  }


  /* ---------- CONVERSION METHODS ---------- */
  
  std::string  toString(  const size_t& i  ) const{
    return   toString(  sigma4bitPackingUtils::toUnpacked( a[i], size(i) )  );
  }

  std::string  toString(   const unpackedSeqT&  unpackedSeq   ) const{
    return  residueIndexMap().vectorToResidues( unpackedSeq );
  }

  unpackedSeqT  toUnpackedSeq(   const std::string&  tagSeq  ) const{
    return  residueIndexMap().toResidueIndicesVector( tagSeq );
  }

  
  /* --------------- I/O METHODS --------------- */

  void readFromBinaryStream( std::istream& iStream ){
    // not yet implemented
  }

  // format is one tag string per line, presorted with no duplicated tags.
  void  readFromTextStream(  std::istream& iStream  );

private:
  // return true iff i_th tag seq is the same as seqToCompare.
  bool  equal(   const size_t&  i,  const unpackedSeqT&  seqToCompare  ) const;

  // returns the index of the first element: element ≧ unpackedSeq
  // or size() if TAG > max(element)
  size_t  upperBound(  const unpackedSeqT&  seqToMatch  ) const;

  void assertIsSorted() const;

  // Return true iff the i_th sequence is lexically less than the j_th sequence
  bool  isLexicallyLess(   const size_t&  i,  const size_t&  j   ) const{
    return  isLexicallyLess(  a[i], size(i),  a[j], size(j)  );
  }
			      

  // Return true iff i_th sequence is lexically less than seq2
  bool  isLexicallyLess(   const size_t&        i,
			   const byte* const&   packedSeqPtr2,
			   const size_t&        seqLen2
			   ) const{
    return  isLexicallyLess(  a[i], size(i),
			      packedSeqPtr2,
			      seqLen2  );
  }


  // Return true iff seq1 is lexically less than seq2,
  // packedSeqPtr1, packedSeqPtr2  point to packed representation of the sequences
  // minSizeInBytes should hold the minimum size in byte size of the two packed representations
  // lenCmp should hold {-1,0,1}, is len(seq1) is greater, equal, or less than len(seq2)
  bool  isLexicallyLess(   const byte* const&  packedSeqPtr1,
			   const size_t&       seqLen1,   //  len in characters
			   const byte* const&  packedSeqPtr2,
			   const size_t&       seqLen2    //  len in characters			   
			   ) const;

  // -------------------- object data --------------------
  byte**  a;
  // After initialization, a will hold size() + 1 pointers, pointing into the memory
  // reserved in mem below.
  // a[i] will point to the start of sequence i.
  // as a special case, a[size()] will point to one past the end of mem
  byte*   mem;   // memory to hold contents of sequences
  size_t  _size;
  size_t  _totalSize;
};



inline  std::ostream& operator<<( std::ostream& os, const TagSet& tagSet ){

  return os;
}



} // end namespace cbrc
#endif // TAGSET_HH_
