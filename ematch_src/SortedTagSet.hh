/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.6.21
 *  Last Modified: $Date: 2009/09/12 02:38:37 $
 *
 *  Description: 
 *
 *  Purpose: (high level purpose or motivation)
 *
 */
#ifndef SORTEDTAGSET_HH_
#define SORTEDTAGSET_HH_
#include <iostream>
#include <boost/foreach.hpp>
#include "utils/sequence/ResidueIndexMap/ResidueIndexMap.hh"
#include "utils/sequence/packedDNA/Sigma4FLArray.hh"

namespace cbrc{

class SortedTagSet{
public:


  /* ********** TYPEDEFS ********** */

  typedef  Sigma4FLArray                      tagSeqT;
  typedef  std::vector<tagSeqT>            tagSeqVecT;
  typedef  enum {binary, text}            fileFormatT;
  typedef  tagSeqVecT::const_iterator  const_iterator;
  typedef  tagSeqVecT::iterator              iterator;

  /* ********** CONSTRUCTORS ********** */

  SortedTagSet(  const tagSeqVecT&  vec  )
  : v(vec){
    initialSort();
  }

  SortedTagSet( std::istream& iStream, fileFormatT fileFormat = text ){
    if( fileFormat == text )  readFromTextStream  ( iStream );
    else                      readFromBinaryStream( iStream );
  }

  void initialSort(){
    std::sort( v.begin(), v.end() );
    v.resize(  unique( v.begin(),v.end() ) - v.begin()  );
  }    


  /* ********** ACCESSORS ********** */
  size_t  size() const  {  return v.size();  }

  bool has( const tagSeqT& tag ) const{
    return(  std::binary_search( v.begin(), v.end(), tag )  );
  }

  const tagSeqT&  operator()( const size_t& i ) const
  {  return v[i];  }

  // returns the index of the first element: element ≧ TAG,
  // or size() if TAG > max(element)
  size_t operator()( const tagSeqT& tag ) const{
    return   std::lower_bound( v.begin(), v.end(), tag )  -  v.begin();
  }

  // if TAG in set, return index of TAG. Otherwise die with error message
  size_t getSerialNumberOrDie( const tagSeqT& tag ) const{
    const size_t  retVal  =  (*this)(tag);
    GDB_ASSERTF(  (*this)(retVal) == tag,
		  "No serial number for string: \"%s\"", tag.c_str()  );
    return retVal;
  }


  // for iteration
  const_iterator  begin() const  {  return v.begin();  }
  const_iterator  end()   const  {  return v.end();    }


  /* ********** CLASS CONSTANTS ********** */

  static const ResidueIndexMap& residueIndexMap(){
    static const ResidueIndexMap _residueIndexMap( "0123" );
    return _residueIndexMap;
  }


  /* ********** CONVERSION METHODS ********** */
  
  tagSeqT  toTagSeq( const std::string& s ) const;

  std::string  toString( const tagSeqT& tag ) const{
    return  residueIndexMap().vectorToResidues( tag.toByteVector() );
  }
    

  
  /* ********** I/O METHODS ********** */

  void readFromBinaryStream( std::istream& iStream ){
    // not yet implemented
  }

  // format is one tag string per line, presorted with no duplicated.
  void readFromTextStream  ( std::istream& iStream );


  /* ********** METHODS FOR DEBUGGING ********** */

  void assertIsSorted() const;

private:
  // object data
  tagSeqVecT  v;
};



inline  std::ostream& operator<<( std::ostream& os, const SortedTagSet& tagSet ){

  BOOST_FOREACH( const SortedTagSet::tagSeqT& curTag, tagSet ){
    os  <<  curTag  <<  std::endl;
  }

  return os;
}



} // end namespace cbrc
#endif // SORTEDTAGSET_HH_
