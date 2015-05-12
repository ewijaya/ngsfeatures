/*  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2006.9.11
 *  Last Modified: $Date: 2006/09/14 11:24:02 $
 *  Description: See header file.
 */
#include "utils/perlish/perlish.hh"
#include "TransfacReader.hh"


namespace cbrc{
  
TransfacReader::TransfacReader( std::istream& is ) 
  :
  labelInfo(),
  is(is)
{
    for( transfacLine = nextNonBlankLine();
	 !transfacLine.empty();
	 transfacLine = nextNonBlankLine() ){
      if(   labelInfo.isRecordSeparator( transfacLine )   )  return;
      if(   labelInfo.isVersion( transfacLine )   ){
	_version += transfacLine.content();
	continue;
      }
      std::cout << "Error: I expected only version or blank lines before first record.\n"
		<< "Last line seen: " << transfacLine << std::endl;
      exit( -1 );
    }
}

bool TransfacReader::readRecord(){
  for( transfacLine = nextNonBlankLine();
       true;
       transfacLine = nextNonBlankLine() ){
  BEGIN_PARSE_TRANFAC_LINE:
    //    std::cout << "in readRecord; " << transfacLine << std::endl;
    if( transfacLine.empty() ) return false; // not more records.

    if(   labelInfo.isRecordSeparator( transfacLine )   )  return true;

    if(   labelInfo.isAccession(       transfacLine )   ){
      _accession = transfacLine.content();
      continue;
    }
    if(   labelInfo.isBindingFactor(     transfacLine )   ){
      _bindingFactor.set( transfacLine.content() );
      continue;
    }
    if(   labelInfo.isDescription(     transfacLine )   ){
      _description = transfacLine.content();
      continue;
    }
    if(   labelInfo.isName(            transfacLine )   ){
      _name = transfacLine.content();
      continue;
    }
    if(   labelInfo.isFreqMatrix(      transfacLine )   ){
      readFreqMatrix();
      goto BEGIN_PARSE_TRANFAC_LINE; // readFreqMatrix reads one line past frequency matrix.
    }
    std::cerr << "** processing not yet coded for: " << transfacLine << std::endl;
  }
}


// Sets frequency matrix.
// Reads one line past frequence matrix in stream.
// Upon return, class global transfacLine is set to that line.
void  TransfacReader::readFreqMatrix(){
  _alphabet = perlish::join(   perlish::split( transfacLine.content() )   );
  _freqMatrix.clear();
  for( transfacLine = nextNonBlankLine();
       !transfacLine.empty();
       transfacLine = nextNonBlankLine() ){
    if( !labelInfo.isFreqMatrixBody( transfacLine ) ){
      return;
    }
    std::vector<std::string> fields = perlish::split( transfacLine.content() );
    if( fields.size() != 5 ){
      std::cout << __FILE__ << ":" << __LINE__
		<< " Error: expected 5 fields in frequency matrix body tranfac line: " 
		<< transfacLine << std::endl;
      exit( -1 );
    }
    _consensusSequence += fields[4];
    fields.pop_back();
    _freqMatrix.push_back(   perlish::toUnsignedInts( fields )   );
  }
  assert( 0 );
}
  

TransfacTextLine TransfacReader::nextNonBlankLine() const{
  TransfacTextLine retval;
  std::string line;
  for( line = perlish::slurpLine( is );
       line.size();
       line = perlish::slurpLine( is ) ){
    retval.set( line );
    if(   !labelInfo.isBlankLine( retval )   )  break;
  }
  if( line.size() ){
    return retval;
  }
  return TransfacTextLine(); // hit end of file, so return empty line.
}      
  
    
}; // end namespace cbrc

