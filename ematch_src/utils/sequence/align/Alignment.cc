/*
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2004, 2006, Paul B. Horton, All rights reserved.
 *  Creation Date: 2004.4.13
 *  Last Modified: $Date: 2008/01/06 07:41:56 $
 *  
 *  Copyright: All rights reserved
 *  
 *  Description: See header files.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Alignment.hh"
#include "utils/perlish/perlish.hh"
#include "utils/sequence/ResidueColorGenebee.hh"

namespace cbrc{

void Alignment::assertStringsValid() const{
  if( s0.size() != s1.size() ){
    std::cout << __FILE__ << ":" << __LINE__ 
	      << "s0.size() = " << s0.size() << " ≠　"
	      << "s1.size() = " << s1.size() << std::endl;
    exit( -1 );
  }
  for( size_t i = 0; i < size(); ++i ){
    if(   !( rim.validResidueIndex( s0[i] ) || (s0[i] == indelChar()) )   ){
      std::cout << __FILE__ << ":" << __LINE__ 
		<< "s0[" << i << "] = " << (size_t) s0[i] << std::endl;
      exit( -1 );
    }
  }
  for( size_t i = 0; i < size(); ++i ){
    if(   !( rim.validResidueIndex( s1[i] ) || (s1[i] == indelChar()) )   ){
      std::cout << __FILE__ << ":" << __LINE__ 
		<< "s1[" << i << "] = " << (size_t) s1[i] << std::endl;
      exit( -1 );
    }
  }
}
      

alignScoreT Alignment::score( const AminoScore& ad ) const{
  alignScoreT retVal = 0;
  bool inGap = false;
  for( size_t i = 0; i < size(); ++i ){
    if( (s0[i] == indelChar()) || (s1[i] == indelChar()) ){
      if( !inGap ){
	inGap = true;
	retVal += ad.gapInitiation();
      }
      retVal += ad.gapExtension();
      continue;
    }
    inGap = false;
    retVal += ad.score( s0[i], s1[i] );
  }
  return retVal;
}

double Alignment::percentIdentity() const{
  size_t numMatches = 0;
  for( size_t i = 0; i < size(); ++i )  numMatches += s0[i] == s1[i];
  return(  200 * numMatches / static_cast<double>( s0.size() + s1.size() )  );
}

void Alignment::outputHtml( std::ostream& os ) const{
  const ResidueColorGenebee rc;
  assert( s0.size() == s1.size() );
  const std::string htmlIndelCharacter( "-" );
  const unsigned int rowLength=75;
  size_t numBasesSoFar0 = 0; // number of bases from sequence 0 printed so far.
  size_t numBasesSoFar1 = 0;
  size_t maxDigits = 1;
  for( size_t i = 10; s0.size() >= i; ++maxDigits, i *= 10 );
  os << "<TT>\n";
  for( unsigned int i = 0; i < size(); ){
    os << "<NOBR>\n";
    for( unsigned int j = 0; j < rowLength; ++j ){
      if( j+i >= size() ) break;
      if( s0[i+j] == indelChar() ) os << htmlIndelCharacter;
      else{
	assert( (size_t) s0[i+j] < rim.sigma() );
	if( s0[i+j] == s1[i+j] ){
	  os << "<FONT color=\"" << rc.getColor( rim.toResidue(s0[i+j]) ) << "\">";
	}
	os << rim.toResidue( s0[i+j] );
	++numBasesSoFar0;
	if( s0[i+j] == s1[i+j] ){
	  os << "</FONT>";
	}
      }
    }
    std::string numBasesSoFarString( perlish::toString( numBasesSoFar0 ) );
    os << " ";
    for( size_t col = 0; col < maxDigits - numBasesSoFarString.size(); ++col ) os << "&nbsp;";
    os << numBasesSoFarString;
    os << "</NOBR><BR>\n";
    os << "<NOBR>\n";
    for( unsigned int j = 0; j < rowLength; ++j, ++i ){
      if( i >= size() ) break;
      if( s1[i] == indelChar() ) os << htmlIndelCharacter;
      else{
	if( s0[i] == s1[i] ){
	  os << "<FONT color=\"" << rc.getColor( rim.toResidue(s1[i]) ) << "\">";
	}
	os << rim.toResidue( s1[i] );
	++numBasesSoFar1;
	if( s0[i] == s1[i] ){
	  os << "</FONT>";
	}
      }
    }
    numBasesSoFarString = perlish::toString( numBasesSoFar1 );
    os << " ";
    for( size_t col = 0; col < maxDigits - numBasesSoFarString.size(); ++col ) os << "&nbsp;";
    os << numBasesSoFarString;
    os << "</NOBR><BR>\n" << "<BR>\n";
  }
  os << "</TT>\n";
}

void Alignment::outputMarkSame( const std::string& outputFormat,
				std::ostream& os,
				const bool markDifferent,
				const char markChar ){
  if(      outputFormat == "text" ){
    outputTextMarkSame( os, markDifferent, markChar );
  }
  else if( outputFormat == "html" ){
    outputHtmlMarkSame( os, markDifferent, markChar );
  }
  else{
    /* debug */ std::cout << __FILE__ << ":" << __LINE__
			  << " Error: Alignment does not support output type: "
			  << outputFormat << std::endl
			  << " supported formats are "Q(ALIGNMENT_OUTPUT_FORMATS)"\n";
    exit( -1 );
  }
}

void Alignment::outputHtmlMarkSame( std::ostream& os,
				    const bool markDifferent, const char markChar ) const{
  const ResidueColorGenebee rc;
  assert( s0.size() == s1.size() );
  const std::string htmlIndelCharacter( "-" );
  const unsigned int rowLength=75;
  size_t numBasesSoFar0 = 0; // number of bases from sequence 0 printed so far.
  size_t numBasesSoFar1 = 0;
  size_t maxDigits = 1;
  for( size_t i = 10; s0.size() >= i; ++maxDigits, i *= 10 );
  os << "<TT>\n";
  for( unsigned int i = 0; i < size(); ){
    // print this part of first sequence.
    os << "<NOBR>\n";
    for( unsigned int j = 0; j < rowLength; ++j ){
      if( j+i >= size() ) break;
      if( s0[i+j] == indelChar() ) os << htmlIndelCharacter;
      else{ // not indel character
	assert( (size_t) s0[i+j] < rim.sigma() );
	os << "<FONT color=\"" << rc.getColor( rim.toResidue(s0[i+j]) ) << "\">";
	os << rim.toResidue( s0[i+j] );
	++numBasesSoFar0;
	os << "</FONT>";
      }
    }
    std::string numBasesSoFarString( perlish::toString( numBasesSoFar0 ) );
    os << " ";
    for( size_t col = 0; col < maxDigits - numBasesSoFarString.size(); ++col ) os << "&nbsp;";
    os << numBasesSoFarString;
    os << "</NOBR><BR>\n";
    // print this part of second sequence.
    os << "<NOBR>\n";
    for( unsigned int j = 0; j < rowLength; ++j ){
      if( i+j >= size() ) break;
      if( s1[i+j] == indelChar() ) os << htmlIndelCharacter;
      else{
	os << "<FONT color=\"" << rc.getColor( rim.toResidue(s1[i+j]) ) << "\">";
	os << rim.toResidue( s1[i+j] );
	++numBasesSoFar1;
	os << "</FONT>";
      }
    }
    numBasesSoFarString = perlish::toString( numBasesSoFar1 );
    os << " ";
    for( size_t col = 0; col < maxDigits - numBasesSoFarString.size(); ++col ) os << "&nbsp;";
    os << numBasesSoFarString;
    os << "</NOBR><BR>\n";
    // print mark.
    os << "<NOBR>\n";
    for( unsigned int j = 0; j < rowLength; ++j, ++i ){
      if( i >= size() ) break;
      if( (s0[i] == s1[i]) != markDifferent )  os << markChar;
      else os << "&nbsp;";
    }
    os << "</NOBR><BR>\n" << "<BR>\n";      

  }
  os << "</TT>\n";
}


void Alignment::outputTextMarkSame( std::ostream& os,
				    const bool markDifferent, const char markChar ) const{
  assert( s0.size() == s1.size() );
  const unsigned int rowLength=75;
  size_t numBasesSoFar0 = 0; // number of bases from sequence 0 printed so far.
  size_t numBasesSoFar1 = 0;
  size_t maxDigits = 1;
  for( size_t i = 10; s0.size() >= i; ++maxDigits, i *= 10 );
  std::string numBasesSoFarFormat( std::string( "%" ) 
				   + perlish::toString( maxDigits ) 
				   + std::string( "d" ) );

  for( unsigned int i = 0; i < size(); /* i is incremented inside this loop */ ){
    if( i )  std::cout << std::endl;

    // print this row's portion of s0
    for( unsigned int j = 0; j < rowLength; ++j ){
      unsigned int idx = i+j;
      if( idx >= size() ) break;
      if( s0[idx] == indelChar() ) os << indelChar();
      else{
	if( (size_t) s0[idx] > rim.sigma() ){ // invalid base character, print as number.
	  os << "(" << (size_t) s0[idx] << ")";
	}else{
	  os << rim.toResidue( s0[idx] );
	}
	++numBasesSoFar0;
      }
    }
    os << " " << perlish::toString( numBasesSoFar0, numBasesSoFarFormat ) << std::endl;
    
    // print this row's portion of s1
    for( unsigned int j = 0; j < rowLength; ++j ){
      unsigned int idx = i+j;
      if( idx >= size() ) break;
      if( s1[idx] == indelChar() ) os << indelChar();
      else{
	os << rim.toResidue( s1[idx] );
	++numBasesSoFar1;
      }
    }
    os << " " << perlish::toString( numBasesSoFar1, numBasesSoFarFormat ) << std::endl;

    // print same or different mark.
    // Actung! In this (and only this loop) i gets incremented.
    for( unsigned int j = 0; j < rowLength; ++j, ++i ){
      unsigned int idx = i;
      if( idx >= size() ) break;
      if( s1[idx] == indelChar() ) os << " ";
      else{
	if( (s0[idx] == s1[idx]) != markDifferent )  os << markChar;
	else  os << " ";
      }
    }
    os << std::endl;
  }
}


std::ostream& operator<<( std::ostream& os, const Alignment& al ){
  assert( al.s0.size() == al.s1.size() );
  const unsigned int rowLength=75;
  size_t numBasesSoFar0 = 0; // number of bases from sequence 0 printed so far.
  size_t numBasesSoFar1 = 0;
  size_t maxDigits = 1;
  for( size_t i = 10; al.s0.size() >= i; ++maxDigits, i *= 10 );
  std::string numBasesSoFarFormat( std::string( "%" ) 
				   + perlish::toString( maxDigits ) 
				   + std::string( "d" ) );
  for( unsigned int i = 0; i < al.size(); ){
    if( i )  std::cout << std::endl;
    for( unsigned int j = 0; j < rowLength; ++j ){
      if( j+i >= al.size() ) break;
      if( al.s0[i+j] == al.indelChar() ) os << al.indelChar();
      else{
	if( (size_t) al.s0[i+j] > al.rim.sigma() ){ // invalid base character, print as number.
	  os << "(" << (size_t) al.s0[i+j] << ")";
	}else{
	  os << al.rim.toResidue( al.s0[i+j] );
	}
	++numBasesSoFar0;
      }
    }
    os << " " << perlish::toString( numBasesSoFar0, numBasesSoFarFormat ) << std::endl;
    for( unsigned int j = 0; j < rowLength; ++j, ++i ){
      if( i >= al.size() ) break;
      if( al.s1[i] == al.indelChar() ) os << al.indelChar();
      else{
	os << al.rim.toResidue( al.s1[i] );
	++numBasesSoFar1;
      }
    }
    os << " " << perlish::toString( numBasesSoFar1, numBasesSoFarFormat ) << std::endl;
  }
  return os;
}


}; // end namespace
