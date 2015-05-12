/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.12
 *  Last Modified: $Date: 2006/09/14 01:27:31 $
 *
 *  Description: 
 *
 *  Purpose: (high level purpose or motivation)
 *
 */
#ifndef _TRANSFACTEXTLABELINFO_HH
#define _TRANSFACTEXTLABELINFO_HH
#include "utils/perlish/perlish.hh"
#include "TransfacTextLine.hh"
#include <iostream>

namespace cbrc{

class TransfacTextLabelInfo{
public:
  TransfacTextLabelInfo()
    : 
    _versionLabel(         "VV" ),
    _accessionLabel(       "AC" ),
    _bindingFactorLabel(   "BF" ),
    _blankLineLabel(       "XX" ),
    _descriptionLabel(     "DE" ),
    _freqMatrixLabel(      "P0" ),
    _nameLabel(            "NA" ),
    _recordSeparatorLabel( "//" )
  {}
  bool isVersion( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _versionLabel );
  }
  bool isAccession( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _accessionLabel );
  }
  bool isBindingFactor( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _bindingFactorLabel );
  }
  bool isBlankLine( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _blankLineLabel );
  }
  bool isDescription( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _descriptionLabel );
  }
  bool isFreqMatrix( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _freqMatrixLabel );
  }
  bool isFreqMatrixBody( const TransfacTextLine& transfacLine ) const{
    return(   perlish::looksLikeUnsignedInt( transfacLine.label() )   );
  }
  bool isName( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _nameLabel );
  }
  bool isRecordSeparator( const TransfacTextLine& transfacLine ) const{
    return( transfacLine.label() == _recordSeparatorLabel );
  }
private:
  const std::string _versionLabel;
  const std::string _accessionLabel;
  const std::string _bindingFactorLabel;
  const std::string _blankLineLabel;
  const std::string _descriptionLabel;
  const std::string _freqMatrixLabel;
  const std::string _nameLabel;
  const std::string _recordSeparatorLabel;
};

}; // end namespace cbrc
#endif // _TRANSFACTEXTLABELINFO_HH
