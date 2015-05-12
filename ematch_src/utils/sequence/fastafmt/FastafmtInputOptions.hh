/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.2.3
 *  Last Modified: $Date: 2008/02/03 23:21:22 $
 *
 *  Description: options to use when reading fasta format sequence files
 *
 */
#ifndef _FASTAFMTINPUTOPTIONS_HH
#define _FASTAFMTINPUTOPTIONS_HH
#include <iostream>
#include <boost/regex.hpp>

namespace cbrc{

class FastafmtInputOptions{
public:
  // constructor
  FastafmtInputOptions( const boost::regex& betweenLabelsDelimiterRegex = defaultBetweenLabelsDelimiterRegex(),
			const boost::regex& beforeNameDelimiterRegex    = defaultBeforeNameDelimiterRegex()
			)
    : _betweenLabelsDelimiterRegex( betweenLabelsDelimiterRegex ),
      _beforeNameDelimiterRegex(    beforeNameDelimiterRegex   )
  {}


  /* ***** accessors ***** */
  const boost::regex& betweenLabelsDelimiterRegex() const{
    return _betweenLabelsDelimiterRegex;
  }

  const boost::regex& beforeNameDelimiterRegex() const{
    return _beforeNameDelimiterRegex;
  }

  void setBetweenLabelsDelimiterRegex( const boost::regex& betweenLabelsDelimiterRegex ){
    _betweenLabelsDelimiterRegex = betweenLabelsDelimiterRegex;
  }

  void setBeforeNameDelimiterRegex( const boost::regex& beforeNameDelimiterRegex ){
    _beforeNameDelimiterRegex = beforeNameDelimiterRegex;
  }


  /* ***** default values ***** */
  static const boost::regex& defaultBetweenLabelsDelimiterRegex(){
    static const boost::regex _defaultBetweenLabelsDelimiterRegex( "\t" );
    return _defaultBetweenLabelsDelimiterRegex;
  }

  static const boost::regex& defaultBeforeNameDelimiterRegex(){
    static const boost::regex _defaultBeforeNameDelimiterRegex( "\\s*" );
    return _defaultBeforeNameDelimiterRegex;
  }

private:
  // object data
  boost::regex _betweenLabelsDelimiterRegex;
  boost::regex _beforeNameDelimiterRegex;
};


static FastafmtInputOptions defaultFastaInputOptions;

} // end namespace cbrc
#endif // _FASTAFMTINPUTOPTIONS_HH
