/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2009/05/17 11:54:23 $
 *
 *  Description: options to use when reading fasta format sequence files
 *
 */
#ifndef _FASTAINPUTOPTIONS_HH
#define _FASTAINPUTOPTIONS_HH
#include <iostream>
#include <boost/regex.hpp>
#include "utils/stl/stringUtils.hh"

namespace cbrc{

class FastaInputOptions{
public:

  /* ********** TYPEDEFS ********** */
  typedef  int  (*seqCharFilterT)(int);


  /* ********** CONSTRUCTORS ********** */
  FastaInputOptions(  const boost::regex&  betweenLabelsDelimiterRegex
		      =  defaultBetweenLabelsDelimiterRegex(),

		      const std::string&   commentMarker
		      =  defaultCommentMarker(),

		      const seqCharFilterT& seqCharFilter
		      =  defaultSeqCharFilter()

		      )
    : _betweenLabelsDelimiterRegex(  betweenLabelsDelimiterRegex ),
      _commentMarker              (  commentMarker               ),
      _seqCharFilter              (  seqCharFilter               )
  {}


  FastaInputOptions(  const FastaInputOptions& fio  )
    : _betweenLabelsDelimiterRegex(  fio._betweenLabelsDelimiterRegex ),
      _commentMarker              (  fio._commentMarker               ),
      _seqCharFilter              (  fio._seqCharFilter               )
  {}



  /* ********** ACCESSORS ********** */

  const boost::regex&  betweenLabelsDelimiterRegex() const{
    return _betweenLabelsDelimiterRegex;
  }

  const std::string&  commentMarker() const{
    return _commentMarker;
  }

  const seqCharFilterT&  seqCharFilter() const{
    return _seqCharFilter;
  }

  void setBetweenLabelsDelimiterRegex( const boost::regex& betweenLabelsDelimiterRegex ){
    _betweenLabelsDelimiterRegex = betweenLabelsDelimiterRegex;
  }

  void setCommentMarker( const std::string& commentMarker ){
    _commentMarker = commentMarker;
  }

  void setSeqCharFilter( const seqCharFilterT& seqCharFilter ){
    _seqCharFilter = seqCharFilter;
  }



  /* ********** GENERAL METHODS ********** */
  const bool isComment( const std::string& line ) const{
    return(  commentMarker().size()
	     ? stringUtils::startsWith( line, commentMarker() )
	     : false  );

    return commentMarker().size();
  }



  /* ********** DEFAULT PARAMETER VALUES ********** */

  static const boost::regex& defaultBetweenLabelsDelimiterRegex(){
    static const boost::regex _defaultBetweenLabelsDelimiterRegex( "\t" );
    return _defaultBetweenLabelsDelimiterRegex;
  }

  static const std::string&  defaultCommentMarker(){
    static const std::string _defaultCommentMarker( "#" );
    return _defaultCommentMarker;
  }

  static const seqCharFilterT& defaultSeqCharFilter(){
    static const seqCharFilterT  _defaultSeqCharFilter  =  std::isalpha;
    return _defaultSeqCharFilter;
  }

private:
  // object data
  boost::regex    _betweenLabelsDelimiterRegex;
  std::string     _commentMarker;
  seqCharFilterT  _seqCharFilter;
};


static FastaInputOptions defaultFastaInputOptions;


inline std::ostream& operator<<( std::ostream& oStream, const FastaInputOptions& fio ){
  oStream << "beforeLabelsDelimiterRegex: " << fio.betweenLabelsDelimiterRegex() << std::endl;
  oStream << "commentMarker:              " << fio.commentMarker()               << std::endl;

  oStream << "seqCharFilter is set to "
	  << (  ( fio.seqCharFilter() == FastaInputOptions::defaultSeqCharFilter() )
		? "" : "non-"  )
	  << "default value\n";

    return oStream;
}
} // end namespace cbrc
#endif // _FASTAINPUTOPTIONS_HH
