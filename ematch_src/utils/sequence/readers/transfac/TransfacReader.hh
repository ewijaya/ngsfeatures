/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright: Paul B. Horton 2006, All rights reserved.
 *  Creation Date: 2006.9.9
 *  Last Modified: $Date: 2006/09/14 11:23:37 $
 *
 *  Description: Class for reading transfac text files.
 *
 */
#ifndef _TRANSFACREADER_HH
#define _TRANSFACREADER_HH
#include <iostream>
#include "TransfacTextLine.hh"
#include "TransfacTextLabelInfo.hh"
#include "TransfacBindingFactor.hh"


namespace cbrc{

class TransfacReader{
public:
  TransfacReader( std::istream& is );
  std::string collectionHeader() const{ return _collectionHeader; }
  TransfacTextLine nextNonBlankLine() const;
  bool readRecord(); // returns false if no more records available in stream.
  std::string version()           const{ return _version; }
  std::string accession()         const{ return _accession; }
  std::string description()       const{ return _description; }
  std::string name()              const{ return _name; }
  TransfacBindingFactor bindingFactor() const{ return _bindingFactor; }

  std::string alphabet()          const{ return _alphabet; }
  std::vector< std::vector<unsigned int> > freqMatrix() const{ return _freqMatrix; }
  std::string consensusSequence() const{ return _consensusSequence; }
private:
  void readFreqMatrix();
  std::string _alphabet;
  std::string _version;
  std::string _accession;
  std::string _name;
  std::string _description;
  std::string _collectionHeader; // describes collection as a whole.
  std::vector< std::vector<unsigned int> > _freqMatrix;
  std::string _consensusSequence;
  TransfacBindingFactor _bindingFactor;
  TransfacTextLabelInfo labelInfo;
  TransfacTextLine transfacLine; // global variable amongst methods.
  std::istream& is;
};

}; // end namespace cbrc
#endif // _TRANSFACREADER_HH
