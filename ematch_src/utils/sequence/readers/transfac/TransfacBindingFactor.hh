/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton 2006, All rights reserved.
 *  Creation Date: 2006.9.9
 *  Last Modified: $Date: 2006/09/12 06:49:27 $
 *
 *  Description: 
 *
 *  Purpose: (high level purpose or motivation)
 *
 */
#ifndef _TRANSFACBINDINGFACTOR_HH
#define _TRANSFACBINDINGFACTOR_HH
#include <iostream>

namespace cbrc{

class TransfacBindingFactor{
public:
  TransfacBindingFactor( const std::string& transfacBFString ){ set( transfacBFString ); }
  TransfacBindingFactor(){}
  void set( const std::string& transfacBFString );
  std::string id() const{ return _id; }
  std::string name() const{ return _name; }
  std::string speciesName() const{ return _speciesName; }
  std::string genusName() const{ return _genusName; }
  std::string speciesCommonName() const{ return _speciesCommonName; }
private:
  std::string _id;
  std::string _name;
  std::string _genusName;
  std::string _speciesName;
  std::string _speciesCommonName;
};

std::ostream& operator<<( std::ostream& os,
			  const TransfacBindingFactor& tbf );


}; // end namespace cbrc
#endif // _TRANSFACBINDINGFACTOR_HH
